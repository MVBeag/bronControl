#include "deviceFinder.h"
#include "paraSelectables.h"
#include "devicesirros.h"
#include "devicesirrosL.h"
#include "devicesirrosS.h"
#include "devicescoro.h"
#include "genLamp.h"
#include "studiomgr.h"

using namespace Framework;

Framework::DeviceFinder::DeviceFinder()
    : QObject()
    , m_mutex(QMutex::Recursive)
    , m_broadcastDevices()
    , m_creatingDevices(){
}

Framework::DeviceFinder::~DeviceFinder(){
    m_creatingDevices.clear();
    m_broadcastDevices.clear();
}

void DeviceFinder::checkResponse(QHostAddress sender, quint16 senderPort, uint interfaceIx){
//    qDebug() << "checkResponse interfaceIndex " << interfaceIx;
    QMutexLocker lock(&m_mutex);
    QListIterator<std::shared_ptr<ScanDevice> >iter(m_broadcastDevices);

    while(iter.hasNext()){
        auto act = iter.next();
        // filter out broadcast to own address
        if(sender == act->getNetworkEntry().ip()){
            continue;
        }
        if(sender.isInSubnet(act->getNetworkEntry().ip(),act->getNetworkEntry().prefixLength())){
            // signal broadcasting device that there was a response to prevent autodelete of the broadcast
            // device
            // keep this one alive
            act->disableSelfDeletion();
            auto cd = std::make_shared<CreatorDevice>(interfaceIx);
            cd->setHostAddress(sender,senderPort);
            cd->setIsDeviceAwareness(true);
            cd->init();
            cd->setPolling(false);
            cd->setOnline(true);
            connect(cd.get(), SIGNAL(initParasRead(std::weak_ptr<Framework::Device>)),this, SLOT(deviceIdentified(std::weak_ptr<Framework::Device>)));
            m_creatingDevices.append(cd);
        }
    }
}

void Framework::DeviceFinder::removeCreatorDevice(std::weak_ptr<CreatorDevice> dev){
    QMutexLocker lock(&m_mutex);
    if(auto d = dev.lock()){
        d->Shutdown();
        if(m_creatingDevices.contains(d)){
            int ix = m_creatingDevices.indexOf(d);
            m_creatingDevices.removeAt(ix);
        }
    }
}

void Framework::DeviceFinder::startDiscoverDevices(BronStudio *studio, QList<QNetworkAddressEntry> usedAddresses){
    QMutexLocker lock(&m_mutex);
    QListIterator<QNetworkAddressEntry> iter(usedAddresses);
    // check if we know the interface already
    while(iter.hasNext()){
        QNetworkAddressEntry ae = iter.next();
        // start scan for all valid ip broadcast addresses
        auto bcast = std::make_shared<ScanDevice>();
        bcast->setStudio(studio);
        bcast->setHostAddress(ae.broadcast(), MW_DEVICE_DEFAULT_SERVICE_PORT);
        bcast->init();
        bcast->setOnline(true);
        bcast->setNetworkEntry(ae);
        connect(bcast.get(), SIGNAL(deleteDevice(std::weak_ptr<Device>)), this, SLOT(removeAnonymusDevice(std::weak_ptr<Device>)));//, Qt::QueuedConnection);
        m_broadcastDevices.append(bcast);
    }
}

void Framework::DeviceFinder::stopDiscoverDevices(BronStudio *studio, bool stopAll){
    QMutexLocker lock(&m_mutex);
    QMutableListIterator<std::shared_ptr<ScanDevice> >iter(m_broadcastDevices);
    while(iter.hasNext()){
        auto sd = iter.next();
        if(stopAll){
            sd->setOnline(false);
            sd->Shutdown();
            iter.remove();
        }
        else{
            if(studio){
                if(sd->isForStudio(studio)){
                    sd->setOnline(false);
                    sd->Shutdown();
                    iter.remove();
                }
            }
        }
    }
}

void Framework::DeviceFinder::removeAnonymusDevice(std::weak_ptr<Device> dev){
    QMutexLocker lock(&m_mutex);
    if(auto dd = std::dynamic_pointer_cast<ScanDevice>(dev.lock())){
        QList<std::shared_ptr<ScanDevice> >::iterator iter = m_broadcastDevices.begin();
        while (iter != m_broadcastDevices.end()) {
            if(*iter == dd){
                (*iter)->setOnline(false);
                (*iter)->Shutdown();
                iter = m_broadcastDevices.erase(iter);
            }
            else{
                ++iter;
            }
        }
    }
}

