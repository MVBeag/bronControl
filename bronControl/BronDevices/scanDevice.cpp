#include "scanDevice.h"
#include "paraSelectables.h"
#include "bronstudio.h"

using namespace Framework;


void Framework::ScanDevice::initParas(){
    // we use studio address, so we can filter out the wanted studio in enterprize mode
    m_remoteParas.insert(STUDIO_ADDRESS,  std::make_shared<DevicePara>(this, (STUDIO_ADDRESS), "",DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
}


void Framework::ScanDevice::makeConnections(){
    connect(m_rescanTimer, SIGNAL(timeout()), this, SLOT(rescanTimeout()));
    connect(&m_selfDeleteTimer, SIGNAL(timeout()), this, SLOT(selfDeletionTimeot()));
}



Framework::ScanDevice::ScanDevice()
    : Device(false)
    , m_isInitialized(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this))
//    , m_pDefaultPara(&m_defaultPara)
    , m_selfDeleteTimer()
    , m_networkEntry()
    , m_respondingDevices()
    , m_fir()
    , m_studio(nullptr){
    initParas();
    m_rescanTimer->setInterval(eRescanTime);
    m_selfDeleteTimer.setInterval(eSelfDeletionTime);
    makeConnections();
    //m_rescanTimer.start();
    emit startRescanTimer();
//    m_selfDeleteTimer.start();
    setPolling(false);
    setRetries(eRetries);
    setScanDevice(true);
//    qDebug() << "construct scan device: " << getHiddenId();
}

Framework::ScanDevice::~ScanDevice(){
//    qDebug()  << "destroy scan device: " << getHiddenId();
    m_remoteParas.clear();
}

QList<std::shared_ptr<DevicePara> > Framework::ScanDevice::GetAllParas() const{
    return m_remoteParas.values();
}

QList<std::shared_ptr<DevicePara> > Framework::ScanDevice::GetPollParas()const{
    QList<std::shared_ptr<DevicePara> > poll;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::ePoll)){
            poll.append(p);
        }
    }
    return poll;
}

QString Framework::ScanDevice::getName() const{
    return "ScanDevice";
}


bool Framework::ScanDevice::isForStudio(BronStudio *studio){
    if(studio && m_studio){
        if(m_studio->getAddress() == studio->getAddress()){
            return true;
        }
        else {
            return false;
        }
    }
    // is studio is nullptr, always true
    return true;
}

const QString Framework::ScanDevice::IdString(int id) const{
    const QMetaEnum &me = QMetaEnum::fromType<ScanDevice::Ids>();
    const char *key = me.valueToKey(static_cast<ScanDevice::Ids>(id));
    if(key){
        return QString(key);
    }
    else{
        return QString("ScanDevice unknown Id: " + QString::number(id));
    }
}


void Framework::ScanDevice::rescanTimeout(){
    getPara(STUDIO_ADDRESS)->getRemote();
}


QList<std::shared_ptr<DevicePara> > Framework::ScanDevice::GetInitParas()const{
    QList<std::shared_ptr<DevicePara> > init;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::eInit)){
            init.append(p);
        }
    }
    return init;
}
