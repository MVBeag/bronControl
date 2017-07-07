#ifndef QTTIMERINTEGRATION_H
#define QTTIMERINTEGRATION_H

#include <QObject>
#include <QTimer>

namespace Framework{
    class Request;
}


using namespace Framework;

class TimerAPI : public QObject{
    Q_OBJECT
    public:

        using timerCB = void(*)(Request *);


        TimerAPI(Request *ct, timerCB cb);
        virtual ~TimerAPI(){}
        void OneShot(uint32_t ms);
        void RepeatedShots(uint32_t ms);


    public slots:
        void timeout(){m_cb(m_context);}
        void onStopAndRelease();

    private:
        Request *m_context;
        timerCB m_cb;
        QTimer *m_timer;
};


inline void TimerAPI::OneShot(uint32_t ms){
    m_timer->setSingleShot(true);
    m_timer->start(ms);
}
inline void TimerAPI::RepeatedShots(uint32_t ms){
    m_timer->setSingleShot(false);
    m_timer->start(ms);
}



#endif // QTTIMERINTEGRATION_H
