#ifndef REQUEST_H
#define REQUEST_H


#include <memory>
#include <functional>
#include <QObject>
#include <QTimer>
#include <QList>
#include "mwtypes.h"


namespace Framework{

class DevicePara;
class Device;

/**
 * @brief The Request class base class for Request Types, they are used to wrap
 * function callbacks into the middleware framework. In addition to this, the
 * retries are handled in the request types. Du to the fact, that we have more than
 * one request per device (up to 10) it is the easiest to provice a "filter" callback
 * deciding to retry or calling the original callback.
 */
class Request : public QObject{
    Q_OBJECT
public:
    /**
     * @brief The eType enum for several request types supported by bron devcies
     */


    enum eTypes{
        eSetInt,
        eGetInt,
        eSetString,
        eGetString
    };

    enum eQueueType{
        eUnknown,
        eInit,
        ePolling,
        eCommand,
        eSequence,
        eRetry
    };

    Request(eTypes t, std::weak_ptr<DevicePara> p, QList<std::function<bool()> > post = QList<std::function<bool()> >());

    virtual ~Request();

    std::weak_ptr<DevicePara> getPara()const;
    eTypes Type()const;
    int getRetries() const;
    void setRetries(int retries);
    void incRetries();
    eQueueType getQueueType() const;
    void setQueueType(eQueueType t);
    void release();

    uint32_t getId() const;

    QList<std::function<bool()> > getPostSendActions() const;

public slots:
    void onTimeout();


signals:
    void stopAndDelete();

private:
    std::weak_ptr<DevicePara> m_p;    /**< parameter, forcing the request */
    eTypes m_type;      /**< type of the request, for all integer and all string types there is only one callback per type */
    int m_retries;      /**< retries counter */
    eQueueType m_qType; /**< used to know in which queue the request is, pollingqueue for polling queue, no
                          retries are initated! */

    uint32_t m_nSequence;
    uint32_t m_Id;
    QTimer * m_timeoutTimer;
    QList<std::function<bool()> >m_postSendActions;     /**< sometimes, we have to make shure that the send is done before other things! */

    friend class Framework::Device;
};

inline std::weak_ptr<DevicePara> Request::getPara() const{
    return m_p;
}

inline Request::eTypes Request::Type() const{
    return m_type;
}

inline int Request::getRetries() const{
    return m_retries;
}

inline void Request::setRetries(int retries){
    m_retries = retries;
}

inline void Request::incRetries(){
    m_retries++;
}

inline Request::eQueueType Request::getQueueType() const{
    return m_qType;
}

inline void Request::setQueueType(Request::eQueueType t){
    m_qType = t;
}

inline void Request::release(){
    deleteLater();
}

inline uint32_t Request::getId() const {
    return m_Id;
}

inline QList<std::function<bool ()> > Request::getPostSendActions() const{
    return m_postSendActions;
}




struct Releaser{
    void operator()(Request *r){
        r->release();
    }

};

}

Q_DECLARE_METATYPE(Framework::Request *)


#endif // REQUEST_H
