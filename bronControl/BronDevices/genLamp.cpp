#include <QMetaEnum>
#include "genLamp.h"
#include "errors.h"
#include "ethaddress.h"
#include "devicescoro.h"
#include "demodevicepara.h"
#include <math.h>

using namespace Framework;

void Framework::GenLamp::initParas(bool demo){
    if(demo){
        m_remoteParas.insert(L1_ADDR + m_lampNr,     std::make_shared<DemoDevicePara>(this, (L1_ADDR + m_lampNr),1,DevicePara::Types::eInt,1.0f,DevicePara::eInit));
        m_remoteParas.insert(L1_APERTURE + m_lampNr, std::make_shared<DemoDevicePara>(this, (L1_APERTURE + m_lampNr),5.6,DevicePara::Types::eInt,10.0f,DevicePara::eInit));
        m_remoteParas.insert(L1_SW + m_lampNr,       std::make_shared<DemoDevicePara>(this, (L1_SW + m_lampNr),true,DevicePara::Types::eInt,1.0f,DevicePara::eInit));
    }
    else{
        m_remoteParas.insert(L1_ADDR + m_lampNr,     std::make_shared<DevicePara>(this, (L1_ADDR + m_lampNr),1,DevicePara::Types::eInt,1.0f,DevicePara::eInit));
        m_remoteParas.insert(L1_APERTURE + m_lampNr, std::make_shared<DevicePara>(this, (L1_APERTURE + m_lampNr),5.6,DevicePara::Types::eInt,10.0f,DevicePara::eInit));
        m_remoteParas.insert(L1_SW + m_lampNr,       std::make_shared<DevicePara>(this, (L1_SW + m_lampNr),true,DevicePara::Types::eInt,1.0f,DevicePara::eInit));
    }
}




QString Framework::GenLamp::getEthernetAddress() const{
    if(m_scoro)
        return m_scoro->getEthernetAddress();
    return QString();
}

bool Framework::GenLamp::isScoroInStandby() const{
    if(m_scoro){
        return m_scoro->getPara(DeviceScoro::STANDBY_SW)->data().toBool();
    }
    return false;
}

const QString Framework::GenLamp::IdString(int id) const{
    const QMetaEnum &me = QMetaEnum::fromType<GenLamp::Ids>();
    const char *key = me.valueToKey(static_cast<GenLamp::Ids>(id));
    if(key){
        return QString(key);
    }
    else{
        return QString("GenLamp unknown Id: " + QString::number(id));
    }
}

Framework::GenLamp::GenLamp(int lampNr, QPointer<DeviceScoro> elder, bool demo)
    : Device(demo)
    , m_isInit(false)
    , m_lampNr(lampNr)
    , m_scoro(elder)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
    // set the correct device for com
    if(m_scoro){
        setEthernetAddress(m_scoro->getEthernetAddress());
        setHostAddress(m_scoro->getHostAddress(), m_scoro->getHostPort());
        switch(m_lampNr){
        case 0: m_name = "L1 ("+m_scoro->getName()+")"; break;
        case 1: m_name = "L2 ("+m_scoro->getName()+")"; break;
        case 2: m_name = "L3 ("+m_scoro->getName()+")"; break;
        default: m_name = "L?";
        }
    }
    else{
        switch(m_lampNr){
        case 0: m_name = "L1"; break;
        case 1: m_name = "L2"; break;
        case 2: m_name = "L3"; break;
        default: m_name = "L?";
        }

    }
    initParas(demo);
}

Framework::GenLamp::GenLamp(const GenLamp &l)
    : Device()
    , m_isInit(l.m_isInit)
    , m_lampNr(l.m_lampNr)
    , m_scoro(l.m_scoro)
    , m_remoteParas(l.m_remoteParas)
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
}

Framework::GenLamp::GenLamp()
    : Device()
    , m_isInit(false)
    , m_lampNr(-1)
    , m_scoro(nullptr)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
}


Framework::GenLamp::~GenLamp(){
    if(m_scoro){
        m_scoro->removeLamp(m_lampNr);
    }
    m_remoteParas.clear();
}

float Framework::GenLamp::getFStopRange() const{
    if(m_scoro){
        return m_scoro->getFStopRange();
    }
    return 0.0f;
}

float GenLamp::getApertureMax() const{
    if(m_scoro){
        return m_scoro->getApertureMax();
    }
    return Device::getApertureMax();
}

bool Framework::GenLamp::setName(const QString &name){
    if(m_name != name){
        m_name = name;
    }
    return false;
}

QString Framework::GenLamp::getName() const{
    return m_name;
}

QList<std::shared_ptr<DevicePara> > Framework::GenLamp::GetInitParas() const{
    QList<std::shared_ptr<DevicePara> > init;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::eInit)){
            init.append(p);
        }
    }
    return init;
}

QList<std::shared_ptr<DevicePara> > Framework::GenLamp::GetPollParas() const{
    QList<std::shared_ptr<DevicePara> > poll;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::ePoll)){
            poll.append(p);
        }
    }
    return poll;
}

int Framework::GenLamp::getStandby() const{
//    if(isScoroInStandby()){
//        qDebug() << "genlamp getStanby() scoro in standby() return true";
//        return 1;
//    }
//    else{
//        qDebug() << "genlamp getStanby()  return " << (getPara(L1_SW + m_lampNr)->data().toInt() == 0);

//        return getPara(L1_SW + m_lampNr)->data().toInt() == 0;
//    }
    return getPara(L1_SW + m_lampNr)->data().toInt() == 0;
}

bool Framework::GenLamp::setStandby(int val){
    if(isScoroInStandby()){
        // trigger local parameter update
        return getPara(L1_SW + m_lampNr)->setData(val > 0 ? 0 : 1, Errors::Id::E_NO_ERROR, true);
        // don't set any commands to scoro in standby, otherwise he will wake up
    }
    else{
        if(val == 1){
            return getPara(L1_SW + m_lampNr)->setRemote(0);
        }
        else{
            return getPara(L1_SW + m_lampNr)->setRemote(1);
        }
    }
}

bool Framework::GenLamp::increasePower(float val){
    if(isScoroInStandby()) return false;

    float currEnergy = getApertureAct();
    float increment = val;
    currEnergy = ((currEnergy * 10.0f) + (increment * 10.0f))/ 10.0f;
    if(currEnergy > getApertureMax()){
        currEnergy = getApertureMax();
    }
    setActApertureEnergy(currEnergy);
    if(m_scoro){
        m_scoro->getPara(DeviceScoro::T01_VALUE)->getRemote();
    }
    return true;
}

bool Framework::GenLamp::decreasePower(float val){
    if(isScoroInStandby()) return false;

    float currEnergy = getApertureAct();
    float increment = val;
    currEnergy = ((currEnergy * 10.0f) - (increment * 10.0f))/ 10.0f;
    if(currEnergy < getApertureMin()){
        currEnergy = getApertureMin();
    }

    setActApertureEnergy(currEnergy);
    if(m_scoro){
        m_scoro->getPara(DeviceScoro::T01_VALUE)->getRemote();
    }
    return true;
}

float Framework::GenLamp::getMaxEnergy() const{
    if(m_scoro){
        return m_scoro->getMaxEnergy();
    }
    else{
        return 0.0f;
    }
}

QPointer<DeviceScoro> Framework::GenLamp::getScoroDevice(){
    return m_scoro;
}

void Framework::GenLamp::onApertureChanged(QVariant val){
    getPara(L1_APERTURE + m_lampNr)->setData(val);
}
