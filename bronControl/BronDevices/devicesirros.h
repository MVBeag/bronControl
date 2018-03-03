#ifndef DEVICESIRROS_H
#define DEVICESIRROS_H

#include <QObject>
#include "device.h"
#include "creatorDevice.h"

namespace Framework{

class DeviceSiros : public Device{
    Q_OBJECT
public:
    enum eSettings{
        eRescanTime = 500 // ms
    };

    enum Ids{
        DEVICE_DESC               = PARAM_DEVICE_DESC,
        STUDIO_ADDRESS            = PARAM_STUDIO_ADDRESS,
        NUM_OF_LAMPS              = PARAM_NUM_OF_LAMPS,
        DEVICE_NAME               = PARAM_DEVICE_NAME,
        SOFTWARE_VERSION          = PARAM_SOFT_VERSION,
        LAMP_ADDRESS              = PARAM_WLAN_LAMP_ADDRESS,
        ALARM                     = PARAM_WLAN_ALARM,
        MAX_ENERGY                = PARAM_WLAN_MAX_ENGERGY,
        F_STOP_RANGE              = PARAM_WLAN_F_STOP_RANGE,
        STANDBY                   = PARAM_WLAN_SIROS_IN_STANDBYE,
        SPEED                     = PARAM_WLAN_SIROS_IN_SPEED,
        AUDIO                     = PARAM_WLAN_SIROS_IN_AUDIO,
        SEQUENCE                  = PARAM_WLAN_SEQUENZ,
        SYNCHRONIZED_SEQUENCE     = PARAM_SYNCRONIZED_SEQUENCE,
        PAUSE_TIME                = PARAM_CHARGE_TIME,
        FLASH_DURATION            = PARAM_FLASH_DURATION,
        DELAY_TIME                = PARAM_DELAYTIME,
        COG_LIGHT_ENA             = PARAM_WLAN_COG_LIGHT_ENA,
        COG_LIGHT_ACT             = PARAM_WLAN_COG_LIGHT_ACT,
        COG_LIGHT_COL             = PARAM_WLAN_COG_LIGHT_COL,
        SLOW_CHARGE               = PARAM_MANUAL_SLOW_CHARGE,
        FLASH                     = PARAM_WLAN_TEST,
        MODEL_LIGHT_ENA           = PARAM_MODEL_LIGHT_ENA,
        MODEL_LIGHT_MODE          = PARAM_MODEL_LIGHT_MODE,
        ENERGY_TOTAL_APPERTURE    = PARAM_APERTURE_ALL_LAMPS,
        APPLICATION               = PARAM_FREEMASK_ALTERMATE_ENA,
        MASKGROUP                 = PARAM_MASKGROUP,
        FLASH_COUNT               = PARAM_FLASH_COUNT,


        /* Device network parameters */
        JOIN_NETWORK              = PARAM_JOIN_NETWORK,
        WLAN_LOGIN1_SSID          = PARAM_WLAN_LOGIN1_SSID,
        WLAN_LOGIN1_PWD           = PARAM_WLAN_LOGIN1_PWD,
        WLAN_ENC_MODE             = PARAM_WLAN_LOGIN1_ENC_TYPE,

        // new in SW >= xx.92
        RF_SELECT                 = PARAM_WLAN_RF_SELECT,
        TAG                       = PARAM_WLAN_TAG,
        UPDATE_SERVICE            = PARAM_WLAN_UPDATE_SERVICE
    };
    Q_ENUM(Ids)

    explicit DeviceSiros();
    explicit DeviceSiros(int lampAddr, bool demo = false);
    explicit DeviceSiros(std::weak_ptr<CreatorDevice> creator);
    explicit DeviceSiros(const DeviceSiros &s);
    virtual ~DeviceSiros();

    virtual eDeviceTypes getType() const override;

    /**
     * @brief GetAllParas get a list with all parameters
     * @return
     */
    virtual QList<std::shared_ptr<DevicePara> > GetAllParas() const override;

    /**
     * @brief GetInitParas Parameters, need to read out during device init
     * @return
     */
    virtual QList<std::shared_ptr<DevicePara> > GetInitParas()const override;

    /**
     * @brief GetPollParas Parameters, needed to be called cyclic
     * @return
     */
    virtual QList<std::shared_ptr<DevicePara> > GetPollParas()const override;


