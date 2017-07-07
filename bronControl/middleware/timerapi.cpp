#include "timerapi.h"
#include "mwtypes.h"
#include "request.h"

using namespace Framework;

TimerAPI::TimerAPI(Request *ct, timerCB cb)
    : QObject()
    , m_context(ct)
    , m_cb(cb)
    , m_timer(new QTimer(this)){
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void TimerAPI::onStopAndRelease(){
    m_timer->stop();
    m_timer->deleteLater();
}



