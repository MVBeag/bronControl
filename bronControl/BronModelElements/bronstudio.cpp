#include "bronstudio.h"
#include "NetWorkScanner.h"
#include "devicesirros.h"
#include <QRegExp>
#include <QStringList>
#include <cfloat>
#include "deviceFinder.h"
#include "studiomgr.h"

// it seems to be, that in the windows port "interface" is used somewhere,
// there are mystic errors in compiling QNetworkInterface/QNetworkSession
#if defined(Q_OS_WIN) && defined(interface)
#undef interface
#endif



#include <QNetworkConfiguration>
#include <QNetworkInterface>
#include "commandsequence.h"

using namespace Framework;


Framework::BronStudio::BronStudio(bool demo, QObject *parent)
    : QObject(parent)
    , m_address(0)
    , m_isOnline(false)
    , m_isPollingAllowed(true) // only for debug cases JR
    , m_isDeviceAwareness(false)
    , m_isDemo(demo)
    , m_runBackgroundUpdate(true)
    , m_name("")
    , m_ssid(m_name)
    , m_password("bronControl")
    , m_encr(static_cast<int>(ParaSelects::EncryptionMode::WPA2))
    , m_specialMode(eNone)
    , m_deviceList()
    , m_groupList()
    , m_sequences()
    , m_myNetworkConfig()
    , m_mySession(nullptr)
    , m_sequenceNr(0L)
    , m_interfaceIx(0xffff){}

Framework::BronStudio::BronStudio(const QNetworkConfiguration &n, bool demo, QObject *parent)
    : QObject(parent)
    , m_address(0)
    , m_isOnline(false)
    , m_isPollingAllowed(true)
    , m_isDeviceAwareness(false)
    , m_isDemo(demo)
    , m_runBackgroundUpdate(true)
    , m_name(n.name())
    , m_ssid(m_name)
    , m_password("bronControl")
    , m_encr(static_cast<int>(ParaSelects::EncryptionMode::WPA2))
    , m_specialMode(eNone)
    , m_deviceList()
    , m_groupList()
    , m_sequences()
    , m_myNetworkConfig(n)
    , m_mySession(nullptr)
    , m_sequenceNr(0L)
    , m_interfaceIx(0xffff){}

Framework::BronStudio::~BronStudio(){
    m_sequences.clear();
    m_deviceList.clear();
    m_deviceList.clear();
}

bool Framework::BronStudio::migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr){
    // switch devices within a sequence, if in sap mode, switch sap device as last device
    if(auto sequ = startStudioSequence("migrateToNetwork")){
        QListIterator<std::shared_ptr<Device> >iter(m_deviceList);
        while(iter.hasNext()){
            auto d = iter.next();
            if(d->getDeviceType() != Device::eTypes::eGeniLamp){
                QString hostAddr = d->getHostAddress().toString();
                if(hostAddr.endsWith(".1") || hostAddr.endsWith(".01") || hostAddr.endsWith(":01") || hostAddr.endsWith(":1")){
                    if(Ssid().contains("Bron-Studio")){
                        sequ->storeFunction(std::bind(&Device::migrateToNewNetwork, d, enterprize, ssid, pw, encr));
                        continue;
                    }
                }
            }
            sequ->append(d);
            d->migrateToNewNetwork(enterprize, ssid, pw, encr);
        }
        endStudioSequence(sequ);
    }

    return 1;
}



bool Framework::BronStudio::setAddress(const int addr){
    if(addr != m_address){
        m_address = addr;
        emit dataChanged(eData::eAddress);
    }
    return true;
}

void Framework::BronStudio::startDiscovery(){
    StudioMgr::getInstance()->startDiscovery(this);
}

void Framework::BronStudio::stopDiscovery(){
    StudioMgr::getInstance()->stopDiscovery(this);
}

