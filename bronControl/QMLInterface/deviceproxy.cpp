#include "deviceproxy.h"
#include <QQmlEngine>
#include "paramappings.h"
#include "devicescoro.h"
#include "genLamp.h"
#include "studioproxy.h"

using namespace Framework;

static QMLInterface::DeviceProxy devProxy;


QMLInterface::DeviceProxy::DeviceProxy(std::weak_ptr<Device> dev)
    : BronItemAPI()
    , m_p(dev)
    , m_ItemAPIPrototype()
    , m_paraList("ParaList", &m_ItemAPIPrototype)
    , m_lampList("LampList", &devProxy)
{
    if(auto d = m_p.lock()){
        // fill QML list model with data
        auto paras = d->GetAllParas();
        QList<std::shared_ptr<DevicePara> >::const_iterator iter = paras.constBegin();
        QList<std::shared_ptr<DevicePara> >::const_iterator end = paras.constEnd();
        for (; iter != end; ++iter) {
            auto pp = std::make_shared<ParaProxy>(*iter);
            if(pp){
                connect(pp.get(), SIGNAL(paraProxyDataChanged(std::weak_ptr<ParaProxy>)), this, SLOT(onParaProxyDataChanged(std::weak_ptr<ParaProxy>)));
                pp->setRole(getRoleForPara((*iter)->Id()));
                int ix = m_paraList.insert(pp);
                if(ix != -1){
                    pp->setModelIx(ix);
                }
            }
        }
    }
}

QMLInterface::DeviceProxy::DeviceProxy(const QMLInterface::DeviceProxy &p)
    : BronItemAPI()
    , m_p(p.m_p)
    , m_ItemAPIPrototype()
    , m_paraList(p.m_paraList)
    , m_lampList(p.m_lampList){
}

QMLInterface::DeviceProxy::DeviceProxy()
    : BronItemAPI()
    , m_p()
    , m_ItemAPIPrototype()
    , m_paraList("ParaList", &m_ItemAPIPrototype)
    , m_lampList("LampList", &devProxy){
}



QMLInterface::DeviceProxy::~DeviceProxy(){
//    m_paraList.clear();
}

