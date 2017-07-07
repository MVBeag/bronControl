/**
  * @abstract studioMgr managed the known studios and the settings stored within the studios
  * BE CAREFUL, WITH Version V2.0 RC 1 a new Section "Studio Manager" with a new Key BackgroundUpdate was inserted!!
  *
  */

#include "bronstudio.h"
#include "helpers.h"
#include "devicesirros.h"
#include "studiomgr.h"
#include <QString>
#include "devicescoro.h"
#include <QSettings>
#include "devicesirros.h"
#include "devicesirrosS.h"
#include "devicesirrosL.h"

using namespace Framework;


// MOVE DEVICE FINDER TO STUDIO MGR !!
//connect(this, SIGNAL(startDeviceSearch(QList<scanAddressFinder>)), DeviceFinder::getInstance(), SLOT(startDiscoverDevices(QList<scanAddressFinder>)));



Framework::StudioMgr::StudioMgr(QObject *parent)
    :QObject(parent)
    , QSharedData()
    , m_mutex(QMutex::Recursive)
    , m_studios()
    , m_netScanner(NetworkScanner::getInstance())
    , m_deviceFinder()
    , m_netAccess(std::make_shared<NetworkAPIImpl> (nullptr, MW_DEVICE_DEFAULT_SERVICE_PORT))
    , m_knownScanAddresses()
    , m_backgroundUpdate(true)
#if defined  Q_OS_IOS
    , m_discoveryTimer()
#elif defined Q_OS_ANDRROID
    , m_discoveryTimer()
#endif
    , m_userScale(1.0f)
{
#if defined  Q_OS_IOS
    m_discoveryTimer.setInterval(static_cast<int>(settings::discoveryTime));
    m_discoveryTimer.setSingleShot(false);
    connect(&m_discoveryTimer, &QTimer::timeout, this, &StudioMgr::onDiscoveryTimeout);
    m_discoveryTimer.start();
#elif defined Q_OS_ANDROID
    m_discoveryTimer.setInterval(static_cast<int>(settings::discoveryTime));
    m_discoveryTimer.setSingleShot(false);
    connect(&m_discoveryTimer, &QTimer::timeout, this, &StudioMgr::onDiscoveryTimeout);
    m_discoveryTimer.start();
#endif

    connect(m_netScanner.data(), &NetworkScanner::netInserted, this, &StudioMgr::netAdded);
    connect(m_netScanner.data(), &NetworkScanner::netRemoved, this, &StudioMgr::netRemoved);
    connect(m_netScanner.data(), &NetworkScanner::firstScanComplete, this, &StudioMgr::checkAndInsertPossibleStudios);
    connect(m_netScanner.data(), &NetworkScanner::netConfigChanged, this, &StudioMgr::onNetConfigChanged);
    readSettings();
    // start scan here!
    m_netScanner->scan();
}

