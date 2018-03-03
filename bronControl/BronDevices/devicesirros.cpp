#include "devicesirros.h"
#include "paraSelectables.h"
#include "devicepara.h"
#include "demodevicepara.h"
#include <math.h>
#include <devicesirrosL.h>
#include <devicesirrosS.h>

using namespace Framework;

/**
  * validity functions
  */

static bool energyValidity(Device &d, const QVariant val){
    return val > d.getApertureMin();
}

static void checkReadoutParas(Device &d){
    QString s = d.getPara(DeviceSiros::UPDATE_SERVICE)->data().toString();
    QStringList paras = s.split(",");
    QStringListIterator iter(paras);
    while(iter.hasNext()){
        QString s = iter.next();
        if(!s.isEmpty()){
            bool ok = false;
            int id = s.toInt(&ok, 16);
            if(ok){
//                qDebug() << d.getName() << " update service: para id: " << id << " [" << d.IdString(id) << "]";
                d.getPara(id)->getRemote();
            }
        }
    }
    return;
}


static void checkSWVersion(Device &d){
//    int version = d.getPara(DeviceSiros::SOFTWARE_VERSION)->data().toInt();
    //check if S or L
    if(d.getType() == Device::eDeviceTypes::eSirosL){
        emit d.swVersionRead(d.shared_from_this());
    }
    else if(d.getType() == Device::eDeviceTypes::eSirosS){
        emit d.swVersionRead(d.shared_from_this());
    }
    else if(d.getType() == Device::eDeviceTypes::eSiros){
        emit d.swVersionRead(d.shared_from_this());
    }
}

