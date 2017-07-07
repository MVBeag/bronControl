#include "remoteClientSM.h"
#include <QDebug>
#include <device.h>
#include <QTimer>
#include "devicesirros.h"
#include "devicescoro.h"
#include <QTime>

using namespace Framework;


Framework::RemoteClientSM::RemoteClientSM(Device &dev, QObject *parent)
    : QObject(parent)
    , m_device(dev)
    , m_Init("init", *this)
    , m_handleIQ("handleIQ", *this)
    , m_handleIQPQ("handleIQPQ", *this)
    , m_threadWaitTime(eStdWaitTime)
    , m_taskTimer(new QTimer())
    , m_abort(false)
    , m_wasRx()
    , m_wasCommand()
    , m_rxData()
    , m_initQueue()
    , m_pollingQueue()
    , m_commandQueue()
    , m_sequenceQueue()
    , m_retryQueue()
    , m_runningRequestQueue(){
    m_taskTimer->setSingleShot(true);
    m_taskTimer->setInterval(m_threadWaitTime);
    m_statePtr = &m_Init;
}

Framework::RemoteClientSM::~RemoteClientSM(){
    while(!m_rxData.isEmpty())
        m_rxData.dequeue();
    while(!m_commandQueue.isEmpty()){
        m_commandQueue.dequeue()->deleteLater();
    }
    while(!m_sequenceQueue.isEmpty()){
        m_sequenceQueue.dequeue()->deleteLater();
    }
    while(!m_initQueue.isEmpty()){
        m_initQueue.dequeue()->deleteLater();
    }
    while(!m_pollingQueue.isEmpty()){
        m_pollingQueue.dequeue()->deleteLater();
    }
    while(!m_retryQueue.isEmpty()){
        m_retryQueue.dequeue()->deleteLater();
    }
    while(!m_runningRequestQueue.isEmpty()){
        m_runningRequestQueue.dequeue();
    }
}

void Framework::RemoteClientSM::Run(bool &abort){
    QMutexLocker lock(&m_device.m_mutex);

    m_taskTimer->stop();

    if(m_wasRx.m_wasThis){
        while(!m_rxData.isEmpty()){
            rxData rx = m_rxData.dequeue();
            m_device.receiveData(rx.m_remoteAddr, rx.m_datagram.data(), rx.m_datagram.size() - 1);
        }
    }
    else{
        m_statePtr = m_statePtr->handleState();
    }

    if(!abort){
        m_taskTimer->setInterval(getThreadWaitTime());
        m_taskTimer->start();
    }
}

Errors::Id Framework::RemoteClientSM::checkMaxPollingReqeustsReached(Request *r){
    Errors::Id ret = Errors::Id::E_NO_ERROR;
    if(r->getQueueType() == Request::eQueueType::ePolling){
        if(m_device.isMaxRequestsReached()){
            ret =  Errors::Id::MANY_REQUESTS;
        }
        else{
            m_device.incRequests();
        }
    }
    return ret;
}

void Framework::RemoteClientSM::doWork(){
    Run(m_abort);
}

void Framework::RemoteClientSM::stopWork(){
    m_abort = true;
}

void Framework::RemoteClientSM::onHandleRx(RemoteAddressInfo ra, QByteArray datagram){
    QMutexLocker lock(&m_device.m_mutex);
    rxData rx(ra, datagram);
    m_rxData.enqueue(rx);
    m_wasRx.m_wasThis = true;
    m_wasRx.m_remaining = m_taskTimer->remainingTime();
    m_taskTimer->start(0);
}

Framework::State::State(QString name, RemoteClientSM &sm, QObject *parent)
    : QObject(parent)
    , m_Name(name)
    , m_sm(sm){}

Framework::Init::Init(QString name, RemoteClientSM &sm, QObject *parent)
    : State(name, sm, parent){}

State *Framework::Init::handleState(){

    /** Init goes unconditional to state m_handleIQ,
     * so give it back
     */
    m_sm.m_device.initMW();
    return &m_sm.m_handleIQ;
}


/** states *****************************************************************************************/
Framework::IQSHF::IQSHF(QString name, RemoteClientSM &sm, QObject *parent)
    : State(name, sm, parent){}