void Framework::StudioMgr::populateDemoStudio(){
    if(m_studios.contains("Bron-Demo0")){
        BronStudio *bs = m_studios.value("Bron-Demo0");
        m_studios.remove("Bron-Demo0");
        bs->deleteLater();
    }
    for(int i = 0; i < 1; i++){
        BronStudio *bronStudio = new BronStudio(true, this);
        if(bronStudio){
           emit studioAdded(bronStudio);
           // name and address are fixed in demo studio
           bronStudio->setName("Bron-Demo" + QString::number(i));
           bronStudio->setSsid("Bron-Demo" + QString::number(i));
           bronStudio->setAddress(i);
           bronStudio->setOnline(false);
           m_studios.insert("Bron-Demo" + QString::number(i), bronStudio);
           std::shared_ptr<Device> d(nullptr);
           d = std::make_shared<DeviceSirosL>(1, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosL");
           d->setParameter(DeviceSiros::MAX_ENERGY, 800);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 5.0);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.1"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosL>(1, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosL");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 7.0);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.2"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosS>(2, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosS");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.4"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosL>(2, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosL");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.5"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosS>(3, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosS");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.6"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSiros>(3, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "Siros");
           d->setParameter(DeviceSiros::MAX_ENERGY, 800);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 6);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.7"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosL>(4, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosL");
           d->setParameter(DeviceSiros::MAX_ENERGY, 800);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 5.0);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.8"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosL>(4, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosL");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 7.0);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.9"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosS>(5, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosS");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.10"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosL>(5, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosL");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.11"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSirosS>(6, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "SirosS");
           d->setParameter(DeviceSiros::MAX_ENERGY, 400);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 8);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.12"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d = std::make_shared<DeviceSiros>(6, true);
           d->setParameter(DeviceSiros::DEVICE_NAME, "Siros");
           d->setParameter(DeviceSiros::MAX_ENERGY, 800);
           d->setParameter(DeviceSiros::ENERGY_TOTAL_APPERTURE, 4.3);
           d->setParameter(DeviceSiros::F_STOP_RANGE, 6);
           d->setParameter(DeviceSiros::FLASH_DURATION, 3000);
           d->setParameter(DeviceSiros::WLAN_LOGIN1_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.13"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
#if 0
           d = std::make_shared<DeviceScoro>(6, true);
           d->setParameter(DeviceScoro::DEVICE_DESC, 4); // scoro 3200 S
           d->setParameter(DeviceScoro::DEVICE_NAME, "ScoroS");
           d->setParameter(DeviceScoro::SCORO_ENERGY_TYPE, 3200);
           d->setParameter(DeviceScoro::GEN_APERTURE, 4.3);
           d->setParameter(DeviceScoro::F_STOP_RANGE, 9.1);
           d->setParameter(DeviceScoro::T01_VALUE, 3000);
           d->setParameter(DeviceScoro::NW_SSID, "Bron-Demo0");
           d->setHostAddress(QHostAddress("127.0.0.14"),1234, false);
           d->init();
           if(bronStudio->addDevice(d))
               connect(d.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), bronStudio, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
           d->setParameter(DeviceScoro::L1_INSERTED, true);
           d->setParameter(DeviceScoro::L2_INSERTED, true);
           d->setParameter(DeviceScoro::L3_INSERTED, true);
#endif
        }
    }
}

void Framework::StudioMgr::freeze(){
    QMapIterator<QString, BronStudio*> iter(m_studios);
    while(iter.hasNext()){
        iter.next();
        auto s = iter.value();
        s->freeze();
    }
}

void Framework::StudioMgr::unfreeze(){
    QMapIterator<QString, BronStudio*> iter(m_studios);
    while(iter.hasNext()){
        iter.next();
        auto s = iter.value();
        s->unfreeze();
    }
}

QExplicitlySharedDataPointer<StudioMgr> Framework::StudioMgr::getInstance(){
    static QExplicitlySharedDataPointer<StudioMgr> mgr(new StudioMgr());

    return mgr;
}

void Framework::StudioMgr::storeSettings(){
    QSettings settings;
    settings.setFallbacksEnabled(false);

    settings.beginGroup("Studio Manager");

    settings.setValue("BackgroundUpdate", getBackgroundUpdate());
    settings.setValue("userScale", getUserScale());

    settings.beginGroup("Studios");
    QMapIterator<QString, BronStudio*> iter(m_studios);
    while(iter.hasNext()){
        iter.next();
        if(iter.key() != "Bron-Demo0"){
            // storage format SSID_#@Address
            BronStudio *s = iter.value();
            settings.beginGroup(iter.key() + "_#@" + QString::number(s->getAddress()));
            settings.setValue("Name",s->Name());
            settings.setValue("Version", "2.00.00");
            settings.setValue("Adress", s->getAddress());
            settings.setValue("SSID", s->Ssid());
            settings.setValue("Password", s->Password());
            settings.setValue("EncryptionMode", s->EncryptionMode());
            settings.setValue("SpecMode", s->getApplication());
            settings.endGroup(); // studio
        }
    }
    settings.endGroup(); // studios
    settings.endGroup(); // studio manager since V2.0 RC 1 (28.04.2017)
}