QVariant QMLInterface::DeviceProxy::getData(int role) const{
    if(auto d = m_p.lock()){
        struct{
            int i;
            float f;
        }ret = {-1, -1.0f};
//        float ret = -1.0f;
        switch(role){
        case ONLINE:            ret.i = d->IsOnline(); break;
        case TYPE:              ret.i = d->getDeviceType(); break;
        case PARAS:             return QVariant::fromValue<BronListModel *>(const_cast<BronListModel *>(&m_paraList));
        case LOCAL_COGNI:
            ret.i = d->getLampAddress();
            if(-1 != ret.i){
              ret.i = ((ret.i - 1) % static_cast<int>(ParaSelects::CognitionLightColor::NONE));
            }
            break;
        case MIN_APERTURE:        return QVariant(d->getApertureMin());
        case MAX_APERTURE:        return QVariant(d->getApertureMax());
        case FULL_MODEL_NAME:   return QVariant(d->getName());
        case UPDATING:          return QVariant(false);
        case INCPOWER:
        case DECPOWER:          break;
        case LAMPS:             return QVariant::fromValue<BronListModel *>(const_cast<BronListModel *>(&m_lampList));
        case LAMP_CHANNEL:      ret.i = d->getChannel(); break;
        case ACT_ENERGY:        return QVariant(d->getActEnergy());
        case NAME:              return QVariant(d->getRemoteName());
        case LAMP_ADDRESS:      ret.i = d->getLampAddress(); break;
        case STANDBY:           ret.i = d->getStandby(); break;
        case DELAY:             ret.f = d->getDelay(); break;
        case MODLIGHT:          ret.i = d->getModLight(); break;
        case MODLIGHTMODE:      ret.i = d->getModLightMode(); break;
        case STUDIO_ADDRESS:    ret.i = d->getStudioAddress(); break;
        case COGNI_COL_ENA:     ret.i = d->getCogniEnable(); break;
        case SW_VERSION:        ret.i = d->getSWVersion(); break;
        case ALARM:             return QVariant(static_cast<int>(d->getAlarm()));
        case MAX_ENERGY:        ret.f = d->getMaxEnergy(); break;
        case F_STOP_RANGE:      ret.f = d->getFStopRange(); break;
        case F_STOP_MAX:        ret.f = d->getApertureMax(); break;
        case SPEED:             ret.i = static_cast<int>(d->getSpeed()); break;
        case AUDIO:             ret.i = d->getAudio(); break;
        case SEQUENCE:          ret.i = d->getSequence(); break;
        case SYN_SEQUENCE:      ret.i = d->getSynSequence(); break;
        case PAUSE_TIME:        ret.f = d->getPauseTime(); break;
        case FLASH_TIME_SETTING:ret.f = d->getFlashTimeSetting(); break;
        case FLASH_DURATION:    ret.i = d->getFlashDuration(); break;
        case SLOW_CHARGE:       ret.i = d->getSlowCharge(); break;
        case TEST:              return false;
        case ACT_APERTURE:      return QVariant(d->getApertureAct());
        case APPLICATION:       ret.i = d->getApplication(); break;
        case MASKGROUP:         ret.i = d->getMaskGroup(); break;
        case NUM_LAMPS:         ret.i = d->getNumLamps(); break;
        case COLOR_TEMP:        ret.i = d->getColorTemp(); break;
        case DIMMER:            ret.i = d->getDimmer(); break;
        case CELL_SENSITIVITY:  ret.i = d->getCellSensitivity(); break;
        case CELL:              ret.i = d->getCell(); break;
        case IR_RFS:            ret.i = d->getIR_RFS(); break;
        case BRIGHT_TEST:       ret.i = d->getBrightnessTest(); break;
        case AUDIO_MODE:        ret.i = d->getRdySoundMode(); break;
//        case KEY_SOUND_VOL:     ret.i = m_p->getKeySoundVol(); break;
        case DISP_INTENSITY:    ret.i = d->getDisplayIntensity(); break;
        case DAILY_COUNTER:     ret.i = d->getDailyCounter(); break;
        case OVERALL_COUNTER:   ret.i = d->getOverallCounter(); break;
        case REMOTE_SWITCH:     ret.i = d->getRemoteControl(); break;


//        case JOIN_NETWORK:      return false;
        case WLAN_SSID:         return QVariant(d->getSSID());
        case WLAN_PWD:          return QVariant(d->getPWD());
        case WLAN_ENC:          return QVariant(d->getEncryptionMode());
        case ERROR:             return QVariant();
        default: return QVariant();
        }
        if(-1 == ret.i && -1.0f == ret.f){
            return QVariant();
        }
        else{
            if(-1 != ret.i){
                return QVariant(ret.i);
            }
            else{
                return QVariant(ret.f);
            }
        }
    }else{
        return QVariant();
    }
}

bool QMLInterface::DeviceProxy::setData(int role, const QVariant &v){
    if(auto d = m_p.lock()){
        switch(role){
        case NAME:      return d->setName(v.toString());
        case ONLINE:    d->setOnline(false); return true;
        case INCPOWER:  return d->increasePower(v.toFloat());
        case DECPOWER:  return d->decreasePower(v.toFloat());
        case SPEED:     return d->setSpeed(static_cast<ParaSelects::SpeedMode>(v.toInt()));

        case LAMP_ADDRESS:  return d->setLampAddress(v.toInt());
        case STANDBY:       return d->setStandby(v.toInt());
        case DELAY:         return d->setDelay(v.toFloat());
        case MODLIGHT:      return d->setModLight(v.toInt());
        case MODLIGHTMODE:  return d->setModLightMode(v.toInt());
        case PARAS:         return false;
        case COGNI_COL_ACT: return false;
        case STUDIO_ADDRESS: return d->setStudioAddress(v.toInt());
        case COGNI_COL_ENA: return d->setCogniEnable(v.toInt());
        case COGNI_COL:     return false;
//        case DEVICE_DESC:
//        case NUM_OF_LAMPS:
        case SW_VERSION:    return d->triggerSwVersionReadout();
        case ALARM:         return false;
        case MAX_ENERGY:  return false;
        case F_STOP_RANGE:  return false;
        case F_STOP_MAX: return false;
        case AUDIO:         return d->setAudio(v.toInt());
        case SEQUENCE:      return d->setSequence(v.toInt());
        case SYN_SEQUENCE:  return d->setSynSequence(v.toInt());
        case PAUSE_TIME:    return d->setPauseTime(v.toFloat());
        case FLASH_TIME_SETTING: return d->setFlashTimeSetting(v.toInt());
        case FLASH_DURATION:    return false;
        case SLOW_CHARGE:   return d->setSlowCharge(v.toInt());
        case TEST:          return d->setTest();
        case ACT_APERTURE:  return d->setActApertureEnergy(v.toFloat());
        case APPLICATION:return d->setApplication(v.toInt());
        case MASKGROUP:     return d->setMaskGroup(v.toInt());
        case WLAN_SSID:     return d->setSSID(v.toString());
        case WLAN_PWD:      return d->setPWD(v.toString());
        case WLAN_ENC:      return d->setEncryptionMode(v.toInt());
        case TYPE:          return false;
        case JOIN_NETWORK:  return d->setJoinNetwork(v.toInt());
        case COLOR_TEMP:    return d->setColorTemp(v.toInt());
        case DIMMER:        return d->setDimmer(v.toInt());
        case CELL_SENSITIVITY: return d->setCellSensitivity(v.toInt());
        case CELL:          return d->setCell(v.toInt());
        case IR_RFS:        return d->setIR_RFS(v.toInt());
        case BRIGHT_TEST:   return d->setBrightnessTest(v.toInt());
        case AUDIO_MODE:    return d->setRdySoundMode(v.toInt());
//        case KEY_SOUND_VOL: return m_p->setKeySoundVol(v.toInt());
        case DISP_INTENSITY:return d->setDisplayIntensity(v.toInt());
        case RESET_FLASH_COUNT: return d->resetDailyCounter();
        case REMOTE_SWITCH: return d->setRemoteControl(static_cast<ParaSelects::RemoteCtrl>(v.toInt()));
        case WINK: d->startWink(); return true;
        case DAILY_COUNTER:
        case OVERALL_COUNTER:
        case LOCAL_COGNI:
        case MIN_APERTURE:
        case MAX_APERTURE:
        case FULL_MODEL_NAME:
        case LAMPS:
        case LAMP_CHANNEL:
        case NUM_LAMPS:
        case UPDATING:
        case ACT_ENERGY:
        case ERROR: return false;
        default: return false;
        }
    }else{
        return false;
    }
}