void Framework::BronStudio::switchNetwork(bool online){
    Q_UNUSED(online)
#if 0
    // get access to network scanner
    QExplicitlySharedDataPointer<NetworkScanner> ns = NetworkScanner::getInstance();
    if(true == online){
        // check if m_myNetworkConfig is active
        if(m_myNetworkConfig.state() == QNetworkConfiguration::Active){
            // yeah, we have wifi
            // start resolving process, destroy possible old sessions
            if(m_mySession){
                delete m_mySession;
            }

            m_mySession = new QNetworkSession(ns->defaultConfig());


            connect(m_mySession, static_cast<void(QNetworkSession::*)(QNetworkSession::SessionError)>(&QNetworkSession::error),[=](QNetworkSession::SessionError error){ qDebug() << "Network Session Error: " << error; });
            connect(m_mySession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(onNetworkStateChanged(QNetworkSession::State)));
            connect(m_mySession, SIGNAL(opened()), this, SLOT(startDiscovery()));
            connect(m_mySession, SIGNAL(closed()), this, SLOT(stopDiscovery()));
            if(ns->isAutoConnectPossible()){
                m_mySession->open();
            }
            // start discovery direct
            else{
                startDiscovery();
            }
        }
        else{
            // check if we have an invalid network config (created only with name!), if so, try
            // to get a valid one by name comparing
            bool found = false;
            if(!m_myNetworkConfig.isValid()){
                // try to find a valid network configuration with the given name
                QList<QNetworkConfiguration> namedConfigs = ns->getNetworksByName(this->Name());
                if(namedConfigs.size() > 0){
                    // get the first networkconfiguration with this name
                    QNetworkConfiguration c = namedConfigs.at(0);
                    setNetwork(c);
                    found = true;
                }
            }
            else{
                found = true;
            }
            if(found){
                // get known networks from scanner
                // check if underlying OS allows to start/stop network

                if(ns->isAutoConnectPossible()){
                    //                bool found = false;
                    QListIterator<QNetworkConfiguration> iter(ns->getNetworks());
                    while(iter.hasNext()){
                        QNetworkConfiguration cfg = iter.next();
                        if(cfg.state() == QNetworkConfiguration::Active){
                            // active network
                            if(cfg.bearerType() == QNetworkConfiguration::BearerWLAN){
                                if(m_myNetworkConfig != cfg){
                                    break;
                                }
                            }
                        }
                    }

                    if(m_mySession == nullptr){
                        m_mySession = new QNetworkSession(m_myNetworkConfig, this);
                    }
                    else{
                        delete m_mySession;
                        m_mySession = new QNetworkSession(m_myNetworkConfig, this);
                    }
                    QNetworkSession(m_myNetworkConfig).open();

                    connect(m_mySession, static_cast<void(QNetworkSession::*)(QNetworkSession::SessionError)>(&QNetworkSession::error),
                            this, &BronStudio::onNetworkError, Qt::QueuedConnection);
                    connect(m_mySession, SIGNAL(stateChanged(QNetworkSession::State)), this, SLOT(onNetworkStateChanged(QNetworkSession::State)));
                    connect(m_mySession, SIGNAL(opened()), this, SLOT(startDiscovery()));
                    connect(m_mySession, SIGNAL(closed()), this, SLOT(stopDiscovery()));
                    m_mySession->open();
                }
                else{
                    startDiscovery();
                }
            }
            else{
                emit error("no valid network found");
            }
        }
    }
    else{
        if(ns->isAutoConnectPossible()){
            if(m_mySession){
                m_mySession->close();
            }
        }
    }
#endif
}


void BronStudio::setOnline(bool val){
    if(val == m_isOnline){
        return;
    }
    m_isOnline = val;
    emit netConfigChanged(); // online symbol
    if(m_isDemo){
        return;
    }

    // switch to studio network
//    switchNetwork(m_isOnline);

    if(false == m_isOnline){
        if(getBackgroundUpdate()){
            return;
        }
    }

    // switch the underelying devices to online
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()){
        iter.next()->setOnline(m_isOnline);
    }
}

void Framework::BronStudio::onInitParasRead(std::weak_ptr<Device>d){
    if(auto dev = d.lock()){
        dev->startUpdateService();
        emit newDeviceUp(d);
    }
}

void Framework::BronStudio::onDeviceLost(std::weak_ptr<Device> d){
    if(auto dev = d.lock()){
        if(removeDevice(dev)){
            dev->Shutdown();
            disconnect(dev.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), this, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));
            disconnect(dev.get(), SIGNAL(deviceLost(std::weak_ptr<Device> )), this, SLOT(onDeviceLost(std::weak_ptr<Device> )));
            disconnect(dev.get(), SIGNAL(error(QString)), this, SIGNAL(error(QString)));
        }
    }
}

bool Framework::BronStudio::removeDeviceFromGroup(std::weak_ptr<Device> dev){
    if(auto d = dev.lock()){
        if(-1 != d->getLampAddress()){
            QMutableListIterator<BronGroup *> iter(m_groupList);
            while(iter.hasNext()){
                BronGroup *bg = iter.next();
                if(bg->removeDevice(d)){
                    if(bg->devicesCount() == 0){
                        iter.remove();
                        emit bronStudioGroupRemoved(bg);
                        bg->deleteLater();
                    }
                    return true;
                }
            }
        }
        else{
            return true;
        }
    }
    return false;
}