void Framework::StudioMgr::readSettings(){
    QSettings settings;
    settings.setFallbacksEnabled(false);

    bool hasStudioMgr = false;
    if(settings.contains("Studio Manager/BackgroundUpdate")){
        hasStudioMgr = true;
    }

    if(hasStudioMgr){
        settings.beginGroup("Studio Manager");
        setBackgroundUpdate(settings.value("BackgroundUpdate").toBool());
        if(settings.contains("userScale")){
            setUserScale(settings.value("userScale").toFloat());
        }
    }
    settings.beginGroup("Studios");
    QListIterator<QString> studios(settings.childGroups());
    while(studios.hasNext()){
        QString studio;
        QString studio_Sign = studios.next();
        auto nameParts = studio_Sign.split("_#@");
        if(nameParts.size()>=1) studio = nameParts[0];
        if(studio != "Bron-Demo0"){
            settings.beginGroup(studio_Sign);
            BronStudio *bronStudio = new BronStudio(false, this);
            if(bronStudio){
                // name and address are fixed in demo studio
                bronStudio->setName(studio);
                bronStudio->setAddress(settings.value("Adress").toInt());
                bronStudio->setPassword(settings.value("Password").toString());
                bronStudio->setSsid(settings.value("SSID").toString());
                bronStudio->setEncrypionMode(settings.value("EncryptionMode").toInt());
                bronStudio->setOnline(false);
                bronStudio->setBackgroundUpdate(hasStudioMgr ? getBackgroundUpdate() : true);  // switched on by default
                addStudio(bronStudio);
            }
            settings.endGroup(); // studio
        }
    }
    settings.endGroup(); // studios

    if(hasStudioMgr){
        settings.endGroup(); // studio manager since V2.0 RC 1 (28.04.2017)
    }
}

Framework::StudioMgr::~StudioMgr(){
#if defined Q_OS_IOS
    m_discoveryTimer.stop();
#elif defined Q_OS_ANDROID
    m_discoveryTimer.stop();
#endif
    storeSettings();
}

bool Framework::StudioMgr::addStudio(BronStudio *s){
    if(!m_studios.contains(s->Name())){
        m_studios.insert(s->Name(),s);
        emit studioAdded(s);
        connect(this, SIGNAL(netConfigChanged(const QNetworkConfiguration)), s, SLOT(onNetConfigChanged(const QNetworkConfiguration)));
        return true;
    }
    else{
        bool isIn = false;
        QListIterator<BronStudio *>iter(m_studios.values(s->Name()));
        while(iter.hasNext()){
            auto studio = iter.next();
            if(studio->getAddress() == s->getAddress()){
                isIn = true;
                break;
            }
        }
        if(!isIn){
            m_studios.insert(s->Name(),s);
            emit studioAdded(s);
            connect(this, SIGNAL(netConfigChanged(const QNetworkConfiguration)), s, SLOT(onNetConfigChanged(const QNetworkConfiguration)));
            return true;
        }
    }
    return false;
}

bool Framework::StudioMgr::removeStudio(BronStudio *s){
    bool wasIn = false;
    if(s){
        if(m_studios.contains(s->Name())){
            QList<BronStudio *> list = m_studios.values(s->Name());
            QMutableListIterator<BronStudio *> iter(list);
            while(iter.hasNext()){
                auto studio = iter.next();
                if(studio->getAddress() == s->getAddress()){
                    iter.remove();
                    disconnect(this, SIGNAL(netConfigChanged(const QNetworkConfiguration)), studio, SLOT(onNetConfigChanged(const QNetworkConfiguration)));
                    emit studioRemoved(studio);
                    wasIn = true;
                    studio->deleteLater();
                }
            }

//            BronStudio *studio = m_studios.take(s->Name());
//            if(studio){
//                disconnect(this, SIGNAL(netConfigChanged(const QNetworkConfiguration)), studio, SLOT(onNetConfigChanged(const QNetworkConfiguration)));
//                emit studioRemoved(studio);
//                wasIn = true;
//                studio->deleteLater();
//            }
        }
    }
    if(wasIn) return true;
    else     return false;
}