State *IQSHF::handleState(){
    /** starts handling the initQueue, do this until empty or to fast (mw says to much parameters)
     */

    // first handle rxsignals
    if(!m_sm.m_retryQueue.isEmpty()){
        Request *r = m_sm.m_retryQueue.head();

        Errors::Id err = m_sm.checkMaxPollingReqeustsReached(r);
        if(err == Errors::Id::E_NO_ERROR){
           err = m_sm.m_device.comCaller(r);
        }

        switch(err){
        case Errors::Id::E_NO_ERROR:
            // request lives in middleware
            if(auto p = r->getPara().lock()){
                m_sm.m_runningRequestQueue.enqueue(p->Id());
            }
            m_sm.m_retryQueue.dequeue();
            return this;
        case Errors::Id::MANY_REQUESTS:
            m_sm.setThreadWaitTime(RemoteClientSM::eTooManyWaitTime);
            return this;
        default:
            // silently discard all other messages
            m_sm.m_retryQueue.dequeue()->deleteLater();
            return this;
        }
    }
    else{
        if(!m_sm.m_initQueue.isEmpty()){
            Request *r = m_sm.m_initQueue.head();

            Errors::Id err = m_sm.checkMaxPollingReqeustsReached(r);
            if(err == Errors::Id::E_NO_ERROR){
                err = m_sm.m_device.comCaller(r);
            }

            switch(err){
            case Errors::Id::E_NO_ERROR:
                // request lives in middleware
                if(auto p = r->getPara().lock()){
                    m_sm.m_runningRequestQueue.enqueue(p->Id());
                }
                m_sm.m_initQueue.dequeue();
                return this;
            case Errors::Id::MANY_REQUESTS:
                m_sm.setThreadWaitTime(RemoteClientSM::eTooManyWaitTime);
                return this;
            default:
                // silently discard all other messages
                m_sm.m_initQueue.dequeue()->deleteLater();
                return this;
            }
        }
        else{
            if(!m_sm.m_runningRequestQueue.isEmpty()){
                m_sm.setThreadWaitTime(RemoteClientSM::eTooManyWaitTime);
                return this;
                // only wait for finishing
            }
            else{
                // if init and retry Queue stay emtpy, change state, be careful about retries, initQueue could be empty and retries are not registered
                // this is the case during retry timeout
                if((m_sm.m_initQueue.size() == 0) && m_sm.m_retryQueue.size() == 0 && m_sm.m_runningRequestQueue.size() == 0){
                    emit m_sm.m_device.initParasRead(m_sm.m_device.shared_from_this());
                    return &m_sm.m_handleIQPQ;
                }
                else{
                    qDebug() << QString("QueueSizes: I: ") << QString::number(m_sm.m_initQueue.size())
                             << " C: " << QString::number(m_sm.m_commandQueue.size())
                             << " P: " << QString::number(m_sm.m_pollingQueue.size())
                             << " R: " << QString::number(m_sm.m_retryQueue.size())
                             << " S: " << QString::number(m_sm.m_runningRequestQueue.size());
                    return this;
                }
            }
        }
    }
}

Framework::IQPQSHF::IQPQSHF(QString name, RemoteClientSM &sm, QObject *parent)
    : State(name, sm, parent){}

State *Framework::IQPQSHF::handleState(){
    // check sequences first
    if(!m_sm.m_sequenceQueue.isEmpty()){
        Request *r = m_sm.m_sequenceQueue.head();

        Errors::Id err = m_sm.checkMaxPollingReqeustsReached(r);
        if(err == Errors::Id::E_NO_ERROR){
            err = m_sm.m_device.comCaller(r);
        }
        switch(err){
        case Errors::Id::E_NO_ERROR:
            // request lives now in middleware
            if(auto p = r->getPara().lock()){
                m_sm.m_runningRequestQueue.enqueue(p->Id());
            }
            m_sm.m_sequenceQueue.dequeue();
            // give client some time to react
            m_sm.setThreadWaitTime(RemoteClientSM::eWriteWaitTime);
            return this;
        case Errors::Id::MANY_REQUESTS:
            m_sm.setThreadWaitTime(RemoteClientSM::eTooManyWaitTime);
            return this;
        default:
            Q_ASSERT(false); // this point should never happen, check return values of used
            //functions
            // silently discard all other messages
            m_sm.m_sequenceQueue.dequeue()->deleteLater();
            return this;
        }
    }
    else{
        // second check retries
        if(!m_sm.m_retryQueue.isEmpty()){
            Request *r = m_sm.m_retryQueue.head();

            Errors::Id err = m_sm.checkMaxPollingReqeustsReached(r);
            if(err == Errors::Id::E_NO_ERROR){
                err = m_sm.m_device.comCaller(r);
            }

            switch(err){
            case Errors::Id::E_NO_ERROR:
                // request lives now in middleware
                if(auto p = r->getPara().lock()){
                    m_sm.m_runningRequestQueue.enqueue(p->Id());
                }
                m_sm.m_retryQueue.dequeue();
                return this;
            case Errors::Id::MANY_REQUESTS:
                m_sm.setThreadWaitTime(RemoteClientSM::eTooManyWaitTime);
                return this;
            default:
                Q_ASSERT(false); // this point should never happen, check return values of used
                //functions
                // silently discard all other messages
                m_sm.m_retryQueue.dequeue()->deleteLater();
                return this;
            }
        }
        else{
            // then check if commandQueue contains data
            if(!m_sm.m_commandQueue.isEmpty()){
                Request *r = m_sm.m_commandQueue.head();

                Errors::Id err = m_sm.checkMaxPollingReqeustsReached(r);
                if(err == Errors::Id::E_NO_ERROR){
                    err = m_sm.m_device.comCaller(r);
                }
                switch(err){
                case Errors::Id::E_NO_ERROR:
                    // request live now in middlware
                    if(auto para = r->getPara().lock()){
                        if(para->GetElder().checkCommandwoResponse(r->getId())){
                            m_sm.m_commandQueue.dequeue()->deleteLater();
                        }
                        else{
                            m_sm.m_runningRequestQueue.enqueue(para->Id());
                            m_sm.m_commandQueue.dequeue();
                        }
                    }
                    else{
                        // para seems not to be valid anymore, remove command silently
                        m_sm.m_commandQueue.dequeue()->deleteLater();
                    }
                    return this;

                case Errors::Id::MANY_REQUESTS:
//                    qDebug() << m_sm.m_device.getName() << " COMMAND DEVICE PIPE FULL!!!!!!!!!!!!!!!!!!!!!!!";

                    m_sm.setThreadWaitTime(RemoteClientSM::eTooManyWaitTime);
                    return this;
                default:
                    // silently discard all other messages
                    m_sm.m_commandQueue.dequeue()->deleteLater();
                    return this;
                }
            }else{
//                if(m_sm.m_device.getName() == "Siros 400 S"){
//                    qDebug() << "poll Siros 400 S polling "
//                             << m_sm.m_device.isPolling()
//                             << " queue size " << m_sm.m_pollingQueue.size();
//                }
                if(m_sm.m_device.isPolling()){
                    if(!m_sm.m_pollingQueue.isEmpty()){
                        // handle polling queue if no command triggered
                        Request *r = m_sm.m_pollingQueue.head();
                        //                    qDebug() << r->getPara()->GetElder().IdString(r->getId())
                        //                             << " "
                        //                             << r->getPara()->GetElder().getName();

                        Errors::Id err = m_sm.checkMaxPollingReqeustsReached(r);
                        if(err == Errors::Id::E_NO_ERROR){
                            err = m_sm.m_device.comCaller(r);
                        }
                        switch(err){
                        case Errors::Id::E_NO_ERROR:
                            // polling queue is never emptied
                            if(auto p = r->getPara().lock()){
                                m_sm.m_runningRequestQueue.enqueue(p->Id());
                            }
                            m_sm.m_pollingQueue.enqueue(m_sm.m_pollingQueue.dequeue());
                            return this;
                        case Errors::Id::MANY_REQUESTS:
                            m_sm.setThreadWaitTime(RemoteClientSM::eTooManyWaitTime);
                            return this;
                        default:
                            //functions
                            // do nothing, maybe we should silently remove the element
                            return this;
                        }
                    }
                    else{
                        m_sm.setThreadWaitTime(RemoteClientSM::eBoaringTime);
                        return this;
                    }
                }
                else{
                    m_sm.setThreadWaitTime(RemoteClientSM::eBoaringTime);
                    return this;
                }
            }
        }
    }
    return this;
}

