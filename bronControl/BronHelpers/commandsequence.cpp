#include "commandsequence.h"
#include "devicepara.h"
#include "device.h"
#include "errors.h"
#include <QDebug>

using namespace Framework;


Framework::CommandSequence::CommandSequence(const uint32_t nr, QString name)
    : QObject()
    , QSharedData()
    , number(nr)
    , m_error(Errors::Id::E_NO_ERROR)
    , m_openForRegistering(true)
    , m_paras()
    , m_SequenceActions()
    , m_name(name){}

Framework::CommandSequence::CommandSequence(const CommandSequence &c)
    : QObject()
    , QSharedData()
    , number(c.number)
    , m_error(c.m_error)
    , m_openForRegistering(c.m_openForRegistering)
    , m_paras(c.m_paras)
    , m_SequenceActions(c.m_SequenceActions)
    , m_name(c.m_name){}

void Framework::CommandSequence::addAction(DeviceAction act){
    m_SequenceActions.append(act);
}

void Framework::CommandSequence::onRxResponse(std::weak_ptr<DevicePara> para, quint32 error){
    if(m_error != Errors::Id::E_NO_ERROR){
        m_error = static_cast<Errors::Id>(error);
    }
    if(m_paras.contains(para)){
        if(auto p = para.lock()){
            disconnect(p.get(), SIGNAL(rxResponse(std::weak_ptr<DevicePara>, quint32)), this, SLOT(onRxResponse(std::weak_ptr<DevicePara>, quint32)));
            if(m_paras.removeOne(para)){
                if(m_paras.size() == 0){
                    if(m_error == Errors::Id::E_NO_ERROR){
                        QMutableListIterator<DeviceAction> iter(m_SequenceActions);
                        while(iter.hasNext()){
                            DeviceAction a = iter.next();
                            a.triggerAction();
                        }
                    }
                    emit sequenceDone(number, m_error);
                }
            }
        }
    }
}