QString Framework::StudioMgr::Name() const{
    return "StudioMgr";
}

QString Framework::StudioMgr::classType() const{
    return "";
}

bool Framework::StudioMgr::hasStudio(const QString &name) const{
    return m_studios.contains(name);
}


QList<BronStudio *> Framework::StudioMgr::getStudios(const QString &name){
    if(m_studios.contains(name)){
        return m_studios.values(name);
    }
    return QList<BronStudio *>();
}

int Framework::StudioMgr::getNumberOfStudios() const{
    return m_studios.size();
}

void Framework::StudioMgr::setOffline(bool val){
    QMap<QString, BronStudio*>::const_iterator iter = m_studios.constBegin();
    QMap<QString, BronStudio*>::const_iterator end = m_studios.constEnd();
    for (; iter != end; ++iter) {
        (*iter)->setOnline(!val);
    }
}

bool Framework::StudioMgr::autoConnectPossible() const{
    if(m_netScanner){
        return m_netScanner->isAutoConnectPossible();
    }
    return false;
}

QMultiMap<QString, BronStudio *> *Framework::StudioMgr::getStudios(){
    return &m_studios;
}

std::weak_ptr<NetworkAPI> StudioMgr::CreateConnection(int localPort, std::shared_ptr<Device> dev){
    Q_UNUSED(localPort);
    QMutexLocker lock(&m_mutex);
    if(dev){
        auto nw = getNetAccess();
        if(auto net = nw.lock()){
            net->append(dev->getHostAddress(), dev);
            return nw;
        }
    }

    return std::weak_ptr<NetworkAPI>();
}

bool Framework::StudioMgr::activateStudio(bool activate ,BronStudio *studio){
    bool isIn = false;
    if(m_studios.contains(studio->Name())){
        QListIterator<BronStudio *>iter(m_studios.values(studio->Name()));
        while(iter.hasNext()){
            auto s = iter.next();
            if(s->getAddress() == studio->getAddress()){
                isIn = true;
            }
        }

        if(isIn){
            if(true == activate){
                studio->setOnline(true);
            }
            else{
                studio->setOnline(false);
                if(studio->Name() != "Bron-Demo0"){
    //                studio->deleteDevices();
                }
            }
            return true;
        }
    }
    return false;
}

bool Framework::StudioMgr::createStudio(QString ssid, int studioAddr, QString pw, int encr){
    bool isIn = false;
    if(!m_studios.contains(ssid)){
        ;   // isIn is already correct
    }
    else{
        QListIterator<BronStudio *>iter(m_studios.values(ssid));
        while(iter.hasNext()){
            auto studio = iter.next();
            if(studio->getAddress() == studioAddr){
                isIn = true;
            }
        }
    }


    if(!isIn){
        // create a studio with an invalid QNetworkConfiguration
        BronStudio *bs = new BronStudio(NetworkScanner::getInstance()->getNetworkCfgMgr()->configurationFromIdentifier(ssid),
                                        false, this);
        if(bs){
            bs->setSsid(ssid);
            bs->setAddress(studioAddr);
            bs->setPassword(pw);
            bs->setName(ssid);
            bs->setEncrypionMode(encr);
            bs->setBackgroundUpdate(getBackgroundUpdate());
            if(!addStudio(bs)){
                delete bs;
                return false;
            }
            storeSettings();
            return true;
        }
    }
    return false;
}

void Framework::StudioMgr::netAdded(const QNetworkConfiguration &n){
    Q_UNUSED(n)
//    if(n.name().contains("Bron-Studio")){
//        BronStudio *bs = new BronStudio(n, false, this);
//        if(bs){
//            bs->setAddress(extractNum(n.name()));
//            if(!addStudio(bs)){
//                delete bs;
//            }
//        }
//    }
}