bool Framework::BronStudio::addDeviceToGroup(std::weak_ptr<Device> d){
//    DevicePara *para = d->Para(Paras::Id::LAMP_ADDRESS);
    if(auto dev = d.lock()){
        int addr = dev->getLampAddress();//para->data().toInt();
        if(-1 != addr){             // in case of scoro
            QListIterator<BronGroup *> iter(m_groupList);
            while(iter.hasNext()){
                BronGroup *bg = iter.next();
                if(bg->getAddress() == addr){
                    bg->addDevice(dev);
                    return true;
                }
            }
            BronGroup *bg = new BronGroup(this);
            bg->addDevice(dev);
            bg->setAddress(addr);
            m_groupList.append(bg);
            emit bronStudioGroupAdded(bg);
            return true;
        }
        return true;   // indicate true even if not assigned to group
    }
    return false;
}

uint Framework::BronStudio::getInterfaceIx() const{
    return m_interfaceIx;
}

void Framework::BronStudio::setInterfaceIx(const uint &interfaceIx){
    m_interfaceIx = interfaceIx;
}

void Framework::BronStudio::freeze(){
    QListIterator<std::shared_ptr<Device> > iter(m_deviceList);
    while (iter.hasNext()) {
        auto d = iter.next();
        d->freeze();
    }
}

void Framework::BronStudio::unfreeze(){
    QListIterator<std::shared_ptr<Device> > iter(m_deviceList);
    while (iter.hasNext()) {
        auto d = iter.next();
        d->unfreeze();
    }
}


void Framework::BronStudio::onDeviceDataChanged(std::weak_ptr<DevicePara> p, std::weak_ptr<Device> d){
    if(auto dev = d.lock()){
        if(auto dp = p.lock()){
            if(dp->Id() == dev->iDLampAddress()){
                // handle groups
                removeDeviceFromGroup(d);
                addDeviceToGroup(d);
            }
            // possible in enterprise mode, studios is from network view only an parameter
            else if(dp->Id() == dev->iDStudioAddress()){
                removeDeviceFromGroup(d);
                dev->Shutdown();
                if(m_deviceList.removeOne(dev)){
                    emit bronStudioGoneDevice(dev);
                }
            }
        }
    }
}

float Framework::BronStudio::getMinCurStudioPower() const{
    float minCurPower = 0.0f;
    QListIterator<std::shared_ptr<Device> > iter(m_deviceList);
    while (iter.hasNext()) {
        auto d = iter.next();
        if(!d->isAvailable()) continue;
        float val = d->getApertureAct();
        if((val < minCurPower) || minCurPower == 0.0f){
            minCurPower = val;
        }
    }
    return minCurPower;
}

float Framework::BronStudio::getMaxCurStudioPower() const{

    float maxCurPower = 0.0f;
    QListIterator<std::shared_ptr<Device> >iter(m_deviceList);
    while (iter.hasNext()) {
        auto d = iter.next();
        if(!d->isAvailable()) continue;
        float val = d->getApertureAct();
        if(val > maxCurPower){
            maxCurPower = val;
        }
    }
    return maxCurPower;

}

float Framework::BronStudio::getMinStudioPower() const {
    float smallestSpan = 10.0f;
    std::shared_ptr<Device> ps(nullptr);

    // find the device with the smallest span
    QListIterator<std::shared_ptr<Device> >iter(m_deviceList);
    while (iter.hasNext()) {
        auto d = iter.next();
        if(!d->isAvailable()) continue;
        float act = d->getApertureAct();
        float min = d->getApertureMin();
        // security check
        if(act > min){
            // this device can move diff
            float actspan = act - min;
            if(actspan < smallestSpan){
                smallestSpan = actspan;
                ps = d;
            }
        }
        else{
            smallestSpan = 0.0f;
            ps = d;
        }
    }
    float minCurStudioPower = getMinCurStudioPower();
    float minValue = 0.0f;
    if(ps){
        if(smallestSpan < 0.01f){
            smallestSpan = 0.0f;
        }
        minValue = minCurStudioPower - smallestSpan;
    }
    return minValue;
}