void DeviceSiros::initParas(int addr, bool demo){
    if(demo){
        m_remoteParas.insert(DEVICE_DESC,             std::make_shared<DemoDevicePara>(this, (DEVICE_DESC),0,DevicePara::Types::eInt,DevicePara::eInit));
        m_remoteParas.insert(STANDBY,                 std::make_shared<DemoDevicePara>(this, (STANDBY), false, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DEVICE_NAME,             std::make_shared<DemoDevicePara>(this, (DEVICE_NAME), "",DevicePara::Types::eString, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SOFTWARE_VERSION,        std::make_shared<DemoDevicePara>(this, (SOFTWARE_VERSION), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(LAMP_ADDRESS,            std::make_shared<DemoDevicePara>(this, (LAMP_ADDRESS), addr, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(STUDIO_ADDRESS,          std::make_shared<DemoDevicePara>(this, (STUDIO_ADDRESS), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(MAX_ENERGY,              std::make_shared<DemoDevicePara>(this, (MAX_ENERGY), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(F_STOP_RANGE,            std::make_shared<DemoDevicePara>(this, (F_STOP_RANGE), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit));
        m_remoteParas.insert(COG_LIGHT_COL,           std::make_shared<DemoDevicePara>(this, (COG_LIGHT_COL), (int)ParaSelects::CognitionLightColor::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(COG_LIGHT_ENA,           std::make_shared<DemoDevicePara>(this, (COG_LIGHT_ENA), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(COG_LIGHT_ACT,           std::make_shared<DemoDevicePara>(this, (COG_LIGHT_ACT), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(ENERGY_TOTAL_APPERTURE,  std::make_shared<DemoDevicePara>(this, (ENERGY_TOTAL_APPERTURE), QVariant(0),DevicePara::Types::eInt, 10.0f, DevicePara::eInit, &energyValidity));
        m_remoteParas.insert(ALARM,                   std::make_shared<DemoDevicePara>(this, (ALARM), (int)ParaSelects::AlarmType::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(AUDIO,                   std::make_shared<DemoDevicePara>(this, (AUDIO), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
//        m_remoteParas.insert(SPEED,                   std::make_shared<DemoDevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::ePoll));
        m_remoteParas.insert(MODEL_LIGHT_ENA,         std::make_shared<DemoDevicePara>(this, (MODEL_LIGHT_ENA), false, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(MODEL_LIGHT_MODE,        std::make_shared<DemoDevicePara>(this, (MODEL_LIGHT_MODE), (int)ParaSelects::ModelingLightMode::FULL,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(FLASH_DURATION,          std::make_shared<DemoDevicePara>(this, (FLASH_DURATION), 0,DevicePara::Types::eInt,  1.0f, DevicePara::eInit));
        m_remoteParas.insert(SEQUENCE,                std::make_shared<DemoDevicePara>(this, (SEQUENCE), 1,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SYNCHRONIZED_SEQUENCE,   std::make_shared<DemoDevicePara>(this, (SYNCHRONIZED_SEQUENCE), (int)ParaSelects::SequenceModes::DISABLED, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(PAUSE_TIME,              std::make_shared<DemoDevicePara>(this, (PAUSE_TIME), 0,DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(DELAY_TIME,              std::make_shared<DemoDevicePara>(this, (DELAY_TIME), 0,DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(APPLICATION,             std::make_shared<DemoDevicePara>(this, (APPLICATION), (int)ParaSelects::FreemaskMode::DISABLED,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(MASKGROUP,               std::make_shared<DemoDevicePara>(this, (MASKGROUP), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(FLASH,                   std::make_shared<DemoDevicePara>(this, (FLASH),0,DevicePara::Types::eInt));
        m_remoteParas.insert(NUM_OF_LAMPS,            std::make_shared<DemoDevicePara>(this, (NUM_OF_LAMPS),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
//        m_remoteParas.insert(SLOW_CHARGE,             std::make_shared<DemoDevicePara>(this, (SLOW_CHARGE),0,DevicePara::Types::eInt, 1.0f, DevicePara::ePoll));
        m_remoteParas.insert(JOIN_NETWORK,            std::make_shared<DemoDevicePara>(this, (JOIN_NETWORK),0,DevicePara::Types::eInt));
        m_remoteParas.insert(WLAN_LOGIN1_SSID,        std::make_shared<DemoDevicePara>(this, (WLAN_LOGIN1_SSID),"Bron-Studio",DevicePara::Types::eString));
        m_remoteParas.insert(WLAN_LOGIN1_PWD,         std::make_shared<DemoDevicePara>(this, (WLAN_LOGIN1_PWD),"bronControl",DevicePara::Types::eString));
        m_remoteParas.insert(WLAN_ENC_MODE,           std::make_shared<DemoDevicePara>(this, (WLAN_ENC_MODE),(int)ParaSelects::EncryptionMode::WPA2,DevicePara::Types::eInt));
        m_remoteParas.insert(RF_SELECT,               std::make_shared<DemoDevicePara>(this, (RF_SELECT),(int)ParaSelects::RemoteCtrlCellRfs::SirosRcRfs,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(TAG,                     std::make_shared<DemoDevicePara>(this, (TAG), "",DevicePara::Types::eString, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(UPDATE_SERVICE,          std::make_shared<DemoDevicePara>(this, (UPDATE_SERVICE), "",DevicePara::Types::eString, 1.0f, DevicePara::eInit, &everValid, &checkReadoutParas));
    }
    else{
        m_remoteParas.insert(DEVICE_DESC,             std::make_shared<DevicePara>(this, (DEVICE_DESC),0,DevicePara::Types::eInt,DevicePara::eInit));
        m_remoteParas.insert(STANDBY,                 std::make_shared<DevicePara>(this, (STANDBY), false, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DEVICE_NAME,             std::make_shared<DevicePara>(this, (DEVICE_NAME), "",DevicePara::Types::eString, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SOFTWARE_VERSION,        std::make_shared<DevicePara>(this, (SOFTWARE_VERSION), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit, &everValid, &checkSWVersion));
        m_remoteParas.insert(LAMP_ADDRESS,            std::make_shared<DevicePara>(this, (LAMP_ADDRESS), addr, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(STUDIO_ADDRESS,          std::make_shared<DevicePara>(this, (STUDIO_ADDRESS), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(MAX_ENERGY,              std::make_shared<DevicePara>(this, (MAX_ENERGY), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(F_STOP_RANGE,            std::make_shared<DevicePara>(this, (F_STOP_RANGE), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit));
        m_remoteParas.insert(COG_LIGHT_COL,           std::make_shared<DevicePara>(this, (COG_LIGHT_COL), (int)ParaSelects::CognitionLightColor::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(COG_LIGHT_ENA,           std::make_shared<DevicePara>(this, (COG_LIGHT_ENA), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(COG_LIGHT_ACT,           std::make_shared<DevicePara>(this, (COG_LIGHT_ACT), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(ENERGY_TOTAL_APPERTURE,  std::make_shared<DevicePara>(this, (ENERGY_TOTAL_APPERTURE), QVariant(0),DevicePara::Types::eInt, 10.0f, DevicePara::eInit, &energyValidity));
        m_remoteParas.insert(ALARM,                   std::make_shared<DevicePara>(this, (ALARM), (int)ParaSelects::AlarmType::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(AUDIO,                   std::make_shared<DevicePara>(this, (AUDIO), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
//        m_remoteParas.insert(SPEED,                   std::make_shared<DevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::ePoll));
        m_remoteParas.insert(MODEL_LIGHT_ENA,         std::make_shared<DevicePara>(this, (MODEL_LIGHT_ENA), false, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(MODEL_LIGHT_MODE,        std::make_shared<DevicePara>(this, (MODEL_LIGHT_MODE), (int)ParaSelects::ModelingLightMode::FULL,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(FLASH_DURATION,          std::make_shared<DevicePara>(this, (FLASH_DURATION), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SEQUENCE,                std::make_shared<DevicePara>(this, (SEQUENCE), 1,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SYNCHRONIZED_SEQUENCE,   std::make_shared<DevicePara>(this, (SYNCHRONIZED_SEQUENCE), (int)ParaSelects::SequenceModes::DISABLED, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(PAUSE_TIME,              std::make_shared<DevicePara>(this, (PAUSE_TIME), 0,DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(DELAY_TIME,              std::make_shared<DevicePara>(this, (DELAY_TIME), 0,DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(APPLICATION,             std::make_shared<DevicePara>(this, (APPLICATION), (int)ParaSelects::FreemaskMode::DISABLED,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(MASKGROUP,               std::make_shared<DevicePara>(this, (MASKGROUP), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(FLASH,                   std::make_shared<DevicePara>(this, (FLASH),0,DevicePara::Types::eInt));
        m_remoteParas.insert(NUM_OF_LAMPS,            std::make_shared<DevicePara>(this, (NUM_OF_LAMPS),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
//        m_remoteParas.insert(SLOW_CHARGE,             std::make_shared<DevicePara>(this, (SLOW_CHARGE),0,DevicePara::Types::eInt, 1.0f, DevicePara::ePoll));
        m_remoteParas.insert(JOIN_NETWORK,            std::make_shared<DevicePara>(this, (JOIN_NETWORK),0,DevicePara::Types::eInt));
// dont't read this in init or any other phase, this paras are given von creator device
        m_remoteParas.insert(WLAN_LOGIN1_SSID,        std::make_shared<DevicePara>(this, (WLAN_LOGIN1_SSID),"Bron-Studio",DevicePara::Types::eString, 1.0f));
        m_remoteParas.insert(WLAN_LOGIN1_PWD,         std::make_shared<DevicePara>(this, (WLAN_LOGIN1_PWD),"bronControl",DevicePara::Types::eString, 1.0f));
        m_remoteParas.insert(WLAN_ENC_MODE,           std::make_shared<DevicePara>(this, (WLAN_ENC_MODE),(int)ParaSelects::EncryptionMode::WPA2,DevicePara::Types::eInt));
        m_remoteParas.insert(RF_SELECT,               std::make_shared<DevicePara>(this, (RF_SELECT),(int)ParaSelects::RemoteCtrlCellRfs::SirosRcRfs,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(TAG,                     std::make_shared<DevicePara>(this, (TAG), "",DevicePara::Types::eString, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(UPDATE_SERVICE,          std::make_shared<DevicePara>(this, (UPDATE_SERVICE), "",DevicePara::Types::eString, 1.0f, DevicePara::eInit, &everValid, &checkReadoutParas));
        }
}

Framework::DeviceSiros::DeviceSiros()
    : Device(false)
    , m_isInitialized(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
    initParas(0, false);
}

Framework::DeviceSiros::DeviceSiros(int lampAddr, bool demo)
    : Device(demo)
    , m_isInitialized(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
    initParas(lampAddr, demo);
}

Framework::DeviceSiros::DeviceSiros(std::weak_ptr<CreatorDevice> creator)
    : Device(false)
    , m_isInitialized(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
//    setBcastDevice(cd->getBcastDevice());
    if(auto cd = creator.lock()){
        setInterfaceIx(cd->getInterfaceIx());
        initParas();
        setHostAddress(cd->getHostAddress(), cd->getHostPort());
        *(getPara(DEVICE_DESC)) = *(cd->getPara(CreatorDevice::DEVICE_DESC));
        *(getPara(DEVICE_NAME)) = *(cd->getPara(CreatorDevice::DEVICE_NAME));
        *(getPara(STUDIO_ADDRESS)) = *(cd->getPara(CreatorDevice::STUDIO_ADDRESS));
        *(getPara(MAX_ENERGY)) = *(cd->getPara(CreatorDevice::MAX_ENERGY));
        *(getPara(F_STOP_RANGE)) = *(cd->getPara(CreatorDevice::F_STOP_RANGE));
        // due to the fact, that some siro aren`t give a SSID or passowrd, correct this here
        *(getPara(WLAN_LOGIN1_SSID))  = *(cd->getPara(CreatorDevice::SSID_COMPACT));
        *(getPara(WLAN_LOGIN1_PWD)) = *(cd->getPara(CreatorDevice::PWD_COMPACT));
        *(getPara(WLAN_ENC_MODE)) = *(cd->getPara(CreatorDevice::ENCR_COMPACT));
        connect(m_rescanTimer, SIGNAL(timeout()), this, SLOT(doRescan()));
        m_rescanTimer->setInterval(eRescanTime);
        m_rescanTimer->setSingleShot(false);
//      rescanTimer is started on initDone
//        emit startRescanTimer();
    }
}

Framework::DeviceSiros::DeviceSiros(const DeviceSiros &s)
    : Device(s.parent())
    , m_isInitialized(s.m_isInitialized)
    , m_remoteParas(s.m_remoteParas)
    , m_defaultPara(std::make_shared<DefaultPara>(this)){}

//protected boolean ecoEnabled = false; sirrosL
//protected float energyCur = 0f;

Framework::DeviceSiros::~DeviceSiros(){
     m_remoteParas.clear();
}

QList<std::shared_ptr<DevicePara> > Framework::DeviceSiros::GetAllParas() const{
    return m_remoteParas.values();
}

QList<std::shared_ptr<DevicePara> > Framework::DeviceSiros::GetPollParas()const{
    QList<std::shared_ptr<DevicePara> > poll;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::ePoll)){
            poll.append(p);
        }
    }
    return poll;
}

float Framework::DeviceSiros::getApertureAct() const{
    return getPara(ENERGY_TOTAL_APPERTURE)->data().toFloat();
}

QString Framework::DeviceSiros::getName() const{
    QString name = "Siros " + getPara(MAX_ENERGY)->data().toString() + " ";
    if(getPara(DEVICE_NAME)->data() != "SirosL"){
        if(getPara(F_STOP_RANGE)->data().toInt() == 8){
            name += "S";
        }
    }
    else{
        name += "L";
    }
    return name;
}



ParaSelects::AlarmType Framework::DeviceSiros::getAlarm() const{
    auto par = getPara(ALARM);
    if(par){
        return static_cast<ParaSelects::AlarmType>(par->data().toInt());
    }
    return ParaSelects::AlarmType::NONE;
}

QList<std::shared_ptr<DevicePara> > Framework::DeviceSiros::GetInitParas()const{
    QList<std::shared_ptr<DevicePara> > init;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::eInit)){
            init.append(p);
        }
    }
    return init;
}

bool Framework::DeviceSiros::setName(const QString &name){
    auto  np = getPara(DEVICE_NAME);
    if(np){
        if(np->data().toString() != name){
            if(np->setRemote(name)){
                return true;
            }
        }
    }
    return false;
}

const QString Framework::DeviceSiros::IdString(int id) const{
    const QMetaEnum &me = QMetaEnum::fromType<DeviceSiros::Ids>();
    const char *key = me.valueToKey(static_cast<DeviceSiros::Ids>(id));
    if(key){
        return QString(key);
    }
    else{
        return QString("DeviceSiros unknown Id: " + QString::number(id));
    }
}

bool Framework::DeviceSiros::increasePower(float val){
    float currEnergy = getApertureAct();
    float increment = val;
    currEnergy = ((currEnergy * 10.0f) + (increment * 10.0f))/ 10.0f;
    if(currEnergy > getApertureMax()){
        currEnergy = getApertureMax();
    }

    getPara(ENERGY_TOTAL_APPERTURE)->setRemote(QVariant::fromValue(currEnergy));
    getPara(FLASH_DURATION)->getRemote();
    return true;
}

bool Framework::DeviceSiros::decreasePower(float val){
    float currEnergy = getApertureAct();
    float increment = val;
    currEnergy = ((currEnergy * 10.0f) - (increment * 10.0f))/ 10.0f;
    if(currEnergy < getApertureMin()){
        currEnergy = getApertureMin();
    }

    getPara(ENERGY_TOTAL_APPERTURE)->setRemote(QVariant::fromValue(currEnergy));
    getPara(FLASH_DURATION)->getRemote();
    return true;
}

bool Framework::DeviceSiros::setSpeed(ParaSelects::SpeedMode val){
    if(getName() == "Siros 400 " || getName() == "Siros 800 "){
        return false;
    }
    getPara(SPEED)->setRemote(static_cast<int>(val));
    getPara(FLASH_DURATION)->getRemote();
    return true;
}

bool DeviceSiros::triggerSwVersionReadout(){
    if(auto p = getPara(SOFTWARE_VERSION)){
        // check if parameter was read in the past
        if(p->getState() == DevicePara::State::eUndefined){
            // not read, initiate it now
            p->getRemote();
        }
        else{
            // data read, emit signal again
            emit swVersionRead(shared_from_this());
        }
    }
    return true;
}

int Framework::DeviceSiros::migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr){
    Q_UNUSED(encr)
    if(enterprize){
        QExplicitlySharedDataPointer<CommandSequence> seq = startCommandSequence("migrate to network");
        QList<std::function<bool()> >postRemoteActions;
        postRemoteActions.append(std::bind(&Device::sendLost, shared_from_this()));
        bool (DevicePara::*fct)(QVariant, QList<std::function<bool()> >) = &DevicePara::setRemote;
        DeviceAction act(seq->number, std::bind(fct, getPara(JOIN_NETWORK), QVariant(enterprize), postRemoteActions));
        seq->addAction(act);
        getPara(WLAN_LOGIN1_SSID)->setRemote(ssid);
        getPara(WLAN_LOGIN1_PWD)->setRemote(pw);
        endCommandSequence(seq);
    }
    else{
        QExplicitlySharedDataPointer<CommandSequence> seq = startCommandSequence("migrate to network");
        QList<std::function<bool()> >postRemoteActions;
        postRemoteActions.append(std::bind(&Device::sendLost, shared_from_this()));
        bool (DevicePara::*fct)(QVariant, QList<std::function<bool()> >) = &DevicePara::setRemote;
        DeviceAction act(seq->number, std::bind(fct, getPara(JOIN_NETWORK), QVariant(2), postRemoteActions));
        seq->addAction(act);
        getPara(WLAN_LOGIN1_SSID)->setRemote("");
        getPara(WLAN_LOGIN1_PWD)->setRemote("");
        endCommandSequence(seq);
    }
    return 1;
}

bool Framework::DeviceSiros::setRemoteControl(ParaSelects::RemoteCtrl sel){
    switch(sel){
    case ParaSelects::RemoteCtrl::RcOff: getPara(RF_SELECT)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlCellRfs::SirosRcOff)); break;
    case ParaSelects::RemoteCtrl::RcRfs: getPara(RF_SELECT)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlCellRfs::SirosRcRfs)); break;
    case ParaSelects::RemoteCtrl::RcIr:  break;
    case ParaSelects::RemoteCtrl::RcIrRfs: getPara(RF_SELECT)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlCellRfs::SirosRcRfs)); break;
    case ParaSelects::RemoteCtrl::RcCell:  getPara(RF_SELECT)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlCellRfs::SirosRcCell)); break;
    case ParaSelects::RemoteCtrl::RcCellRfs: getPara(RF_SELECT)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlCellRfs::SirosRcRfsCell)); break;
    case ParaSelects::RemoteCtrl::RcCellIr:  getPara(RF_SELECT)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlCellRfs::SirosRcCell)); break;
    case ParaSelects::RemoteCtrl::RcCellIrRfs: getPara(RF_SELECT)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlCellRfs::SirosRcRfsCell)); break;
    }
    return true;
}

int Framework::DeviceSiros::getRemoteControl() const{
    switch(static_cast<ParaSelects::RemoteCtrlCellRfs>(getPara(RF_SELECT)->data().toInt())){
    case ParaSelects::RemoteCtrlCellRfs::SirosRcOff:  return static_cast<int>(ParaSelects::RemoteCtrl::RcOff);
    case ParaSelects::RemoteCtrlCellRfs::SirosRcCell: return static_cast<int>(ParaSelects::RemoteCtrl::RcCell);
    case ParaSelects::RemoteCtrlCellRfs::SirosRcRfs:  return static_cast<int>(ParaSelects::RemoteCtrl::RcRfs);
    case ParaSelects::RemoteCtrlCellRfs::SirosRcRfsCell: return static_cast<int>(ParaSelects::RemoteCtrl::RcCellRfs);
    }
    return -1;
}

void Framework::DeviceSiros::removeRemotePara(int id){
    if(m_remoteParas.contains(id)){
        m_remoteParas.remove(id);
    }
}

void Framework::DeviceSiros::insertRemotePara(int id, std::shared_ptr<DevicePara> p){
    m_remoteParas.insert(id, p);
}

void Framework::DeviceSiros::correctNetworkSettings(){
    QString ssid(getPara(WLAN_LOGIN1_SSID)->data().toString());
    if(ssid == ""){
        ssid = "Bron-Studio" + QString::number(getStudioAddress(),10);
        getPara(WLAN_LOGIN1_SSID)->setData(ssid, Errors::Id::E_NO_ERROR);
        getPara(WLAN_LOGIN1_PWD)->setData("bronControl", Errors::Id::E_NO_ERROR);
//        getPara(WLAN_ENC_MODE)->setData((int)ParaSelects::EncryptionMode::WPA2, Errors::Id::E_NO_ERROR);
    }
}


