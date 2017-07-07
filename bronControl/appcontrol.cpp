#include "appcontrol.h"
#include "studiomgr.h"
#include "NetWorkScanner.h"




AppControl *AppControl::getInstance(QObject *parent){
    static AppControl *a = new AppControl(parent);
    return a;
}


bool AppControl::eventFilter(QObject *obj, QEvent *event){
    if(event->type() == QEvent::MouseButtonPress){
        emit showCogni();
    }
    return QObject::eventFilter(obj, event);
}

AppControl::AppControl(QObject *parent)
    : QObject(parent){
}