    virtual bool setTest() const override;
    /**
     * @brief isInit returns the init state
     * @return
     */

    virtual float getApertureAct() const override;

    virtual ParaSelects::SpeedMode getSpeed() const override;

    virtual bool isInit() const override;

    /**
     * @brief setInit sets the init state
     * @param val
     */
    virtual void setInit(bool val) override;

    QString getName() const override;

    /**
     * @brief isInStandby gets the standby state
     * @return
     */
    virtual int getStandby() const override;
    virtual bool setStandby(int val) override;

    /**
     * @brief getAlarm gets the alarm
     * @return NONE if no alarm, alarm code if alarm
     */
    virtual ParaSelects::AlarmType getAlarm() const override;

    virtual QString className() const override;


    /**
     * @brief GetParaLocal
     * @param id
     * @return local parameter without communication
     */
    virtual std::shared_ptr<DevicePara> getPara(int id )const override;
    virtual int getLampAddress() const override;
    virtual int getFlashCount() const override;
    virtual bool setLampAddress(int val) override;
    virtual int getStudioAddress() const override;
    virtual bool setName(const QString &name) override;
    virtual int iDLampAddress() const override;
    virtual int iDStudioAddress() const override;
    virtual int iDAudio() const override;


    virtual const QString IdString(int id) const override;
    virtual bool increasePower(float val) override;
    virtual bool decreasePower(float val) override;
    virtual bool setModLight(int val) override;
    virtual int getModLight() const override;
    virtual int getModLightMode() const override;
    virtual bool setModLightMode(int val) override;
    virtual bool setSpeed(ParaSelects::SpeedMode val) override;
    virtual void setDefaultParaId(int id) override;
    virtual QString getRemoteName() override;
    virtual float getDelay() const override;
    virtual bool setDelay(float val) override;
    virtual bool setStudioAddress(int val) override;
    virtual bool setActApertureEnergy(float val) override;
    virtual bool setAudio(int val) override;
    virtual int getAudio() const override;
    virtual int getSWVersion() const override;
    virtual bool triggerSwVersionReadout() override;
    virtual float getMaxEnergy() const override;
    virtual int getSequence() const override;
    virtual bool setSequence(int val) const override;
    virtual float getFStopRange() const override;
    virtual float getPauseTime() const override;
    virtual bool setPauseTime(float val) override;
//    virtual int getFlashTimeSetting() const override;
//    virtual bool setFlashTimeSetting(int val) override;
    virtual int getFlashDuration() const override;
    virtual int getSlowCharge() const override;
    virtual bool setSlowCharge(int val)override;
    virtual int getNumLamps() const override;
    virtual QString getSSID() const override;
    virtual bool setSSID(QString val) override;
    virtual QString getPWD() const override;
    virtual bool setPWD(QString val) override;
    virtual int getEncryptionMode() const override;
    virtual bool setEncryptionMode(int val) override;
    virtual bool setJoinNetwork(int val) override;
    virtual int getSynSequence() const override;
    virtual bool setSynSequence(int val) override;
    virtual int getApplication() const override;
    virtual bool setApplication(int val) override;
    virtual int getMaskGroup() const override;
    virtual bool setMaskGroup(int val) override;
    virtual int migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr) override;
    virtual bool setCogniEnable(bool ena) override;
    virtual int getCogniEnable() const override;
    virtual bool setRemoteControl(ParaSelects::RemoteCtrl sel) override;
    virtual int getRemoteControl() const override;
    virtual bool checkCommandwoResponse(int id) override;
    virtual bool isTestFlash(int id) override;
    virtual void removeRemotePara(int id) override;
    void insertRemotePara(int id, std::shared_ptr<DevicePara> p);
    virtual void correctNetworkSettings() override;
    virtual void setPolling(bool val) override;
    virtual void startUpdateService() override;
    virtual bool isUpdateService(int id) override;
    virtual void activateWink() override;
    virtual void deactivateWink() override;
    virtual void changeWink(bool ena) override;



public slots:
    void doRescan();



private:
    bool m_isInitialized; /**< init indicator */
protected:
    QMap<int, std::shared_ptr<DevicePara> > m_remoteParas; /**< Vector with deviceParameters */
private:
    std::shared_ptr<DefaultPara> m_defaultPara;
    void initParas(int addr = 0, bool demo = false);

};