float Framework::BronStudio::getMaxStudioPower() const{
    float smallestSpan = 10.0f;
    std::shared_ptr<Device> ps(nullptr);

    // find device with smalles span
    QListIterator<std::shared_ptr<Device> >iter(m_deviceList);
    while (iter.hasNext()) {
        auto d = iter.next();
        if(!d->isAvailable()) continue;
        float act = d->getApertureAct();
        float max = d->getApertureMax();
        // security check
        if(act < max){
            float actspan = max - act;
            if(actspan < smallestSpan){
                smallestSpan = actspan;
                ps = d;
            }
        }
        else{
            smallestSpan = 0.0f;
            ps = d;
        }
    }

    float maxCurStudioPower = getMaxCurStudioPower();
    float maxValue = 10.0f;
    if(ps){
        if(smallestSpan < 0.01f){
            smallestSpan = 0.0f;
        }
        maxValue = maxCurStudioPower + smallestSpan;
    }
    return maxValue;
}


void DefaultErrorFunction(BronStudio &d, QVariant){
    emit d.serviceError("BronStudio", "unknown Service");
}

void Framework::BronStudio::setPolling(bool val){
    if(m_isPollingAllowed == val) return;

    m_isPollingAllowed = val;
    QListIterator<std::shared_ptr<Device> > iter(m_deviceList);
    while(iter.hasNext()){
        iter.next()->setPolling(val);
    }
}

void Framework::BronStudio::setDeviceAwareness(bool val){
    if(m_isDeviceAwareness == val) return;

    m_isDeviceAwareness = val;
    QListIterator<std::shared_ptr<Device> > iter(m_deviceList);
    while(iter.hasNext()){
        iter.next()->setIsDeviceAwareness(val);
    }
}

bool Framework::BronStudio::setIncPower(float increment){
    float maxCurPower = getMaxCurStudioPower();
    float maxPower    = getMaxStudioPower();


    if (maxCurPower < maxPower) {
        if (maxPower - maxCurPower < increment) {
            increment = maxPower - maxCurPower;
        }

        QListIterator<std::shared_ptr<Device> > iter(getDevices());
        while(iter.hasNext()) {
            auto dev = iter.next();
            // filter geniLamps out, otherwise the customer get confused, one beep for Score, and one beep for each lamp
            if(dev->getDeviceType() != Device::eTypes::eGeniLamp){
               if(!dev->isAvailable()) continue;
               dev->increasePower(increment);
            }
        }
        return true;
    }
    return false;
}

bool Framework::BronStudio::setDecPower(double decrement){
    double curPower = getCurStudioPower();
    double minPower = getMinStudioPower();

    if (curPower > minPower) {
        if (curPower - minPower < decrement) {
            decrement = curPower - minPower;
        }

        QListIterator<std::shared_ptr<Device> >iter(getDevices());
        while(iter.hasNext()) {
            auto dev = iter.next();
            // filter geniLamps out, otherwise the customer get confused, one beep for Score, and one beep for each lamp
            if(dev->getDeviceType() != Device::eTypes::eGeniLamp){
                if(!dev->isAvailable()) continue;
                dev->decreasePower(decrement);
            }
        }
        return true;
    }

    return false;
}

bool Framework::BronStudio::setSwitchModlight(int val){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(dev->getModLight() != -1){
            if(!dev->isAvailable()) continue;
            dev->setModLight(val);
            ret = true;
        }
    }
    return ret;
}

int Framework::BronStudio::getSwitchModlight() {
    bool isfirst = true;
    int act = 0;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(dev->getModLight() != -1){
            if(isfirst){
                act = dev->getModLight();
                isfirst = false;
            }
            else{
                if(act != dev->getModLight()){
                    return 2;
                }
            }
        }
    }

    return act;
}

bool Framework::BronStudio::setStandby(int val){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        // filter geniLamps out, scoro is enough otherwise, if the command for setting standby
        // of a lamp arrives after the standby of the scoro, the scoro is wake up!!
        if(dev->getDeviceType() != Device::eTypes::eGeniLamp){
            dev->setStandby(val);
            ret = true;
        }
    }
    return ret;
}

int Framework::BronStudio::getStandby(){
    bool isfirst = true;
    int act = 0;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(isfirst){
            act = dev->getStandby();
            isfirst = false;
        }
        else{
            if(act != dev->getStandby()){
                return 2;
            }
        }
    }

    return act;
}

bool Framework::BronStudio::setSwitchSpeed(int val){
    bool ret = false;
    if(val == 0 || val == 1){
        QListIterator<std::shared_ptr<Device> > iter(getDevices());
        while(iter.hasNext()) {
            auto dev = iter.next();
            if(!dev->isAvailable()) continue;
            dev->setSpeed(static_cast<ParaSelects::SpeedMode>(val));
            ret = true;
        }
    }
    return ret;
}

