#include "demodevicepara.h"
#include "device.h"

Framework::DemoDevicePara::DemoDevicePara(Device *parent, int id, const QVariant value, Types type, float scale, DevicePara::Settings s, preReadFct fct, depFunction dfct)
    : DevicePara(parent, id, value, type, scale, s, fct, dfct){
}

bool Framework::DemoDevicePara::setRemote(const QVariant value, QList<std::function<bool()> > postCmd){
    Q_UNUSED(postCmd)
    m_elder.storePara(this);
    setDataForRemote(triggerReason::eNewTarget, value);
    if(Type() == DevicePara::Types::eInt){
        return setData(QVariant(value.toFloat() * m_scaling));
    }else if(Type() == DevicePara::Types::eString){
        return setData(value);
    }
    return false;
}

bool Framework::DemoDevicePara::setRemote(){
    setDataForRemote();
    return true;
}

void Framework::DemoDevicePara::getRemote(){
    setDataForRemote();
}