inline Device::eDeviceTypes DeviceSiros::getType() const{
    return Device::eDeviceTypes::eSiros;
}

inline bool DeviceSiros::setTest() const{
    return getPara(FLASH)->setRemote(1);
}

inline ParaSelects::SpeedMode DeviceSiros::getSpeed() const{
    if(getName() == "Siros 400 " || getName() == "Siros 800 "){
        return ParaSelects::SpeedMode::notSupported;
    }
    return static_cast<ParaSelects::SpeedMode>(getPara(SPEED)->data().toInt());
}

inline bool DeviceSiros::isInit() const{
    return m_isInitialized;
}

inline void DeviceSiros::setInit(bool val){
    m_isInitialized = val;
}

inline int DeviceSiros::getStandby() const{
    return getPara(STANDBY)->data().toInt();
}

inline bool DeviceSiros::setStandby(int val){
    return getPara(STANDBY)->setRemote(val);
}

inline QString DeviceSiros::className() const{
    return DeviceSiros::staticMetaObject.className();
}

inline std::shared_ptr<DevicePara> DeviceSiros::getPara(int id) const{
    return m_remoteParas.value(id, m_defaultPara);      // dont give nullptr in case of error otherwise crash will haplen, create a dummy device that logs the invalid id
}

inline int DeviceSiros::getLampAddress() const{
    // take care that this para is valid
    return getPara(LAMP_ADDRESS)->data().toInt();
}

inline bool DeviceSiros::setLampAddress(int val){
    return getPara(LAMP_ADDRESS)->setRemote(val);
}

inline int DeviceSiros::getFlashCount() const{
    // take care that this para is valid
    return getPara(FLASH_COUNT)->data().toInt(); // if I write 23, 23 is displayed
}

inline int DeviceSiros::getStudioAddress() const{
    // take care that this para is valid
    return getPara(STUDIO_ADDRESS)->data().toInt();
}

inline int DeviceSiros::iDLampAddress() const{
    return LAMP_ADDRESS;
}

inline int DeviceSiros::iDStudioAddress() const{
    return STUDIO_ADDRESS;
}

inline int DeviceSiros::iDAudio() const{
    return AUDIO;
}

inline bool DeviceSiros::setModLight(int val){
    return getPara(MODEL_LIGHT_ENA)->setRemote(val);
}

inline int DeviceSiros::getModLight() const{
    return getPara(MODEL_LIGHT_ENA)->data().toInt();
}

inline int DeviceSiros::getModLightMode() const{
    return getPara(MODEL_LIGHT_MODE)->data().toInt();
}

inline bool DeviceSiros::setModLightMode(int val){
    return getPara(MODEL_LIGHT_MODE)->setRemote(val);
}

inline void DeviceSiros::setDefaultParaId(int id){
    m_defaultPara->setId(id);
}

inline QString DeviceSiros::getRemoteName(){
    return getPara(DEVICE_NAME)->data().toString();
}

inline float DeviceSiros::getDelay() const{
    return getPara(DELAY_TIME)->data().toFloat();
}

inline bool DeviceSiros::setDelay(float val){
    return getPara(DELAY_TIME)->setRemote(val);
}

inline bool DeviceSiros::setStudioAddress(int val){
    return getPara(STUDIO_ADDRESS)->setRemote(val);
}

inline bool DeviceSiros::setActApertureEnergy(float val){
    return getPara(ENERGY_TOTAL_APPERTURE)->setRemote(val);
}

inline bool DeviceSiros::setAudio(int val){
    return getPara(AUDIO)->setRemote(val);
}

inline int DeviceSiros::getAudio() const{
    return getPara(AUDIO)->data().toInt();
}

inline int DeviceSiros::getSWVersion() const{
    return getPara(SOFTWARE_VERSION)->data().toInt();
}

inline float DeviceSiros::getMaxEnergy() const{
    return getPara(MAX_ENERGY)->data().toFloat();
}

inline int DeviceSiros::getSequence() const{
    return getPara(SEQUENCE)->data().toInt();
}

inline bool DeviceSiros::setSequence(int val) const{
    return getPara(SEQUENCE)->setRemote(val == 0 ? 1 : val);
}

