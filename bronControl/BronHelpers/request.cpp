#include "request.h"
#include "device.h"
#include "devicepara.h"
#include "studiomgr.h"
#include "networkapi_impl.h"

using namespace Framework;

Request::Request(eTypes t, std::weak_ptr<DevicePara> p, QList<std::function<bool()> >post)
    : QObject()
    , m_p(p)
    , m_type(t)
    , m_retries(0)
    , m_qType(eUnknown)
    , m_nSequence(0L)
    , m_Id(-1)
    , m_timeoutTimer(nullptr)
    , m_postSendActions(post){
    if(auto para = p.lock()){
        m_Id = para->Id();
    }
}

Request::~Request(){
    if(m_timeoutTimer){
        emit stopAndDelete();
        m_timeoutTimer = nullptr;
    }
}

void Request::onTimeout(){
    if(auto p = m_p.lock()){
        p->GetElder().outstandingRequestTimeout(this);
    }
}
