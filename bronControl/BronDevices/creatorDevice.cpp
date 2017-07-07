#include "creatorDevice.h"
#include "paraSelectables.h"


using namespace Framework;

void Framework::CreatorDevice::initParas(){
    m_remoteParas.insert(DEVICE_DESC,             std::make_shared<DevicePara>(this, (DEVICE_DESC),0,DevicePara::Types::eInt,1.0f, DevicePara::eInit));
    m_remoteParas.insert(DEVICE_NAME,             std::make_shared<DevicePara>(this, (DEVICE_NAME), "",DevicePara::Types::eString,1.0f, DevicePara::eInit));
    m_remoteParas.insert(STUDIO_ADDRESS,          std::make_shared<DevicePara>(this, (STUDIO_ADDRESS), 0, DevicePara::Types::eInt,1.0f, DevicePara::eInit));
    m_remoteParas.insert(MAX_ENERGY,              std::make_shared<DevicePara>(this, (MAX_ENERGY), 0,DevicePara::Types::eInt,1.0f, DevicePara::eInit));
    m_remoteParas.insert(F_STOP_RANGE,            std::make_shared<DevicePara>(this, (F_STOP_RANGE), 0, DevicePara::Types::eInt,10.0f, DevicePara::eInit));
    m_remoteParas.insert(SSID_COMPACT,            std::make_shared<DevicePara>(this, (SSID_COMPACT), "",DevicePara::Types::eString,1.0f, DevicePara::eInit));
    m_remoteParas.insert(SSID_GENI,               std::make_shared<DevicePara>(this, (SSID_GENI), "",DevicePara::Types::eString,1.0f, DevicePara::eInit));
    m_remoteParas.insert(PWD_COMPACT,             std::make_shared<DevicePara>(this, (PWD_COMPACT), "",DevicePara::Types::eString,1.0f, DevicePara::eInit));
    m_remoteParas.insert(PWD_GENI,                std::make_shared<DevicePara>(this, (PWD_GENI), "",DevicePara::Types::eString,1.0f, DevicePara::eInit));
    m_remoteParas.insert(ENCR_COMPACT,            std::make_shared<DevicePara>(this, (ENCR_COMPACT), 0,DevicePara::Types::eInt,1.0f, DevicePara::eInit));
}

void Framework::CreatorDevice::makeConnections(){
}

Framework::CreatorDevice::CreatorDevice(uint interfaceIx)
    : Device(false)
    , m_isInitialized(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
//    , m_pDefaultPara(&m_defaultPara){
    setInterfaceIx(interfaceIx);
    initParas();
    makeConnections();
    setPolling(false);
}

Framework::CreatorDevice::~CreatorDevice(){
     m_remoteParas.clear();
}

QList<std::shared_ptr<DevicePara> > Framework::CreatorDevice::GetAllParas() const{
    return m_remoteParas.values();
}

QList<std::shared_ptr<DevicePara> > Framework::CreatorDevice::GetPollParas()const{
    QList<std::shared_ptr<DevicePara> > poll;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::ePoll)){
            poll.append(p);
        }
    }
    return poll;
}

QString Framework::CreatorDevice::getName() const{
    return getPara(DEVICE_NAME)->data().toString();
}

const QString Framework::CreatorDevice::IdString(int id) const{
    const QMetaEnum &me = QMetaEnum::fromType<CreatorDevice::Ids>();
    const char *key = me.valueToKey(static_cast<CreatorDevice::Ids>(id));
    if(key){
        return QString(key);
    }
    else{
        return QString("CreatorDevice unknown Id: " + QString::number(id));
    }
}

QList<std::shared_ptr<DevicePara> > Framework::CreatorDevice::GetInitParas()const{
    QList<std::shared_ptr<DevicePara> > init;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::eInit)){
            init.append(p);
        }
    }
    return init;
}
