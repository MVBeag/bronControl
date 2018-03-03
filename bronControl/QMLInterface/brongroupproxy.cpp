#include "brongroupproxy.h"
#include "brongroup.h"
#include <QDebug>
#include <QQmlEngine>
#include "studioproxy.h"

QMLInterface::BronGroupProxy::BronGroupProxy(StudioProxy *sp, BronGroup *group)
    : BronItemAPI()
    , m_group(group)
    , m_deviceAPIPrototype()
    , m_deviceList("groupDeviceList", &m_deviceAPIPrototype)
    , m_minPower(std::numeric_limits<double>::max())
    , m_maxPower(0.0)
    , m_maxMinPower(0.0)
    , m_minMaxPower(10.0)
    , m_studioProxy(sp){
    init();
}

QMLInterface::BronGroupProxy::BronGroupProxy(const QMLInterface::BronGroupProxy &p)
    : BronItemAPI()
    , m_group(p.m_group)
    , m_deviceAPIPrototype()
    , m_deviceList("groupDeviceList", &m_deviceAPIPrototype)
    , m_minPower(std::numeric_limits<double>::max())
    , m_maxPower(0.0)
    , m_maxMinPower(0.0)
    , m_minMaxPower(10.0)
    , m_studioProxy(p.m_studioProxy){
    init();
}

