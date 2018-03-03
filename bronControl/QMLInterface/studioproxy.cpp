#include "studioproxy.h"
#include "studiomgr.h"
#include "genLamp.h"
#include <QQmlEngine>
#include "appcontrol.h"
#include <limits>

using namespace Framework;

QMLInterface::StudioProxy::StudioProxy(BronStudio *std)
    : BronItemAPI()
    , m_studio(std)
    , m_deviceAPIPrototype()
    , m_groupAPIPrototype(this)
    , m_deviceList("studioDeviceList", &m_deviceAPIPrototype)
    , m_groupList("studioGroupList",&m_groupAPIPrototype)
    , m_minPower(0.0)
    , m_maxPower(0.0)
    , m_maxMinPower(0.0)
    , m_minMaxPower(10.0){
//    , m_remote(nullptr){
//    , m_mutex(){
    if(m_studio){
        // handle Device and group proxies
        QList<std::shared_ptr<Device> >devList = m_studio->getDevices();
        auto iter = devList.constBegin();
        auto end = devList.constEnd();
        for(;iter != end; ++iter){
            if(auto p = std::make_shared<DeviceProxy>(*iter)){
                int ix = m_deviceList.insert(p);
                if(ix != -1){
                    connect(p.get(), SIGNAL(deviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                    connect(p.get(), SIGNAL(deviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                    p->setModelIx(ix);
                }
            }
        }

        QListIterator<BronGroup *> giter(m_studio->GetGroups());
        while(giter.hasNext()){
            BronGroup *bg = giter.next();
            if(auto proxy = std::make_shared<BronGroupProxy>(this, bg)){
                int ix = m_groupList.insert(proxy);
                if(ix != -1){
                    connect(proxy.get(), SIGNAL(groupProxyDataChanged(int, QVector<int>)), &m_groupList,  SLOT(dataChanged(int,QVector<int>)));//, Qt::QueuedConnection);
                    connect(proxy.get(), SIGNAL(elementChanged(int)), &m_groupList, SLOT(onElementChanged(int)));
                    proxy->setModelIx(ix);
                }
            }
        }

        connect(m_studio, SIGNAL(bronStudioGroupAdded(BronGroup*)), this, SLOT(onBronStudioGroupAdded(BronGroup*)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(bronStudioGroupRemoved(BronGroup*)), this, SLOT(onBronStudioGroupRemoved(BronGroup*)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(bronStudioNewDevice(std::weak_ptr<Device>)), this, SLOT(onBronStudioNewDevice(std::weak_ptr<Device>)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(bronStudioGoneDevice(std::shared_ptr<Device>)), this, SLOT(onBronStudioGoneDevice(std::shared_ptr<Device>)));//, Qt::QueuedConnection);
        // real update of the HMI (deviceList)
        connect(this, SIGNAL(studioProxyDeviceProxyDataChanged(int,QVector<int>)), &m_deviceList, SLOT(dataChanged(int,QVector<int>)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(netConfigChanged()), this, SLOT(onNetConfigChanged()));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(dataChanged(BronStudio::eData)), this, SLOT(onDataChanged(BronStudio::eData)));
        connect(m_studio, SIGNAL(error(QString)), this, SLOT(onError(QString)));
        }
}

QMLInterface::StudioProxy::StudioProxy(const QMLInterface::StudioProxy &p)
    : BronItemAPI()
    , m_studio(p.m_studio)
    , m_deviceAPIPrototype()        // only needed for roles
    , m_groupAPIPrototype(this)         // only needed for roles
    , m_deviceList(p.m_groupList)
    , m_groupList(p.m_groupList)
    , m_minPower(0.0)
    , m_maxPower(0.0)
    , m_maxMinPower(0.0)
    , m_minMaxPower(10.0){
//    , m_remote(nullptr){
//    , m_mutex(){
    if(m_studio){
        connect(m_studio, SIGNAL(bronStudioGroupAdded(BronGroup*)), this, SLOT(onBronStudioGroupAdded(BronGroup*)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(bronStudioGroupRemoved(BronGroup*)), this, SLOT(onBronStudioGroupRemoved(BronGroup*)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(bronStudioNewDevice(std::weak_ptr<Device>)), this, SLOT(onBronStudioNewDevice(std::weak_ptr<Device>)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(bronStudioGoneDevice(std::shared_ptr<Device>)), this, SLOT(onBronStudioGoneDevice(std::shared_ptr<Device>)));//, Qt::QueuedConnection);
        connect(this, SIGNAL(studioProxyDeviceProxyDataChanged(int,QVector<int>)), &m_deviceList, SLOT(dataChanged(int,QVector<int>)));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(netConfigChanged()), this, SLOT(onNetConfigChanged()));//, Qt::QueuedConnection);
        connect(m_studio, SIGNAL(dataChanged(BronStudio::eData)), this, SLOT(onDataChanged(BronStudio::eData)));
        connect(m_studio, SIGNAL(error(QString)), this, SIGNAL(onError(QString)));

        QListIterator<std::shared_ptr<BronItemAPI> > iter(m_deviceList.getListIterator());
        int ix = 0;
        while(iter.hasNext()){
            if(auto p = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
                connect(p.get(), SIGNAL(deviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                connect(p.get(), SIGNAL(deviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                p->setModelIx(ix);
            }
            ix++;
        }
    }
}

QMLInterface::StudioProxy::~StudioProxy(){}

void QMLInterface::StudioProxy::clearElements(){
    m_groupList.clear();
    m_deviceList.clear();
}

QVariant QMLInterface::StudioProxy::getData(int role) const{
    if(m_studio){
        switch(role){
        case NAME:      return QVariant(m_studio->Name());
        case NETWORK:   return QVariant(m_studio->Ssid());
        case PASSWORD:  return QVariant(m_studio->Password());
//        case STUDIO:    return QVariant::fromValue(*this);
        case STUDIO:    return QVariant::fromValue(std::dynamic_pointer_cast<StudioProxy>(std::const_pointer_cast<BronItemAPI>(shared_from_this())));
        case NETWORKSTATE: return QVariant(m_studio->netWorkState());
        case ADDRESS:   return QVariant(m_studio->getAddress());
        case GROUPS:    return QVariant::fromValue<BronListModel *>(const_cast<BronListModel *>(&m_groupList));
        case DEVICES:   return QVariant::fromValue<BronListModel *>(const_cast<BronListModel *>(&m_deviceList));
        case ONLINE:    return QVariant(m_studio->getOnline());
        case AWARENESS: return QVariant(m_studio->getDeviceAwareness());
//        case INCPOWER:
//        case DECPOWER:  return QVariant();
        case SWITHMODLIGHT: return QVariant(m_studio->getSwitchModlight());
        case SWITCHSTANDBY: return QVariant(m_studio->getStandby());
        case SWITCHCOGNILIGHT: return QVariant(m_studio->getCogniLight());
//        case TEST: return QVariant();
        case MINPOWER:
            const_cast<StudioProxy *>(this)->checkPowerLimitsWOSignals();
            return QVariant(m_minPower);
        case MAXPOWER:
            const_cast<StudioProxy *>(this)->checkPowerLimitsWOSignals();
            return QVariant(m_maxPower);
        case MAXMINPOSSIBLEENERGY: return QVariant(m_studio->getMinStudioPower());
        case MINMAXPOSSIBLEENERGY: return QVariant(m_studio->getMaxStudioPower());
        case SPEED: return QVariant(m_studio->getSwitchSpeed());
        case AUDIO: return QVariant(m_studio->getAudio());
        case APPLICATION: return m_studio->getApplication();
        case COGNITION: return false ;//m_studio->getShowCogni();
        case COGNITION_ENABLE: return m_studio->getCogniEnable();
        case SWITCH_REMOTE: return m_studio->getRemoteSwitch();

        }
    }
    return QVariant();
}

bool QMLInterface::StudioProxy::setData(int role, const QVariant &v){
//    QMutexLocker locker(&m_mutex);
    if(m_studio){
        switch(role){
        case NAME:      return false;//return m_studio->setName(v.toString());
        case NETWORK:   return m_studio->setSsid(v.toString());
        case PASSWORD:  return m_studio->setPassword(v.toString());
        case STUDIO:    return false; // no way to set from QML
        case NETWORKSTATE: return false;
        case ADDRESS:   return m_studio->setAddress(v.toInt());
        case GROUPS:     return false; // no way to set from QML, but to force update
        case DEVICES:    return false; // no way to set form QML
        case ONLINE:     return StudioMgr::getInstance()->activateStudio(v.toBool(), m_studio);
        case AWARENESS:  m_studio->setDeviceAwareness(v.toBool()); return true;
        case INCPOWER:   return m_studio->setIncPower(v.toDouble());
        case DECPOWER:   return m_studio->setDecPower(v.toDouble());
        case SWITHMODLIGHT: return m_studio->setSwitchModlight(static_cast<int>(v.toBool()));
        case SWITCHSTANDBY: return m_studio->setStandby(static_cast<int>(v.toBool()));
        case SWITCHCOGNILIGHT: return m_studio->setCogniLight(v.toInt());
        case TEST: return m_studio->setTest(v.toBool());
        case MINPOWER:
        case MAXPOWER:
        case MAXMINPOSSIBLEENERGY:
        case MINMAXPOSSIBLEENERGY: return false;
        case SPEED: return m_studio->setSwitchSpeed(v.toInt());
        case AUDIO: return m_studio->setAudio(v.toBool());
        case APPLICATION: return m_studio->setApplication(incubateDevices(v.toMap()));
//        case COGNITION: return m_studio->setShowCogni();
        case COGNITION_ENABLE: return m_studio->setCogniEnable(v.toBool());
        case SWITCH_REMOTE: return m_studio->setRemoteSwitch(v.toInt());
        case WINK: return m_studio->wink();
        }
    }
    return false;
}

QHash<int, QByteArray> QMLInterface::StudioProxy::roleNames() const{
    static const QHash<int, QByteArray> roles(std::initializer_list<std::pair<int, QByteArray> >({
    {NAME, "name"},
    {NETWORK, "network"},
    {SSID, "ssid"},
    {PASSWORD, "password"},
    {NETWORKSTATE, "networkState"},
    {STUDIO, "studio"},
    {ADDRESS, "address"},
    {GROUPS, "groups"},
    {DEVICES, "devices"},
    {ONLINE, "online"},
    {AWARENESS, "awareness"},
    {INCPOWER, "increasePower"},
    {DECPOWER, "decreasePower"},
    {SWITHMODLIGHT, "switchModLight"},
    {SWITCHSTANDBY, "switchStandby"},
    {SWITCHCOGNILIGHT, "switchCogniLight"},
    {TEST, "testFlash"},
    {MINPOWER, "minPower"},
    {MAXPOWER, "maxPower"},
    {MAXMINPOSSIBLEENERGY, "maxMinPossibleEnergy"},
    {MINMAXPOSSIBLEENERGY, "minMaxPossibleEnergy"},
    {SPEED, "speed"},
    {AUDIO, "audio"},
    {APPLICATION, "application"},
    {COGNITION, "showCogni"},
    {COGNITION_ENABLE, "cogniEnable"},
    {SWITCH_REMOTE, "switchRemote"},
    {WINK, "wink"}}));

    return roles;
}

void QMLInterface::StudioProxy::checkPowerLimits(){
//    QMutexLocker locker(&m_mutex);
    double min = std::numeric_limits<double>::max();
    double max = 0.0;
    QListIterator<std::shared_ptr<BronItemAPI> >iter(m_deviceList.getListIterator());
    while(iter.hasNext()){
        if(auto dp = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
            if(auto dev = dp->getDevice().lock()){
                if(!dev->isAvailable())continue;
                double act = static_cast<double>(dev->getApertureAct());
                if(act < min){
                    min = act;
                }
                if(act > max){
                    max = act;
                }
            }
        }
    }

    QVector<int>v;

    if(m_minPower != min){
        m_minPower = min;
        v.append(StudioProxy::MINPOWER);
    }
    if(m_maxPower != max){
        m_maxPower = max;
        v.append(StudioProxy::MAXPOWER);
    }
    double maxMinPower = m_studio->getMinStudioPower();
    if(m_maxMinPower != maxMinPower){
        m_maxMinPower = maxMinPower;
        v.append(StudioProxy::MAXMINPOSSIBLEENERGY);
    }
    double minMaxPower = m_studio->getMaxStudioPower();
    if(m_minMaxPower != minMaxPower){
        m_minMaxPower = minMaxPower;
        v.append(StudioProxy::MINMAXPOSSIBLEENERGY);
    }

    if(v.size() > 0){
        emit studioProxyDataChanged(getModelIx(), v);
    }

}

void QMLInterface::StudioProxy::checkPowerLimitsWOSignals(){
//    QMutexLocker locker(&m_mutex);
    double min = std::numeric_limits<double>::max();
    double max = 0.0;
    QListIterator<std::shared_ptr<BronItemAPI> >iter(m_deviceList.getListIterator());
    while(iter.hasNext()){
        if(auto dp = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
            if(auto dev = dp->getDevice().lock()){
                if(!dev->isAvailable()) continue;
                double act = static_cast<double>(dev->getApertureAct());
                if(act < min){
                    min = act;
                }
                if(act > max){
                    max = act;
                }

            }
        }
    }


    if(m_minPower != min){
        m_minPower = min;
    }
    if(m_maxPower != max){
        m_maxPower = max;
    }
    double maxMinPower = m_studio->getMinStudioPower();
    if(m_maxMinPower != maxMinPower){
        m_maxMinPower = maxMinPower;
    }
    double minMaxPower = m_studio->getMaxStudioPower();
    if(m_minMaxPower != minMaxPower){
        m_minMaxPower = minMaxPower;
    }
}


std::shared_ptr<QMLInterface::DeviceProxy> QMLInterface::StudioProxy::getProxyOfDevice(std::weak_ptr<Device> p) const{
//    QMutexLocker locker(const_cast<QMutex *>(&m_mutex));
    if(auto pshared = p.lock()){
        QListIterator<std::shared_ptr<BronItemAPI> >iter(m_deviceList.getListIterator());
        while(iter.hasNext()){
            if(auto dp = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
                if(*(dp->getDevice().lock()) == *pshared){
                    return dp;
                }
            }
        }
    }
    return nullptr;
}

QVariantMap QMLInterface::StudioProxy::incubateDevices(QVariantMap val){
    QVariantMap ret;
    QMapIterator<QString, QVariant> iter(val);
    while(iter.hasNext()){
        iter.next();
        if(iter.key() == "deviceValues"){
            QVariantList retList;
            QVariantList list  = iter.value().toList();
            QListIterator<QVariant> liter(list);
            while(liter.hasNext()){
                QVariant data = liter.next();
                QMap<QString,QVariant> obj = data.toMap();
                int index = obj.value("index").toInt();
//                auto item = m_deviceList.getItem(index);
//                DeviceProxy *dp = dynamic_cast<DeviceProxy *>(item);
                if(auto dp = std::dynamic_pointer_cast<DeviceProxy>(m_deviceList.getItem(index))){
                    obj.insert("devicePtr", QVariant::fromValue(dp->getDevice()));
                }
                retList.append(QVariant::fromValue(obj));
            }
            ret.insert(iter.key(), QVariant::fromValue(retList));
        }
        else{
            ret.insert(iter.key(), iter.value());
        }
    }
    return ret;
}

void QMLInterface::StudioProxy::onBronStudioNewDevice(std::weak_ptr<Device> d){
    // check if device is already in studioprxy
    if(auto dev = d.lock()){
        QListIterator<std::shared_ptr<BronItemAPI> > iter(m_deviceList.getListIterator());
        bool isIn = false;
        while(iter.hasNext()){
            if(auto p = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
                if(auto d = p->getDevice().lock()){
                    if(*dev == *d){
                        isIn = true;
                    }
                }
            }
        }
        if(!isIn){
            if(auto dp = std::make_shared<DeviceProxy>(d)){
                int ix = m_deviceList.insert(dp, true, false);
                if(ix != -1){
                    connect(dp.get(), SIGNAL(deviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)));
                    connect(dp.get(), SIGNAL(deviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)));
                    dp->setModelIx(ix);
                }
                checkPowerLimits();
                // all proxy independent if they have a visualisation or not, are held in m_deviceList! every other entity (Groups or Generators)
                // have to use the proxies in this group
                // So dev is a geniLamp [1], insert the actual generated DeviceProxy also at the Generator [2] of the geni Lamp, otherwise the HMI
                // will not reflect Device (DeviceProxy). Doing this way, eases the updating of the HMI
                // be careful, there are two cases: Generator is first in m_deviceList, and second, Lamp is first in m_deviceList
                bool assigned = false;
                // first case: generator was first in
                if(dev->getDeviceType() == Device::eTypes::eGeniLamp){ // [1]
                    auto gl = std::dynamic_pointer_cast<GenLamp>(dev);
                    if(gl){
                        // try to find the generator (deviceProxy) the geniLamp belongs to
                        iter = m_deviceList.getListIterator();
                        while(iter.hasNext()){
                            if(auto geniProxy = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
                                if(auto geni = geniProxy->getDevice().lock()){
                                    if(geni->getDeviceType() == Device::eTypes::eGeni){
                                        if (*geni == *(gl->getScoroDevice())){ // [2]
                                            int ix = geniProxy->getLampList().insert(dp, false, true);
                                            if(-1 != ix){
                                                connect(dp.get(), SIGNAL(deviceProxyUpdateGeniLampListQML(int,QVector<int>)), &geniProxy->getLampList(), SLOT(dataChanged(int,QVector<int>)));//, Qt::QueuedConnection);
                                                dp->setLampIx(ix);
                                                assigned = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if(false == assigned){
                    if(dev->getDeviceType() == Device::eTypes::eGeni){ // [1]
                        auto geni = std::dynamic_pointer_cast<DeviceScoro>(dev);
                        if(geni){
                            iter = m_deviceList.getListIterator();
                            while(iter.hasNext()){
                                if(auto lampProxy = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
                                    if(auto lamp = lampProxy->getDevice().lock()){
                                        if(lamp->getDeviceType() == Device::eTypes::eGeniLamp){
                                            // check, if lamp belongs to this generator
                                            if(geni->hasLamp(std::dynamic_pointer_cast<GenLamp>(lamp))){
                                                int ix = dp->getLampList().insert(lampProxy, false, true);
                                                if(-1 != ix){
                                                    connect(lampProxy.get(), SIGNAL(deviceProxyUpdateGeniLampListQML(int,QVector<int>)), &dp->getLampList(), SLOT(dataChanged(int,QVector<int>)));//, Qt::QueuedConnection);
                                                    dp->setLampIx(ix);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void QMLInterface::StudioProxy::onBronStudioGoneDevice(std::shared_ptr<Device> dev){
    int size = m_deviceList.getSize();
    bool isIn = false;
    // check, if it a geni lamp
    if(dev->getDeviceType() == Device::eTypes::eGeniLamp){
        // get elder (scoro)
        if(auto genLamp = std::dynamic_pointer_cast<GenLamp>(dev)){
            QPointer<DeviceScoro> scoro = genLamp->getScoroDevice();
            if(!scoro.isNull()){
                scoro->removeLamp(genLamp->getNr());
            }
        }
    }

    // check if sub device in scoro first run
    for(int i = 0; i < size; i++){
//        BronItemAPI *it = m_deviceList.getItem(i);
//        DeviceProxy *p = dynamic_cast<DeviceProxy *>(it);
        if(auto p = std::dynamic_pointer_cast<DeviceProxy>(m_deviceList.getItem(i))){
            if(auto d = p->getDevice().lock()){
                if(d->getDeviceType() == Device::eTypes::eGeni){
                    // is scoro, remove all lamps form this scoro
                    BronListModel &lampList = p->getLampList();
                    int lampListSize = lampList.getSize();
                    for(int j = 0; j < lampListSize; j++){
//                        BronItemAPI *lampItem = lampList.getItem(j);
//                        DeviceProxy *lampProxy = dynamic_cast<DeviceProxy *>(lampItem);
                        if(auto lampProxy = std::dynamic_pointer_cast<DeviceProxy>(lampList.getItem(j))){
                            if(auto alamp = lampProxy->getDevice().lock()){
                                if(alamp->getHiddenId() == dev->getHiddenId()){
                                    lampList.removeItemAt(j,false,true);
                                    isIn = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(isIn){
            break;
        }
    }

    isIn = false;
    int i = 0;
    for(i = 0; i < size; i++){
//        BronItemAPI *it = m_deviceList.getItem(i);
//        DeviceProxy *p = dynamic_cast<DeviceProxy *>(it);
        if(auto p = std::dynamic_pointer_cast<DeviceProxy>(m_deviceList.getItem(i))){
            if(auto aDev = p->getDevice().lock()){
                if(dev->getHiddenId() == aDev->getHiddenId()){
                    isIn = true;
                    break;
                }
            }
        }
    }
    if(isIn){
        if(auto dp = std::dynamic_pointer_cast<DeviceProxy>(m_deviceList.removeItemAt(i,true,false))){
            disconnect(dp.get(), SIGNAL(deviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)));
            disconnect(dp.get(), SIGNAL(deviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(int,std::weak_ptr<DeviceProxy>)));
        }
    }
}

void QMLInterface::StudioProxy::onBronStudioGroupAdded(BronGroup *group){
    bool isIn = false;
    QListIterator<std::shared_ptr<BronItemAPI> > iter = m_groupList.getListIterator();
    while(iter.hasNext()){
        if(auto p = std::dynamic_pointer_cast<BronGroupProxy>(iter.next())){
            if(group->getAddress() == p->getGroup()->getAddress()){
                isIn = true;
                break;
            }
        }
    }
    if(!isIn){
        if(auto gp = std::make_shared<BronGroupProxy>(this, group)){
            int ix = m_groupList.insert(gp);
            if(ix != -1){
                connect(gp.get(), SIGNAL(elementChanged(int)), &m_groupList, SLOT(onElementChanged(int)));
                connect(gp.get(), SIGNAL(groupProxyDataChanged(int, QVector<int>)), &m_groupList,  SLOT(dataChanged(int,QVector<int>)));//, Qt::QueuedConnection);
                gp->setModelIx(ix);
            }
        }
    }
}

void QMLInterface::StudioProxy::onBronStudioGroupRemoved(BronGroup *group){
    QListIterator<std::shared_ptr<BronItemAPI> > iter = m_groupList.getListIterator();
    bool isIn = false;
    int i = 0;
    while(iter.hasNext()){
        if(auto p = std::dynamic_pointer_cast<BronGroupProxy>(iter.next())){
            if(group == p->getGroup()){
                isIn = true;
                break;
            }
        }
        i++;
    }
    if(isIn){
        if(auto gp = std::dynamic_pointer_cast<BronGroupProxy>(m_groupList.removeItemAt(i,true, false))){
            disconnect(gp.get(), SIGNAL(elementChanged(int)), &m_groupList, SLOT(onElementChanged(int)));
            disconnect(gp.get(), SIGNAL(groupProxyDataChanged(int, QVector<int>)), &m_groupList,  SLOT(dataChanged(int,QVector<int>)));
        }
    }
}

void QMLInterface::StudioProxy::onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy> p){
    if(auto dp = p.lock()){
        static const QVector<int> paras(std::initializer_list<int>({
                                           DeviceProxy::ONLINE,
                                           DeviceProxy::PARAS,
                                           DeviceProxy::LOCAL_COGNI,
                                           DeviceProxy::MIN_APERTURE,
                                           DeviceProxy::MAX_APERTURE,
                                           DeviceProxy::FULL_MODEL_NAME
                                       }));
            emit studioProxyDeviceProxyDataChanged(dp->getModelIx(), paras);
    }
}

void QMLInterface::StudioProxy::onDeviceProxyUpdateQML(int role, std::weak_ptr<DeviceProxy> p){
    if(auto dp = p.lock()){
        // do here the group management also if role => Paras::Id::LAMP_ADDRESS
        if(role == DeviceProxy::LAMP_ADDRESS){
            QVector<int>paras(std::initializer_list<int>({role, DeviceProxy::LOCAL_COGNI}));
            emit studioProxyDeviceProxyDataChanged(dp->getModelIx(), paras);
            return;
        }
        else if(role == DeviceProxy::ACT_APERTURE){
            emit studioProxyDeviceProxyDataChanged(dp->getModelIx(), QVector<int>(1, DeviceProxy::ACT_ENERGY));
            checkPowerLimits();
        }
        else if(role == DeviceProxy::FLASH_COUNT){
            emit studioProxyDataChanged(getModelIx(), QVector<int> (1, StudioProxy::FLASH_COUNT));
        }
        else if(role == DeviceProxy::MODLIGHT){
            emit studioProxyDataChanged(getModelIx(), QVector<int> (1, StudioProxy::SWITHMODLIGHT));
        }
        else if(role == DeviceProxy::STANDBY){
            emit studioProxyDataChanged(getModelIx(), QVector<int> (1, StudioProxy::SWITCHSTANDBY));
        }
        else if(role == DeviceProxy::STUDIO_ADDRESS){
            if(m_studio){
                if(m_studio->deleteDevice(dp->getDevice())){
    //                dp->deleteLater();
                }
                return;
            }
        }
        else if(role == DeviceProxy::SPEED || role == DeviceProxy::F_STOP_MAX || role == DeviceProxy::F_STOP_RANGE){
            static const QVector<int> paras(std::initializer_list<int>({  StudioProxy::SPEED,
                                                                          StudioProxy::MINPOWER,
                                                                          StudioProxy::MAXPOWER,
                                                                          StudioProxy::MINMAXPOSSIBLEENERGY,
                                                                          StudioProxy::MAXMINPOSSIBLEENERGY }));
            emit studioProxyDataChanged(getModelIx(), paras);
        }
        else if(role == DeviceProxy::AUDIO){
            emit studioProxyDataChanged(getModelIx(), QVector<int> (1, StudioProxy::AUDIO));
        }
        else if(role == DeviceProxy::COGNI_COL_ENA){
            emit studioProxyDataChanged(getModelIx(), QVector<int>(1, StudioProxy::SWITCHCOGNILIGHT));
        }
        else if(role == DeviceProxy::eRoles::REMOTE_SWITCH || role == DeviceProxy::eRoles::CELL || role == DeviceProxy::eRoles::IR_RFS){
            emit studioProxyDataChanged(getModelIx(), QVector<int>(1, StudioProxy::SWITCH_REMOTE));
        }
        emit studioProxyDeviceProxyDataChanged(dp->getModelIx(), QVector<int>(1,role));
    }
}

void QMLInterface::StudioProxy::onNetConfigChanged(){
    static const QVector<int> paras(std::initializer_list<int>({
                                       StudioProxy::ONLINE,
                                       StudioProxy::NETWORKSTATE}));
    emit studioProxyDataChanged(getModelIx(), paras);
}

void QMLInterface::StudioProxy::onDataChanged(BronStudio::eData data){
    if(data == BronStudio::eData::eAddress){
        emit studioProxyDataChanged(getModelIx(), QVector<int>(1, StudioProxy::ADDRESS));
    }
    else if(data == BronStudio::eData::eSpecialModes){
        emit studioProxyDataChanged(getModelIx(), QVector<int>(1, StudioProxy::APPLICATION));
    }
}