int Framework::BronStudio::getSwitchSpeed(){
    bool isfirst = true;
    ParaSelects::SpeedMode act = ParaSelects::SpeedMode::Normal;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        if(dev->getSpeed() != ParaSelects::SpeedMode::notSupported){
            if(isfirst){
                act = dev->getSpeed();
                isfirst = false;
            }
            else{
                if(act != dev->getSpeed()){
                    return 2;
                }
            }
        }
    }

    return static_cast<int>(act);
}

bool Framework::BronStudio::setCogniLight(int val){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()){
        auto dev = iter.next();
        if(dev->isAvailable()){
            dev->setCogniColEnable(val);
            ret = true;
        }
    }
    return ret;
}

int Framework::BronStudio::getCogniLight(){
    bool isfirst = true;
    int act = 0;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()){
        auto dev = iter.next();
        if(-1 != dev->getCogniColEnable()){
            if(isfirst){
                act = dev->getCogniColEnable();
                isfirst = false;
            }
            else{
                if(act != dev->getCogniColEnable()){
                    return 2;
                }
            }
        }
    }
    return act;
}

bool Framework::BronStudio::setTest(bool){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        dev->setTest();
        ret = true;
    }
    return ret;
}


bool Framework::BronStudio::setAudio(bool val){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        dev->setAudio(val);
        ret = true;
    }
    return ret;
}

int Framework::BronStudio::getAudio(){
    bool isfirst = true;
    int act = 0;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(-1 != dev->getAudio()){
            if(isfirst){
                if(dev->getDeviceType() == Device::eTypes::eGeni){
                    act = (dev->getAudio() > 0) ? 1 : 0;
                }
                else{
                    act = dev->getAudio();
                }
                isfirst = false;
            }
            else{
                int tmp = 0;
                if(dev->getDeviceType() == Device::eTypes::eGeni){
                    tmp = (dev->getAudio() > 0) ? 1 : 0;
                }
                else{
                    tmp = dev->getAudio();
                }

                if(act != tmp){
                    return 2;
                }
            }
        }
    }
    return act;
}