void QMLInterface::BronGroupProxy::init(){
    // elements are deleted in devicelist
    if(m_group){
        connect(m_group, SIGNAL(deviceAdded(std::weak_ptr<Device>)), this, SLOT(onDeviceAdded(std::weak_ptr<Device> )));//, Qt::QueuedConnection);
        connect(m_group, SIGNAL(deviceRemoved(std::weak_ptr<Device>)), this, SLOT(onDeviceRemoved(std::weak_ptr<Device> )));//, Qt::QueuedConnection);

        QListIterator<std::weak_ptr<Device> > iter(m_group->getDevices());
        while(iter.hasNext()){
            if(m_studioProxy){
                if(auto dp = m_studioProxy->getProxyOfDevice(iter.next())){
                    // don't change the indexes of the device proxies, the visualisation uses
                    // not the groupProxys, it uses the deviceList!!
                    if(m_deviceList.insert(dp, false, false) != -1){
                        connect(dp.get(), SIGNAL(deviceProxyUpdateQML(int, std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(int, std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                        connect(dp.get(), SIGNAL(deviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                        // don't set the index of device proxy here, the shown devices on hmi are not
                        // from group proxy
                    }
                }
            }
            else{
                break;
            }
        }
    }
}



QMLInterface::BronGroupProxy::~BronGroupProxy(){
    m_deviceList.clear();
}

QVariant QMLInterface::BronGroupProxy::getData(int role) const{
    if(m_group){
        switch(role){
        // we can return the contents of the first element, all elements have the same address, power, ...!
        case ADDRESS:       return QVariant(m_group->getAddress());
        case INCPOWER:
        case DECPOWER:   return QVariant();
        case DEVICES:       return QVariant::fromValue<BronListModel *>(const_cast<BronListModel *>(&m_deviceList));
        case COGNICOLOR:    return QVariant((m_group->getAddress() - 1) % static_cast<int>(ParaSelects::CognitionLightColor::NONE));//return QVariant(static_cast<int>(m_group->GetColor()));
        case MINPOWER:
            const_cast<BronGroupProxy *>(this)->checkPowerLimitsWOSignals();
            return QVariant(m_minPower);
        case MAXPOWER:
            const_cast<BronGroupProxy *>(this)->checkPowerLimitsWOSignals();
            return QVariant(m_maxPower);
        case MAXMINPOSSIBLEENERGY: return QVariant(m_group->getMinGroupPower());
        case MINMAXPOSSIBLEENERGY: return QVariant(m_group->getMaxGroupPower());
        case STANDBY: return QVariant(m_group->getStandby());
        case COGNITION_ENABLE: return QVariant(m_group->getCogniEnabled());
        default: return QVariant();
        }
    }else{
        return QVariant();
    }
}

bool QMLInterface::BronGroupProxy::setData(int role, const QVariant &val){
    if(m_group){
        switch(role){
//        case ADDRESS:  return; // nothing to do, addresses can't be set within a group
        case INCPOWER: return m_group->setIncEnergy(val.toDouble());
        case DECPOWER: return m_group->setDecEnergy(val.toDouble());
        case STANDBY: return m_group->setStandby(val.toBool());
//        case COGNITION: return m_group->setShowCogni();
        case COGNITION_ENABLE: return m_group->setCogniEnabled(val.toBool());
        case WINK: return m_group->wink();
//        case COGNICOLOR: return;
//        case MINPOWER:
//        case MAXPOWER:
//        case MINMAXPOSSIBLEENERGY:
//        case MAXMINPOSSIBLEENERGY:
        default: return false;
        }
    }
    return false;
}

QHash<int, QByteArray> QMLInterface::BronGroupProxy::roleNames() const{
    static const QHash<int, QByteArray> roles(std::initializer_list<std::pair<int, QByteArray> >({
    {ADDRESS, "address"},
    {DEVICES, "devices"},
    {COGNICOLOR, "cogniColor"},
    {MINPOWER, "minPower"},
    {MAXPOWER, "maxPower"},
    {MAXMINPOSSIBLEENERGY, "maxMinPossibleEnergy"},
    {MINMAXPOSSIBLEENERGY, "minMaxPossibleEnergy"},
    {INCPOWER, "increasePower"},
    {DECPOWER, "decreasePower"},
    {STANDBY, "standby"},
    {COGNITION, "showCogni"},
    {COGNITION_ENABLE, "cogniEnable"},
    {WINK, "wink"}}));
    return roles;
}

void QMLInterface::BronGroupProxy::onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy> dp){
    Q_UNUSED(dp);
//    static const QVector<int> paras(std::initializer_list<int>({
//                                       DeviceProxy::ONLINE,
//                                       DeviceProxy::PARAS,
//                                       DeviceProxy::COM_STATE,
//                                       DeviceProxy::LOCAL_COGNI,
//                                       DeviceProxy::MIN_ENERGY,
//                                       DeviceProxy::MAX_ENERGY,
//                                       DeviceProxy::FULL_MODEL_NAME
//                                   }));
//        emit groupProxyDataChanged(getModelIx(), paras);
}

void QMLInterface::BronGroupProxy::onDeviceProxyUpdateQML(int role, std::weak_ptr<DeviceProxy> dp){
    Q_UNUSED(dp);
    // do here the group management also if role => Paras::Id::LAMP_ADDRESS
    if(role == DeviceProxy::LAMP_ADDRESS){
        QVector<int>paras(std::initializer_list<int>({BronGroupProxy::ADDRESS,
                                                      BronGroupProxy::COGNICOLOR}));
        emit groupProxyDataChanged(getModelIx(), paras);
        return;
    }
    else if(role == DeviceProxy::ACT_APERTURE){
        checkPowerLimits();
    }
}

void QMLInterface::BronGroupProxy::onDeviceAdded(std::weak_ptr<Device> dev){
//    qDebug()  << "groupProxy::onDeviceAdded " << dev->getName() << " address " << dev->getLampAddress();
    if(m_studioProxy){
            if(auto dp = m_studioProxy->getProxyOfDevice(dev)){
    //            qDebug()  << "groupProxy::onDeviceAdded proxy found";
                int insertix = m_deviceList.insert(dp, false, false);
                if(-1 != insertix){
                    emit elementChanged(insertix);
    //                qDebug() << "groupProxy::onDeviceAdded ok " << m_deviceList.getSize();
                    // don't use index of DeviceProxy here, the shown devices on hmi are build from the groups
                    connect(dp.get(), SIGNAL(deviceProxyUpdateQML(int, std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(int, std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                    connect(dp.get(), SIGNAL(deviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)));//, Qt::QueuedConnection);
                }
            }
    }
}

void QMLInterface::BronGroupProxy::onDeviceRemoved(std::weak_ptr<Device> d){
//    qDebug()  << "groupProxy::onDeviceRemoved " << dev->getName() << " address " << dev->getLampAddress();
    if(m_studioProxy){
        if(auto dp = m_studioProxy->getProxyOfDevice(d)){
//            qDebug()  << "groupProxy::onDeviceRemoved proxy found";
            if(m_deviceList.contains(dp)){
                int removedIx = m_deviceList.remove(dp, false, false);
                if(-1 != removedIx){
                    emit elementChanged(removedIx);
                }
//                qDebug()  << "groupProxy::onDeviceRemoved ok " << m_deviceList.getSize();
                disconnect(dp.get(), SIGNAL(deviceProxyUpdateQML(int, std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(int, std::weak_ptr<DeviceProxy>)));
                disconnect(dp.get(), SIGNAL(deviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)), this, SLOT(onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy>)));

            }
        }
    }
}


void QMLInterface::BronGroupProxy::checkPowerLimits(){
    double min = std::numeric_limits<double>::max();
    double max = 0.0;
    QListIterator<std::shared_ptr<BronItemAPI> >iter(m_deviceList.getListIterator());
    while(iter.hasNext()){
        if(auto dp = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
            if(auto dev = dp->getDevice().lock()){
                if(!dev->isAvailable()) continue;
                double act = dev->getApertureAct();
                if(act < min){
                    min = act;
                }
                if(act > max){
                    max = act;
                }
            }
        }
    }

    QVector<int> v;

    if(m_minPower != min){
        m_minPower = min;
        v.append(BronGroupProxy::MINPOWER);
    }
    if(m_maxPower != max){
        m_maxPower = max;
        v.append(BronGroupProxy::MAXPOWER);
    }
    double maxMinPower = m_group->getMinGroupPower();
    if(m_maxMinPower != maxMinPower){
        m_maxMinPower = maxMinPower;
        v.append(BronGroupProxy::MAXMINPOSSIBLEENERGY);
    }
    double minMaxPower = m_group->getMaxGroupPower();
    if(m_minMaxPower != minMaxPower){
        m_minMaxPower = minMaxPower;
        v.append(BronGroupProxy::MINMAXPOSSIBLEENERGY);
    }

    if(v.size()>0){
        emit groupProxyDataChanged(getModelIx(), v);
    }
}
void QMLInterface::BronGroupProxy::checkPowerLimitsWOSignals(){
    double min = std::numeric_limits<double>::max();
    double max = 0.0;
    QListIterator<std::shared_ptr<BronItemAPI> >iter(m_deviceList.getListIterator());
    while(iter.hasNext()){
        if(auto dp = std::dynamic_pointer_cast<DeviceProxy>(iter.next())){
            if(auto dev = dp->getDevice().lock()){
                if(!dev->isAvailable()) continue;
                double act = dev->getApertureAct();
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
    double maxMinPower = m_group->getMinGroupPower();
    if(m_maxMinPower != maxMinPower){
        m_maxMinPower = maxMinPower;
    }
    double minMaxPower = m_group->getMaxGroupPower();
    if(m_minMaxPower != minMaxPower){
        m_minMaxPower = minMaxPower;
    }
}