void Framework::DeviceFinder::deviceIdentified(std::weak_ptr<Device> d){
    QMutexLocker lock(&m_mutex);
    if(auto dev = d.lock()){
    auto cd = std::dynamic_pointer_cast<CreatorDevice>(dev);
        if(cd){
            std::shared_ptr<Device> newDevice(nullptr);
            removeCreatorDevice(cd);
            QString name = cd->getName();
            if(name == "SirosL"){
                newDevice = std::make_shared<DeviceSirosL>(cd);
                newDevice->correctNetworkSettings();
            }
            else if(name == "Siros"){
                switch(cd->getPara(DeviceSiros::F_STOP_RANGE)->data().toInt()){
                case 6:{
                    newDevice = std::make_shared<DeviceSiros>(cd);
                    newDevice->correctNetworkSettings();
                    break;
                }
                case 8:{
                    newDevice = std::make_shared<DeviceSirosS>(cd);
                    newDevice->correctNetworkSettings();
                    break;
                }
                default: qDebug() << "unkown Device found";
                }

            }
            else if(name == "scoro"){
                newDevice = std::make_shared<DeviceScoro>(cd);
                connect(dynamic_cast<DeviceScoro *>(newDevice.get()), &DeviceScoro::createLamp, this, &DeviceFinder::onCreateLamp);
                connect(dynamic_cast<DeviceScoro *>(newDevice.get()), &DeviceScoro::destroyLamp, this, &DeviceFinder::onDestroyLamp);
            }

            if(newDevice){
                StudioMgr::getInstance()->newIdentifiedDevice(newDevice);
            }
        }
    }

}

void Framework::DeviceFinder::onCreateLamp(DeviceScoro *d, int lampNr){
    QMutexLocker lock(&m_mutex);
    auto gl = std::make_shared<GenLamp>(lampNr, d, d->getDemo());
    if(gl){
        d->addLamp(gl);
        switch(lampNr){
        case 0:
            connect(d, SIGNAL(l1AddrChanged(QVariant)), gl.get(), SLOT(onAddrChanged(QVariant)));
            connect(d, SIGNAL(l1SwChanged(QVariant)), gl.get(), SLOT(onSwChanged(QVariant)));
            connect(d, SIGNAL(l1ApertureChanged(QVariant)), gl.get(), SLOT(onApertureChanged(QVariant)));
            break;
        case 1:
            connect(d, SIGNAL(l2AddrChanged(QVariant)), gl.get(), SLOT(onAddrChanged(QVariant)));
            connect(d, SIGNAL(l2SwChanged(QVariant)), gl.get(), SLOT(onSwChanged(QVariant)));
            connect(d, SIGNAL(l2ApertureChanged(QVariant)), gl.get(), SLOT(onApertureChanged(QVariant)));
            break;

        case 2:
            connect(d, SIGNAL(l3AddrChanged(QVariant)), gl.get(), SLOT(onAddrChanged(QVariant)));
            connect(d, SIGNAL(l3SwChanged(QVariant)), gl.get(), SLOT(onSwChanged(QVariant)));
            connect(d, SIGNAL(l3ApertureChanged(QVariant)), gl.get(), SLOT(onApertureChanged(QVariant)));
            break;
        }


        StudioMgr::getInstance()->newIdentifiedDevice(gl);
    }
}

void Framework::DeviceFinder::onDestroyLamp(DeviceScoro *d, int lampNr){
    QMutexLocker lock(&m_mutex);
        auto gl = d->removeLamp(lampNr).lock();
        if(gl){
            switch(lampNr){
            case 0:
                disconnect(d, SIGNAL(l1AddrChanged(QVariant)), gl.get(), SLOT(onAddrChanged(QVariant)));
                disconnect(d, SIGNAL(l1SwChanged(QVariant)), gl.get(), SLOT(onSwChanged(QVariant)));
                disconnect(d, SIGNAL(l1ApertureChanged(QVariant)), gl.get(), SLOT(onApertureChanged(QVariant)));
                break;
            case 1:
                disconnect(d, SIGNAL(l2AddrChanged(QVariant)), gl.get(), SLOT(onAddrChanged(QVariant)));
                disconnect(d, SIGNAL(l2SwChanged(QVariant)), gl.get(), SLOT(onSwChanged(QVariant)));
                disconnect(d, SIGNAL(l2ApertureChanged(QVariant)), gl.get(), SLOT(onApertureChanged(QVariant)));
                break;

            case 2:
                disconnect(d, SIGNAL(l3AddrChanged(QVariant)), gl.get(), SLOT(onAddrChanged(QVariant)));
                disconnect(d, SIGNAL(l3SwChanged(QVariant)), gl.get(), SLOT(onSwChanged(QVariant)));
                disconnect(d, SIGNAL(l3ApertureChanged(QVariant)), gl.get(), SLOT(onApertureChanged(QVariant)));
                break;
            }
            emit gl->deviceLost(gl);
        }
}


bool operator ==(const QNetworkInterface &a, const QNetworkInterface &b){
    if(a.isValid() && b.isValid()){
        return a.hardwareAddress() == b.hardwareAddress();
    }
    return false;
}