bool Framework::BronStudio::setApplication(QVariantMap val){

    enum class defReset : uint8_t{
        eNotSet = 0,
        eReset = 1,
        eDefault = 2,
        eApp = 3
    } ctrl(defReset::eNotSet);

    if(val.contains("isReset")){
            ctrl = val.value("isReset").toBool() ? defReset::eReset : defReset::eNotSet;
    }

    if(ctrl == defReset::eReset){
        // reset Application
        // in case of eNone, write unconditional, to get the QML side up to date
        m_specialMode = BronStudio::eSpecialModes::eNone;
        emit dataChanged(eData::eSpecialModes);

        QListIterator<std::shared_ptr<Device> >iter(m_deviceList);
        while(iter.hasNext()){
//            iter.next()->restoreTempParas();
            auto dev = iter.next();
            // sequence relevant values
            QExplicitlySharedDataPointer<CommandSequence> s = dev->startCommandSequence("set application reset");
            dev->setRemoteControl(ParaSelects::RemoteCtrl::RcOff);
            dev->setSynSequence(static_cast<int>(ParaSelects::SequenceModes::DISABLED));
            dev->setSequence(0);
            dev->setPauseTime(0.0f);
            // freemask, alternate relevant values
            dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::DISABLED));
            dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::DISABLED));
            dev->restoreRemoteControl();
            dev->endCommandSequence(s);
        }
    }
    else{
        QString application;
        if(val.contains("application")){
            application = val.value("application").toString();
            if(application == "sequence"){
                if(m_specialMode != BronStudio::eSpecialModes::eSequence){
                    m_specialMode = BronStudio::eSpecialModes::eSequence;
                    emit dataChanged(eData::eSpecialModes);
                }
                disableDumbDevices();
                if(val.contains("deviceValues")){
                    QVariantList list = val.value("deviceValues").toList();
                    QListIterator<QVariant> iter(list);
                    while(iter.hasNext()){
                        QVariant data = iter.next();
                        QMap<QString,QVariant> obj = data.toMap();
                        bool isMaster = obj.value("isMaster").toBool();
                        int seq = obj.value("sequence").toInt();
                        float delay = obj.value("delay").toFloat();
                        std::weak_ptr<Device> d = (obj.value("devicePtr")).value<std::weak_ptr<Device> >();
                        if(auto dev = d.lock()){
//                            ParamLocker lock(*dev);
                            dev->storeRemoteControl();
                            QExplicitlySharedDataPointer<CommandSequence> s = dev->startCommandSequence("set application(sequence)");
                            if(isMaster){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setPauseTime(delay);
                                dev->setSequence(seq);
                                dev->setSynSequence(static_cast<int>(ParaSelects::SequenceModes::MASTER));
                            }
                            else{
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcCell);
                                dev->setSynSequence(static_cast<int>(ParaSelects::SequenceModes::SLAVE));
                                dev->setSequence(0);
                                dev->setPauseTime(0.0f);
                            }
                            dev->endCommandSequence(s);
                        }
                    }
                }
            }
            else if(application == "freemask"){
                if(m_specialMode != BronStudio::eSpecialModes::eFreemask){
                    m_specialMode = BronStudio::eSpecialModes::eFreemask;
                    emit dataChanged(eData::eSpecialModes);
                }
                disableDumbDevices();
                if(val.contains("deviceValues")){
                    QVariantList list = val.value("deviceValues").toList();
                    QListIterator<QVariant> iter(list);
                    while(iter.hasNext()){
                        QVariant data = iter.next();
                        QMap<QString,QVariant> obj = data.toMap();
                        QString group = obj.value("group").toString();
                        std::weak_ptr<Device> d = (obj.value("devicePtr")).value<std::weak_ptr<Device> >();
                        if(auto dev = d.lock()){
//                            ParamLocker lock(*dev);
                            dev->storeRemoteControl();
                            QExplicitlySharedDataPointer<CommandSequence> s = dev->startCommandSequence("set application(freemask)");
                            if(group == ""){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcOff);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::DISABLED));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::DISABLED));
                            }
                            ///////////FREEMASK///////////////////////////////////////////////
                            else if(group == "MAIN"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::ACTIVE));
                            }
                            else if(group == "MASK"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT1));
                            }
                            ///////ALTERNATE //////////////////////////////////////////////////
                            else if(group == "ACTIVE"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::ACTIVE));
                            }
                            else if(group == "WAIT 1"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT1));
                            }
                            else if(group == "WAIT 2"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::ALTERNATE_3_MASKS));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT2));
                            }
                            else if(group == "WAIT 3"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::ALTERNATE_4_MASKS));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT3));
                            }
                            dev->endCommandSequence(s);
                        }
                    }
                }
            }
            else if(application == "alternate"){
                if(m_specialMode != BronStudio::eSpecialModes::eAlternate){
                    m_specialMode = BronStudio::eSpecialModes::eAlternate;
                    emit dataChanged(eData::eSpecialModes);
                }
                disableDumbDevices();
                if(val.contains("deviceValues")){
                    QVariantList list = val.value("deviceValues").toList();
                    QListIterator<QVariant> iter(list);
                    QString ggroup;
                    while(iter.hasNext()){
                        QVariant data = iter.next();
                        QMap<QString,QVariant> obj = data.toMap();
                        QString group = obj.value("group").toString();
                        if(group == "WAIT 3"){
                            ggroup = group;
                        }
                        else if(group == "WAIT 2"){
                            if(ggroup != "WAIT 3"){
                                ggroup = group;
                            }
                        }
                        else if(group == "WAIT 1"){
                            if(ggroup != "WAIT 3" && ggroup != "WAIT 2"){
                                ggroup = group;
                            }
                        }
                        else if(group == "ACTIVE"){
                            if(ggroup != "WAIT 3" && ggroup != "WAIT 2" && ggroup != "WAIT 1"){
                                ggroup = group;
                            }
                        }
                    }
                    iter = list;
                    while(iter.hasNext()){
                        QVariant data = iter.next();
                        QMap<QString,QVariant> obj = data.toMap();
                        QString group = obj.value("group").toString();
                        std::weak_ptr<Device> d = (obj.value("devicePtr")).value<std::weak_ptr<Device> >();
                        if(auto dev = d.lock()){
//                            ParamLocker lock(*dev);
                            dev->storeRemoteControl();
                            QExplicitlySharedDataPointer<CommandSequence> s = dev->startCommandSequence("set application(alternate)");
                            if(group == ""){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcOff);
                                dev->setSynSequence(static_cast<int>(ParaSelects::SequenceModes::DISABLED));
                                dev->setApplication(0);  // disable
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::DISABLED));
                            }
                            ///////////FREEMASK///////////////////////////////////////////////
                            else if(group == "MAIN"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::ACTIVE));
                            }
                            else if(group == "MASK"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT1));
                            }
                            ///////ALTERNATE //////////////////////////////////////////////////
                            else if(ggroup == "ACTIVE"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                if(group == "ACTIVE"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::ACTIVE));
                                }
                                else if(group == "WAIT 1"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT1));
                                }
                                else if(group == "WAIT 2"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT2));
                                }
                                else if(group == "WAIT 3"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT3));
                                }
                            }
                            else if(ggroup == "WAIT 1"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::FREEMASK));
                                if(group == "ACTIVE"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::ACTIVE));
                                }
                                else if(group == "WAIT 1"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT1));
                                }
                            }
                            else if(ggroup == "WAIT 2"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::ALTERNATE_3_MASKS));
                                if(group == "ACTIVE"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::ACTIVE));
                                }
                                else if(group == "WAIT 1"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT1));
                                }
                                else if(group == "WAIT 2"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT2));
                                }
                            }
                            else if(ggroup == "WAIT 3"){
                                dev->setRemoteControl(ParaSelects::RemoteCtrl::RcIrRfs);
                                dev->setApplication(static_cast<int>(ParaSelects::FreemaskMode::ALTERNATE_4_MASKS));
                                if(group == "ACTIVE"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::ACTIVE));
                                }
                                else if(group == "WAIT 1"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT1));
                                }
                                else if(group == "WAIT 2"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT2));
                                }
                                else if(group == "WAIT 3"){
                                    dev->setMaskGroup(static_cast<int>(ParaSelects::MaskGroup::WAIT3));
                                }
                            }
                            dev->endCommandSequence(s);
                        }
                    }
                }
            }
        }
    }
    return false;
}