inline float DeviceSiros::getFStopRange() const{
    return getPara(F_STOP_RANGE)->data().toFloat();
}

inline float DeviceSiros::getPauseTime() const{
    return getPara(PAUSE_TIME)->data().toFloat();
}

inline bool DeviceSiros::setPauseTime(float val){
    return getPara(PAUSE_TIME)->setRemote(val);
}

//inline int DeviceSiros::getFlashTimeSetting() const{
//    return getSpeed();
//}

//inline bool DeviceSiros::setFlashTimeSetting(int val){
//    return setSpeed(val);
//}

inline int DeviceSiros::getFlashDuration() const{
    return getPara(FLASH_DURATION)->data().toInt();
}

inline int DeviceSiros::getSlowCharge() const{
    return getPara(SLOW_CHARGE)->data().toInt();
}

inline bool DeviceSiros::setSlowCharge(int val){
    return getPara(SLOW_CHARGE)->setRemote(val);
}

inline int DeviceSiros::getNumLamps() const{
    return getPara(NUM_OF_LAMPS)->data().toInt();
}

inline QString DeviceSiros::getSSID() const{
    return getPara(WLAN_LOGIN1_SSID)->data().toString();
}

inline bool DeviceSiros::setSSID(QString val){
    return getPara(WLAN_LOGIN1_SSID)->setRemote(val);
}

inline QString DeviceSiros::getPWD() const{
    return getPara(WLAN_LOGIN1_PWD)->data().toString();
}

inline bool DeviceSiros::setPWD(QString val){
    return getPara(WLAN_LOGIN1_PWD)->setRemote(val);
}

inline int DeviceSiros::getEncryptionMode() const{
    return getPara(WLAN_ENC_MODE)->data().toInt();
}

inline bool DeviceSiros::setEncryptionMode(int val){
    return getPara(WLAN_ENC_MODE)->setRemote(val);
}

inline bool DeviceSiros::setJoinNetwork(int val){
    return getPara(JOIN_NETWORK)->setRemote(val);
}

inline int DeviceSiros::getSynSequence() const{
    return getPara(SYNCHRONIZED_SEQUENCE)->data().toInt();
}

inline bool DeviceSiros::setSynSequence(int val){
    // hopefully, the device switches automatically to photocell
    return getPara(SYNCHRONIZED_SEQUENCE)->setRemote(val);
}

inline int DeviceSiros::getApplication() const{
    return getPara(APPLICATION)->data().toInt();
}

inline bool DeviceSiros::setApplication(int val){
    return getPara(APPLICATION)->setRemote(val);
}

inline int DeviceSiros::getMaskGroup() const{
    return getPara(MASKGROUP)->data().toInt();
}

inline bool DeviceSiros::setMaskGroup(int val){
    return getPara(MASKGROUP)->setRemote(val);
}



inline bool DeviceSiros::setCogniEnable(bool ena){
    return getPara(COG_LIGHT_ENA)->setRemote(ena);
}

inline int DeviceSiros::getCogniEnable() const{
    return getPara(COG_LIGHT_ENA)->data().toInt();
}

inline bool DeviceSiros::checkCommandwoResponse(int id){
    return (static_cast<Ids>(id) == Ids::FLASH);
}

inline bool DeviceSiros::isTestFlash(int id){
    return (static_cast<Ids>(id) == Ids::FLASH);
}

inline void DeviceSiros::setPolling(bool){
    Device::setPolling(false);
    // siros are not polled
}

inline void DeviceSiros::startUpdateService(){
    emit startRescanTimer();
}

inline bool DeviceSiros::isUpdateService(int id){
    return (static_cast<Ids>(id) == Ids::UPDATE_SERVICE);
}

inline void DeviceSiros::activateWink(){
    getPara(COG_LIGHT_ENA)->setRemote(1);
}

inline void DeviceSiros::deactivateWink(){
    getPara(COG_LIGHT_ENA)->setRemote(0);
}

inline void DeviceSiros::changeWink(bool ena){
    getPara(COG_LIGHT_ENA)->setRemote(ena);
}

inline void DeviceSiros::doRescan(){
    getPara(UPDATE_SERVICE)->getRemote();
}

}

Q_DECLARE_METATYPE(Framework::DeviceSiros)

#endif // DEVICESIRROS_H