QHash<int, QByteArray> QMLInterface::DeviceProxy::roleNames() const{
    static const QHash<int, QByteArray> roles(std::initializer_list<std::pair<int, QByteArray> >({
        {NAME, "name"},
        {TYPE, "type"},
        {ONLINE, "online"},
        {PARAS, "paras"},
        {LOCAL_COGNI, "cogniColor"},
        {MIN_APERTURE, "minEnergy"},
        {MAX_APERTURE, "maxEnergy"},
        {FULL_MODEL_NAME, "fullModelName"},
        {UPDATING, "updating"},
        {INCPOWER, "increasePower"},
        {DECPOWER, "decreasePower"},
        {LAMPS, "lamps"},
        {LAMP_CHANNEL, "lampChannel"},
        {ACT_ENERGY, "apertureJ"},
        {STANDBY, "standby"},
        {DELAY, "delay"},
        {MODLIGHT, "modLight"},
        {MODLIGHTMODE, "modLightMode"},
        {STUDIO_ADDRESS, "studio_address"},
        {LAMP_ADDRESS, "address"},
        {COGNI_COL_ENA, "cogniColEnable"},
        {COGNI_COL_ACT, "cogniColAct"},
        {COGNI_COL, "cogniCol"},
//        {DEVICE_DESC, "descriptor"},
//        {NUM_OF_LAMPS, "numOfLamps"},
        {SW_VERSION, "swVersion"},
        {ALARM, "alarm"},
        {MAX_ENERGY, "maxEnergyJ"},
        {F_STOP_RANGE, "fStopRange"},
        {F_STOP_MAX, "fStopMax"},
        {SPEED, "speed"},
        {AUDIO, "audio"},
        {SEQUENCE, "sequence"},
        {SYN_SEQUENCE, "syncSequence"},
        {PAUSE_TIME, "pauseTime"},
        {FLASH_TIME_SETTING, "flashTimeSetting"},
        {FLASH_DURATION, "flashDuration"},
        {SLOW_CHARGE, "slowCharge"},
        {TEST, "test"},
        {ACT_APERTURE, "energyTotalAperture"},
        {APPLICATION, "application"},
        {MASKGROUP, "maskgroup"},
        {NUM_LAMPS, "numLamps"},
        {COLOR_TEMP, "colorTemp"},
        {DIMMER, "dimmer"},
        {CELL_SENSITIVITY, "cellSens"},
        {CELL, "cell"},
        {IR_RFS, "irRfs"},
        {BRIGHT_TEST, "brightnessTest"},
        {AUDIO_MODE, "audioMode"},
//        {KEY_SOUND_VOL, "keySoundVol"},
        {DISP_INTENSITY, "dispIntensity"},
        {DAILY_COUNTER, "dailyCounter"},
        {OVERALL_COUNTER, "overallCounter"},
        {RESET_FLASH_COUNT, "resetDailyCount"},


        {JOIN_NETWORK, "joinNetwork"},
        {WLAN_SSID, "wlanSsid"},
        {WLAN_PWD, "wlanPwd"},
        {WLAN_ENC, "wlanEnc"},
        {ERROR, "error"},
        {REMOTE_SWITCH, "remoteSwitch"},
        {WINK, "wink"}}));

    return roles;
}