void Framework::StudioMgr::netRemoved(const QNetworkConfiguration &n){
    if(hasStudio(n.name())){
        QListIterator<BronStudio *> iter(getStudios(n.name()));
        while(iter.hasNext()){
            iter.next()->setOnline(false);
        }
    }
}

void Framework::StudioMgr::checkAndInsertPossibleStudios(){
    QListIterator<QNetworkConfiguration> netIter(m_netScanner->getNetworks());
    while(netIter.hasNext()){
        QNetworkConfiguration c = netIter.next();
        QMapIterator<QString, BronStudio *>studioIter(m_studios);
        while(studioIter.hasNext()){
            studioIter.next();
            BronStudio *studio = studioIter.value();
            bool isNetworkValid = studio->getNetwork().isValid();
            if(isNetworkValid){
                if(studio->getNetwork().name() == c.name()){
                    break;
                }
            }
            else if(studio->Name() == c.name()){
                // set configuration if studio name is equal to network name
                studio->setNetwork(c);
                break;
            }
        }
    }
//        if(!isIn){
//            if(c.name().contains("Bron-Studio")){
//                BronStudio *bs = new BronStudio(c,false, this);
//                if(bs){
//                    bs->setAddress(extractNum(c.name()));
//                    if(!addStudio(bs)){
//                        delete bs;
//                    }
//                }
//            }
//       }
//    }
    emit networkScanFinished();
}

void Framework::StudioMgr::newIdentifiedDevice(std::weak_ptr<Device> d){
    if(auto dev = d.lock()){
        bool studioExists = false;
        int studioAddress = dev->getStudioAddress();
        QString studioSSID = dev->getSSID();
        // check if studio already exists
        QListIterator<BronStudio *> iter(m_studios.values());
        // check first if studio is online and device belongs to it
        while(iter.hasNext()){
            BronStudio *s = iter.next();
            if(s->getAddress() == studioAddress && s->Ssid() == studioSSID){
                studioExists = true;
                // this method is called form online device!
                // to do: in enterprize mode, more than one studio can be online
                //            if(s->getOnline()){
                // check if dev is already in studio
                if(!s->isDeviceAlreadyIn(dev)){
                    if(s->addDevice(dev)){
                        connect(dev.get(), SIGNAL(deviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)), s, SLOT(onDeviceDataChanged(std::weak_ptr<DevicePara>,std::weak_ptr<Device>)));//, Qt::QueuedConnection);
                        connect(dev.get(), SIGNAL(initParasRead(std::weak_ptr<Device>)), s, SLOT(onInitParasRead(std::weak_ptr<Device>)));//, Qt::QueuedConnection);
                        connect(dev.get(), SIGNAL(deviceLost(std::weak_ptr<Device> )), s, SLOT(onDeviceLost(std::weak_ptr<Device> )));//, Qt::QueuedConnection);
                        connect(dev.get(), SIGNAL(error(QString)), s, SIGNAL(error(QString)));
                        connect(dev.get(), SIGNAL(swVersionRead(std::shared_ptr<Device>)), this, SLOT(onSwVersionRead(std::shared_ptr<Device>)));
                        dev->init();
                        dev->setPolling(s->getPolling());
                        dev->setIsDeviceAwareness(s->getDeviceAwareness());
                        dev->setOnline(getBackgroundUpdate());
                    }
                }
            }
        }

        if(studioExists == false){
            // create one with the address and ssid as name
            createStudio(dev->getSSID(), studioAddress, dev->getPWD(), dev->getEncryptionMode());
        }
    }
}


