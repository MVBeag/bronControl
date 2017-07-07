// This header file was generated using VisualSC's Export Code Function

// Visual Statechart is an open source project. Visit us at https://github.com/daviddrell/visualsc
#ifndef REMOTECLIENTSM_H
#define REMOTECLIENTSM_H

#include <memory>
#include <QObject>
#include <QStateMachine>
#include <QFinalState>
#include <QQueue>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QPointer>
#include <QThread>
#include "errors.h"
#include "request.h"
#include "remoteaddressinfo.h"

namespace Framework{

class Request;
class RemoteClientSM;
class Device;


class rxData : public QObject{
    Q_OBJECT
    public:
    rxData(RemoteAddressInfo rAddr, QByteArray data)
        : QObject()
        , m_remoteAddr(rAddr)
        , m_datagram(data){}
    rxData(const rxData &d)
        : QObject()
        , m_remoteAddr(d.m_remoteAddr)
        , m_datagram(d.m_datagram){}

    rxData & operator=(const rxData &d){
        m_remoteAddr = d.m_remoteAddr;
        m_datagram = d.m_datagram;
        return *this;
    }


    RemoteAddressInfo m_remoteAddr;
    QByteArray m_datagram;
};



class State : public QObject{
    Q_OBJECT

protected:
    QString m_Name;
    RemoteClientSM &m_sm;

public:
    explicit State(QString name, RemoteClientSM &sm , QObject *parent = 0);
    virtual ~State() {}

    QString Name() const;
    virtual State *handleState() = 0;
};

inline QString State::Name() const{
    return m_Name;
}

class Init : public State{
public:
    explicit Init(QString name, RemoteClientSM &sm, QObject *parent = 0);
    virtual ~Init(){}

    virtual State *handleState() override;
};

class IQSHF : public State{
public:
    explicit IQSHF(QString name, RemoteClientSM &sm, QObject *parent = 0);
    virtual ~IQSHF(){}

    virtual State *handleState() override;
};

class IQPQSHF : public State{
public:
    explicit IQPQSHF(QString name, RemoteClientSM &sm, QObject *parent = 0);
    virtual ~IQPQSHF(){}

    virtual State *handleState() override;
};


class RemoteClientSM : public QObject{
    Q_OBJECT

    friend class Init;
    friend class IQSHF;
    friend class IQPQSHF;

public:
    enum eSettings{
        eStdWaitTime     = 10,  // ms
        eBoaringTime     = 100, // ms used if there is nothing to do
        eWriteWaitTime   = 150, // ms
        eTooManyWaitTime = 1000 // ms
    };

    struct tmrCtrl{
        tmrCtrl(): m_wasThis(false), m_remaining(-1){}
        bool m_wasThis;
        int m_remaining;
    };

    RemoteClientSM(Device &dev, QObject*parent=NULL);
    ~RemoteClientSM();

    /**
     * @brief control functions for the state machine
     */
    void abort();
    void allowWork(bool val);


    void Run(bool &abort);

    /**
     * @brief setThreadWaitTime defines the next call to start doWork
     * @param ms
     */
    void setThreadWaitTime(int ms);

    /**
     * @brief getThreadWaitTime returns the actual time to next start
     * @return
     */
    int getThreadWaitTime();

    QTimer * getTaskTimer();

    /**
     * @brief checkMaxPollingReqeustsReached checks if the max number of polling requests are reached. In the middleware, we can only have an fixed amount
     * of parallel requests, at half of this value, we don't allow further polling requests (for performance reasons)
     * @param r request to check
     * @return Errors::Id::NO_ERROR if not reached, Errors::Id::MANY_REQUESTS if limit is reached
     */
    Errors::Id checkMaxPollingReqeustsReached(Request *r);

public slots:

    /**
     * @brief doWork worker slot of m_worker
     */
    void doWork();

    /**
     * @brief stopWork suspend the thread
     */
    void stopWork();

    /**
     * @brief handleRx a response was received
     * @param ri
     * @param datagram
     */
    void onHandleRx(RemoteAddressInfo ra, QByteArray datagram);

    // queue handling to thread via signals
    void onEnqueueInitQueue(Request *r);
    void onEnqueuePollingQueue(Request *r);
    void onEnqueueCommandQueue(Request *r);
    void onEnqueueSequenceQueue(Request *r);
    void onEnqueueRetryQueue(Request *r);
    void onDequeueRunningRequest(int id);
    void onRemoveParaFromQueue(Request::eQueueType type, uint32_t id);

private:
    Device& m_device;
    State *m_statePtr = nullptr;
    Init  m_Init;
    IQSHF m_handleIQ;
    IQPQSHF m_handleIQPQ;
    int m_threadWaitTime;   /**< time, each worker. doWork call should wait until the next one */
    QTimer *m_taskTimer;
    bool m_abort;
    tmrCtrl m_wasRx;
    tmrCtrl m_wasCommand;
    QQueue<rxData> m_rxData;
    QQueue<Request *> m_initQueue;              /**< init queue, these you have to read first */
    QQueue<Request *> m_pollingQueue;           /**< queue containing the parameter, needed to pool cyclic */
    QQueue<Request *> m_commandQueue;           /**< if a user interaction occours, the requests are stored in the queue */
    QQueue<Request *> m_sequenceQueue;          /**< used by sequences, highes prio */
    QQueue<Request *> m_retryQueue;             /**< used for retries */
    QQueue<int> m_runningRequestQueue;    /**< request is stored until an response is seen */
};


inline void RemoteClientSM::setThreadWaitTime(int ms){
    m_threadWaitTime = ms;
}

inline QTimer *RemoteClientSM::getTaskTimer(){
    return m_taskTimer;
}

inline void RemoteClientSM::onEnqueueInitQueue(Request *r){
    r->setQueueType(Request::eQueueType::eInit);
    m_initQueue.enqueue(r);
}

inline void RemoteClientSM::onEnqueuePollingQueue(Request *r){
    r->setQueueType(Request::eQueueType::ePolling);
    m_pollingQueue.enqueue(r);
}

inline void RemoteClientSM::onEnqueueRetryQueue(Request *r){
    m_retryQueue.enqueue(r);
}

inline void RemoteClientSM::abort(){
    m_abort = true;
}

inline void RemoteClientSM::allowWork(bool val){
    m_abort = !val;
}

}

#endif // REMOTECLIENTSM_H