QHostAddress QMLInterface::DeviceProxy::HostAddress() const{
    if(auto dev = m_p.lock()){
        return dev->getHostAddress();
    }
    return QHostAddress();
}

int QMLInterface::DeviceProxy::getRoleForPara(int id){
    if(auto dev = m_p.lock()){
        const QHash<int, int>& hash = ParaMappings::getDeviceMappings(dev);
        int role = hash.value(id);
        if(role == 0){ // default constructed value for int
            return -1;
        }
        else{
            return role;
        }
    }
    return -1;
}

void QMLInterface::DeviceProxy::onParaProxyDataChanged(std::weak_ptr<ParaProxy> ppx){
    if(auto pp = ppx.lock()){
        int role = pp->getRole();
        if(role != -1){
            emit deviceProxyUpdateQML(role, std::dynamic_pointer_cast<DeviceProxy>(shared_from_this()));
            if(role == DeviceProxy::CELL || role == DeviceProxy::IR_RFS){
                emit deviceProxyUpdateQML(DeviceProxy::REMOTE_SWITCH, std::dynamic_pointer_cast<DeviceProxy>(shared_from_this()));
            }
        }
        else{
            emit deviceProxyUpdateQML(std::dynamic_pointer_cast<DeviceProxy>(shared_from_this()));
        }

        if(auto dev = m_p.lock()){
            if(dev->getDeviceType() == Device::eTypes::eGeniLamp){
                if(role != -1){
                    if(role == DeviceProxy::LAMP_ADDRESS){
                        static const QVector<int> paras(std::initializer_list<int>({
                                                           DeviceProxy::LOCAL_COGNI,
                                                           DeviceProxy::LAMP_ADDRESS
                                                       }));
                        emit deviceProxyUpdateGeniLampListQML(getLampIx(), paras);
                    }
                    else{
                        emit deviceProxyUpdateGeniLampListQML(getLampIx(), QVector<int>(1,role));
                    }
                }
                else{
                    static const QVector<int> paras(std::initializer_list<int>({
                                                       DeviceProxy::ONLINE,
                                                       DeviceProxy::PARAS,
                                                       DeviceProxy::LOCAL_COGNI,
                                                       DeviceProxy::MIN_APERTURE,
                                                       DeviceProxy::MAX_APERTURE,
                                                       DeviceProxy::FULL_MODEL_NAME
                                                   }));
                    emit deviceProxyUpdateGeniLampListQML(getLampIx(), paras);
                }
            }
        }
    }
}


void QMLInterface::DeviceProxy::onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy> dp){
    if(auto proxy = dp.lock()){
        static const QVector<int> paras(std::initializer_list<int>({
                                           DeviceProxy::ONLINE,
                                           DeviceProxy::PARAS,
                                           DeviceProxy::LOCAL_COGNI,
                                           DeviceProxy::MIN_APERTURE,
                                           DeviceProxy::MAX_APERTURE,
                                           DeviceProxy::FULL_MODEL_NAME
                                       }));
        emit deviceProxyDeviceProxyDataChanged(proxy->getModelIx(), paras);
    }
}

void QMLInterface::DeviceProxy::onDeviceProxyUpdateQML(int role, std::weak_ptr<DeviceProxy> dp){
    if(auto px = dp.lock()){
        // do here the group management also if role => Paras::Id::LAMP_ADDRESS
        if(role == DeviceProxy::LAMP_ADDRESS){
            QVector<int>paras(std::initializer_list<int>({role, DeviceProxy::LOCAL_COGNI}));
            emit deviceProxyDeviceProxyDataChanged(px->getModelIx(), paras);
            return;
        }
        emit deviceProxyDeviceProxyDataChanged(px->getModelIx(), QVector<int>(1,role));
    }
}

QMLInterface::BronListModel &QMLInterface::DeviceProxy::getLampList(){
    return m_lampList;
}
