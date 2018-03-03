#include "brongroup.h"
#include "device.h"
#include "devicepara.h"
#include <cfloat>
#include <limits>

using namespace Framework;




Framework::BronGroup::BronGroup(QObject *parent)
    : QObject(parent)
    , m_devices()
    , m_cogniColor(ParaSelects::CognitionLightColor::NONE){
}

Framework::BronGroup::BronGroup(const BronGroup &g)
    : QObject(g.parent())
    , m_devices(g.m_devices)
    , m_cogniColor(g.m_cogniColor)
    , m_address(g.m_address){
}

Framework::BronGroup::~BronGroup(){
    m_devices.clear();
}

bool Framework::BronGroup::setIncEnergy(float increment){
    double maxCurPower = getMaxCurGroupPower();
    double maxPower    = getMaxGroupPower();

    if (maxCurPower < maxPower) {
        if (maxPower - maxCurPower < increment) {
            increment = maxPower - maxCurPower;
        }

        auto lst = getDevices();
        auto iter = lst.constBegin();
        auto end = lst.constEnd();
        for (; iter != end; ++iter) {
            if(auto d = (*iter).lock()){
                if(!d->isAvailable()) continue;
                d->increasePower(increment);
            }
        }
        return true;
    }
    return false;
}

bool Framework::BronGroup::setDecEnergy(float decrement){
    float curPower = getCurGroupPower();
    float minPower = getMinGroupPower();

    if (curPower > minPower) {
        if (curPower - minPower < decrement) {
            decrement = curPower - minPower;
        }

        auto lst = getDevices();
        auto iter = lst.constBegin();
        auto end = lst.constEnd();
        for (; iter != end; ++iter) {
            if(auto d = (*iter).lock()){
                if(!d->isAvailable()) continue;
                d->decreasePower(decrement);
            }
        }
        return true;
    }
    return false;
}

float Framework::BronGroup::getMinCurGroupPower(){
    float minCurPower = 0.0f;

    auto iter = m_devices.constBegin();
    auto end = m_devices.constEnd();
    for (; iter != end; ++iter) {
        if(auto d = (*iter).lock()){
            if(!d->isAvailable()) continue;
            float val = d->getApertureAct();
            if((val < minCurPower) || minCurPower == 0.0f){
                minCurPower = val;
            }
        }
    }
    return minCurPower;
}

float Framework::BronGroup::getMaxCurGroupPower(){
    float maxCurPower = 0.0f;
    auto iter = m_devices.constBegin();
    auto end = m_devices.constEnd();
    for (; iter != end; ++iter) {
        if(auto d = (*iter).lock()){
            if(!d->isAvailable()) continue;
            float val = d->getApertureAct();
            if(val > maxCurPower){
                maxCurPower = val;
            }
        }
    }
    return maxCurPower;
}

float Framework::BronGroup::getMinGroupPower() {
        float minPowerMax = 0.0f;
        auto iter = m_devices.constBegin();
        auto end = m_devices.constEnd();
        for (; iter != end; ++iter) {
            if(auto d = (*iter).lock()){
                if(!d->isAvailable()) continue;
                float val = d->getApertureMin();
                if (val > minPowerMax) {
                    minPowerMax = val;
                }
            }
        }
        return minPowerMax;
}

float Framework::BronGroup::getMaxGroupPower(){
    float maxPower = std::numeric_limits<float>::max();
    auto iter = m_devices.constBegin();
    auto end = m_devices.constEnd();
    for (; iter != end; ++iter) {
        if(auto d = (*iter).lock()){
            if(!d->isAvailable()) continue;
            float val = d->getApertureMax();
            if (val < maxPower) {
                maxPower = val;
            }
        }
    }
    return maxPower;
}

bool Framework::BronGroup::setStandby(bool standby){
    QListIterator<std::weak_ptr<Device> > iter(m_devices);
    while(iter.hasNext()){
        if(auto d = iter.next().lock()){
            d->setStandby(standby);
        }
    }
    return true;
}

int Framework::BronGroup::getStandby() const{
    int ret = 0;
    bool isFirst = true;
    QListIterator<std::weak_ptr<Device> > iter(m_devices);
    while(iter.hasNext()){
        if(auto d = iter.next().lock()){
            if(isFirst){
                ret = d->getStandby();
                isFirst = false;
            }
            else{
                int val = d->getStandby();
                if(val != ret){
                    ret = 2;
                    break;
                }
            }
        }
    }
    return ret;
}

//bool Framework::BronGroup::setShowCogni(){
//    QListIterator<std::weak_ptr<Device> > iter(m_devices);
//    while(iter.hasNext()) {
//        if(auto dev = iter.next().lock()){
//            if(!dev->isAvailable()) continue;
//            dev->setShowCogni();
//        }
//    }
//    return true;
//}

bool Framework::BronGroup::wink(){
    QListIterator<std::weak_ptr<Device> > iter(m_devices);
    while(iter.hasNext()) {
        if(auto dev = iter.next().lock()){
            if(!dev->isAvailable()) continue;
            dev->startWink();
        }
    }
    return true;
}


bool Framework::BronGroup::setCogniEnabled(bool val){
    QListIterator<std::weak_ptr<Device> > iter(m_devices);
    while(iter.hasNext()) {
        if(auto dev = iter.next().lock()){
            if(!dev->isAvailable()) continue;
            dev->setCogniEnable(val);
        }
    }
    return true;
}

int BronGroup::getCogniEnabled() const{
    int ret = 0;
    QListIterator<std::weak_ptr<Device> > iter(m_devices);
    while(iter.hasNext()) {
        if(auto dev = iter.next().lock()){
            if(!dev->isAvailable()) continue;
            int tmp = dev->getCogniEnable();
            if(-1 == tmp) continue;
            ret = tmp;
        }
    }
    return ret;
}

bool Framework::BronGroup::removeDevice(std::weak_ptr<Device> dev){
    QMutableListIterator<std::weak_ptr<Device> > i(m_devices);
    while (i.hasNext()) {
        auto di = i.next().lock();
        auto d = dev.lock();
        if(di && d){
            if(di->getHiddenId() == d->getHiddenId()){
                i.remove();
                emit deviceRemoved(dev);
                return true;
            }
        }
    }


//    if(m_devices.contains(dev)){
//        if(m_devices.remove(dev)){
//            emit deviceRemoved(dev);
//            return true;
//        }
//    }
    return false;
}

void Framework::BronGroup::addDevice(std::weak_ptr<Device> dev){
    m_devices.append(dev);
    emit deviceAdded(dev);
}