int Framework::BronStudio::getApplication() const{
    return m_specialMode;
}

bool Framework::BronStudio::setShowCogni(){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        dev->setShowCogni();
        ret = true;
    }
    return ret;
}

bool Framework::BronStudio::setCogniEnable(bool ena){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        if(dev->getCogniColEnable() != -1){
            dev->setCogniEnable(ena);
        }
        ret = true;
    }
    return ret;
}

int Framework::BronStudio::getCogniEnable(){
    int ret = 0;
    bool diff = false;
    bool isFirst = true;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        if(dev->getCogniColEnable() != -1){
            if(isFirst){
                isFirst = false;
                ret = dev->getCogniEnable();
            }
            else{
                int tret = dev->getCogniEnable();
                if(tret > ret){
                    ret = tret;
                    diff = true;
                }
            }
        }
    }
    if(diff){
        iter = getDevices();
        while(iter.hasNext()) {
            auto dev = iter.next();
            if(!dev->isAvailable()) continue;
            if(dev->getCogniColEnable() != -1){
                    dev->setCogniEnable(ret);
            }
        }
    }
    return ret;
}

bool Framework::BronStudio::setRemoteSwitch(int sel){
    bool ret = false;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        dev->setRemoteControl(static_cast<ParaSelects::RemoteCtrl>(sel));
        ret = true;
    }
    return ret;
}

int Framework::BronStudio::getRemoteSwitch(){
    bool isfirst = true;
    int act = 0;
    QListIterator<std::shared_ptr<Device> > iter(getDevices());
    while(iter.hasNext()) {
        auto dev = iter.next();
        if(!dev->isAvailable()) continue;
        if(isfirst){
            act = dev->getRemoteControl();
            if(act != -1){
                 isfirst = false;
            }
        }
        else{
            int tmp = dev->getRemoteControl();
            if(tmp != -1){
                if(act != tmp){
                    return static_cast<int>(ParaSelects::RemoteCtrl::RcCellIrRfs) + 1;
                }
            }
        }
    }

    return act;
}

void Framework::BronStudio::disableDumbDevices(){
    QListIterator<std::shared_ptr<Device> >iter(m_deviceList);
    while(iter.hasNext()){
        auto d = iter.next();
        d->makeApplicationTransparent();
    }
}

std::shared_ptr<StudioSequence> Framework::BronStudio::startStudioSequence(QString name){
    if(auto ss = std::make_shared<StudioSequence>(++m_sequenceNr, name)){
        m_sequences.append(ss);
        connect(ss.get(), SIGNAL(devSequenceDone(std::shared_ptr<StudioSequence>)), this, SLOT(onDevSequenceDone(std::shared_ptr<StudioSequence>)));
        return ss;
    }
    return nullptr;
}

void Framework::BronStudio::endStudioSequence(std::shared_ptr<StudioSequence> ss){
    ss->close();
}

QNetworkInterface Framework::BronStudio::getInterface() const{
    if(m_mySession){
        if(m_mySession->isOpen()){
            return m_mySession->interface();
        }
    }
    return QNetworkInterface();
}