void Framework::StudioMgr::startDiscovery(BronStudio *studio){
    // interface returns an invalid interface for some reasons
    // so as workaround filter all valid ipv4 addresses of all interfaces
    // that are up (we don't care about the specific interface)
    // loop over all interfaces

// DON'T DO THAT, ON OSX THERE CAME BULLSHIT BACK FOR THE INTERFACE, ASK APPLE NOT ME
//    QNetworkInterface iface = studio->getInterface();
//    if(iface.isValid()){
//        qDebug() << iface.humanReadableName();
//        QListIterator<QNetworkAddressEntry> i(iface.addressEntries());
//        while (i.hasNext()) {
//            QNetworkAddressEntry e = i.next();
//            m_knownScanAddresses.append(e);
//            qDebug() << e.ip().toIPv4Address() << " " << e.broadcast().toIPv4Address() << e.ip().protocol();
//        }
//    }
//    else{
    QListIterator<QNetworkInterface> iter(QNetworkInterface::allInterfaces());
    while(iter.hasNext()){
        QNetworkInterface iface = iter.next();
        // check if interface is up, no loopback is able to broadcast
        if((!iface.flags().testFlag(QNetworkInterface::IsLoopBack)) &&
                iface.flags().testFlag(QNetworkInterface::IsUp) &&
                iface.flags().testFlag(QNetworkInterface::CanBroadcast)){
            // check all address entries of this interface
            QListIterator<QNetworkAddressEntry>addrIter(iface.addressEntries());
            while(addrIter.hasNext()){
                QNetworkAddressEntry addr = addrIter.next();
                if(addr.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                        addr.ip() != QHostAddress(QHostAddress::LocalHost)){
                    m_knownScanAddresses.append(addr);
                }
            }
        }
    }
    //    }
    if(m_knownScanAddresses.size() > 0){
        m_deviceFinder.startDiscoverDevices(studio, m_knownScanAddresses);
    }
}

void Framework::StudioMgr::stopDiscovery(BronStudio *studio){
    m_deviceFinder.stopDiscoverDevices(studio);
    m_knownScanAddresses.clear();
}


void Framework::StudioMgr::onNetConfigChanged(const QNetworkConfiguration cfg){
    Q_UNUSED(cfg)
    // check if we are in online state
    if(m_netScanner->getNetworkCfgMgr()->allConfigurations(QNetworkConfiguration::Active).count() > 0){
        stopDiscovery();
        startDiscovery();
    }
    else{
        // no interface active, so no discovery needed
        stopDiscovery();
    }

//    QMapIterator<QString, BronStudio *> mIter(m_studios);
//    while(mIter.hasNext()){
//        mIter.next();
//        qDebug() << mIter.value()->Name() <<   " StudioMgr::onNetworkChanged " << mIter.value()->Ssid() << " " << mIter.value()->getNetwork().name() << " " << cfg.name();
//        if(mIter.value()->Name() == cfg.name()){
//            emit netConfigChanged(cfg);
//        }
    //    }
}

#if defined Q_OS_IOS
void Framework::StudioMgr::onDiscoveryTimeout(){
    // check if we are in online state
    if(m_netScanner->getNetworkCfgMgr()->allConfigurations(QNetworkConfiguration::Active).count() > 0){
        stopDiscovery();
        startDiscovery();
    }
    else{
        // no interface active, so no discovery needed
        stopDiscovery();
    }

}
#elif defined Q_OS_ANDROID
void Framework::StudioMgr::onDiscoveryTimeout(){
    // check if we are in online state
    if(m_netScanner->getNetworkCfgMgr()->allConfigurations(QNetworkConfiguration::Active).count() > 0){
        stopDiscovery();
        startDiscovery();
    }
    else{
        // no interface active, so no discovery needed
        stopDiscovery();
    }
    
}
#endif

void Framework::StudioMgr::onApplicationStateChanged(Qt::ApplicationState state){
    switch (state){
    case Qt::ApplicationState::ApplicationActive:
        unfreeze();
        break;
    case Qt::ApplicationState::ApplicationHidden: break;
    case Qt::ApplicationState::ApplicationInactive:
    case Qt::ApplicationState::ApplicationSuspended:
        freeze();
        storeSettings();
        break;
    }
}