int Framework::RemoteClientSM::getThreadWaitTime(){
    QMutexLocker lock(&m_device.m_mutex);

    if(m_wasRx.m_wasThis){
        m_wasRx.m_wasThis = false;
        if(-1 == m_wasRx.m_remaining){
            return m_threadWaitTime;
        }
        else{
            int ret = m_wasRx.m_remaining;
            m_wasRx.m_remaining = -1;
            return ret;
        }
    }
    else if(m_wasCommand.m_wasThis){
        m_wasCommand.m_wasThis = false;
        if(-1 == m_wasCommand.m_remaining){
            return m_threadWaitTime;
        }
        else{
            int ret = m_wasCommand.m_remaining;
            m_wasCommand.m_remaining = -1;
            return ret;
        }
    }
    else{
        return m_threadWaitTime;
    }
    return 1000;
}

void Framework::RemoteClientSM::onEnqueueCommandQueue(Request *r){
    r->setQueueType(Request::eQueueType::eCommand);
    m_commandQueue.enqueue(r);
    m_taskTimer->stop();
    m_wasCommand.m_wasThis = true;
    m_wasCommand.m_remaining = m_taskTimer->remainingTime();
    m_taskTimer->start(0);
}

void Framework::RemoteClientSM::onEnqueueSequenceQueue(Request *r){
    r->setQueueType(Request::eQueueType::eSequence);
    m_sequenceQueue.enqueue(r);
    m_taskTimer->stop();
    m_wasCommand.m_wasThis = true;
    m_wasCommand.m_remaining = m_taskTimer->remainingTime();
    m_taskTimer->start(0);
}

void Framework::RemoteClientSM::onDequeueRunningRequest(int id){
    if(m_runningRequestQueue.contains(id)){
        m_runningRequestQueue.removeOne(id);
    }
}

void Framework::RemoteClientSM::onRemoveParaFromQueue(Request::eQueueType type, uint32_t id){
    Q_UNUSED(type)
    // search all types of queue to remove the parameter
    QQueue<Request *> queues[5] = {
        m_initQueue,
        m_pollingQueue,
        m_commandQueue,
        m_sequenceQueue,
        m_retryQueue
    };

    for(int i = 0; i < 5; i++){
        QMutexLocker lock(&m_device.m_mutex);
        QMutableListIterator<Request *> iter(queues[i]);
        while(iter.hasNext()){
            // no need to check about the device type, we are within a device
            if(iter.next()->getId() == id){
                iter.remove();
            }
        }
    }

}