void Framework::BronStudio::deleteDevices(){
//    QListIterator<std::shared_ptr<Device> > iter(m_deviceList);
    QMutableListIterator<std::shared_ptr<Device> > iter(m_deviceList);
    while( iter.hasNext()){
        if(auto d = iter.next()){
           // don't handle geniLamps, they are handled from scoro
                int type = d->getDeviceType();
                if(type == Device::eTypes::eGeni){
                    auto scoro = std::dynamic_pointer_cast<DeviceScoro>(d);
                    scoro->isInDeleter(true);
                }
                removeDeviceFromGroup(d);
                d->Shutdown();
                emit bronStudioGoneDevice(d);
                disconnect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), this, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));
                disconnect(d.get(), SIGNAL(deviceLost(std::weak_ptr<Device> )), this, SLOT(onDeviceLost(std::weak_ptr<Device> )));
                disconnect(d.get(), SIGNAL(error(QString)), this, SIGNAL(error(QString)));
                // be careful, iter.remove deletes the element (smart pointer)
                iter.remove();
            }
    }

}

bool Framework::BronStudio::deleteDevice(std::weak_ptr<Device> dev){
    if(auto d = dev.lock()){
        if(removeDevice(d)){
            d->Shutdown();
            disconnect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), this, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));
            disconnect(d.get(), SIGNAL(deviceLost(std::weak_ptr<Device> )), this, SLOT(onDeviceLost(std::weak_ptr<Device> )));
            disconnect(d.get(), SIGNAL(error(QString)), this, SIGNAL(error(QString)));
            return true;
        }
    }
    return false;
}

bool Framework::BronStudio::removeDevice(std::weak_ptr<Device> dev){
    if(auto d = dev.lock()){
        if(removeDeviceFromGroup(d)){
            if(m_deviceList.removeOne(d)){
                emit bronStudioGoneDevice(d);
                if(m_deviceList.isEmpty()){
                    m_isOnline = false;
                    emit netConfigChanged();
                    emit studioEmpty();
                }
                return true;
            }
        }
    }
    return false;
}

bool Framework::BronStudio::addDevice(std::weak_ptr<Device> dev){
    if(auto d = dev.lock()){
        if(!m_deviceList.contains(d)){
            emit bronStudioNewDevice(d);
            m_deviceList.append(d);
            emit netConfigChanged();
            return addDeviceToGroup(d);
        }
    }
    return false;
}



bool Framework::BronStudio::isDeviceAlreadyIn(std::weak_ptr<Device> d){
    if(auto dev = d.lock()){
        QListIterator<std::shared_ptr<Device> > iter(m_deviceList);
        while(iter.hasNext()){
            auto d = iter.next();
            if(d->getHiddenId() == dev->getHiddenId()){
                return true;
            }
        }
    }
    return false;
}

int Framework::BronStudio::netWorkState() const{
    if(m_deviceList.count() > 0){
        return static_cast<int>(BronStudio::eNetworkState::eActive);
    }
    else{
        return static_cast<int>(BronStudio::eNetworkState::eKnownNotVisible);
    }
}


void Framework::BronStudio::onNetConfigChanged(const QNetworkConfiguration cfg){
    if(Name() == cfg.name()){
        if(!cfg.state().testFlag(QNetworkConfiguration::Active)){
            m_isOnline = false;
        }
        emit netConfigChanged();
    }
}

void Framework::BronStudio::onNetworkError(QNetworkSession::SessionError error){
    switch(error){
    case QNetworkSession::UnknownSessionError:
    case QNetworkSession::SessionAbortedError:
    case QNetworkSession::RoamingError:
    case QNetworkSession::OperationNotSupportedError:
    case QNetworkSession::InvalidConfigurationError:
        break;
    }
}

void Framework::BronStudio::onDevSequenceDone(std::shared_ptr<StudioSequence>seq){
    if(m_sequences.contains(seq)){
        disconnect(seq.get(), SIGNAL(devSequenceDone(std::shared_ptr<StudioSequence>)), this, SLOT(onDevSequenceDone(std::shared_ptr<StudioSequence>)));
        m_sequences.removeOne(seq);
    }
}

void Framework::BronStudio::onNetworkStateChanged(QNetworkSession::State state){
    bool online = false;

    switch(state){
    case QNetworkSession::Invalid:
    case QNetworkSession::NotAvailable:  break;
    case QNetworkSession::Connecting:
    case QNetworkSession::Connected:     online = true; break;
    case QNetworkSession::Closing:
    case QNetworkSession::Disconnected: break;
    case QNetworkSession::Roaming:      online = true; break;
    }
    if(online != m_isOnline){
        emit netConfigChanged();
    }
}


