#ifndef DEVICESCORO_H
#define DEVICESCORO_H

#include <QObject>
#include "device.h"
#include "creatorDevice.h"

namespace Framework{

class GenLamp;

class DeviceScoro : public Device{
    Q_OBJECT
public:

   enum eSettings{
       eRescanTime = 500 // ms
   };

   enum Ids{
       // scoro parameters
       DEVICE_DESC              = scoroParam::SP_DEVICE_DESC,             /* -p get; -c returns: n=0 invalid descriptor, n=1 ScoroE1600, n=2 ScoroE3200, n=3 ScoroS1600, n=4 ScoroS3200 */
       DEVICE_NAME              = scoroParam::SP_DEVICE_NAME,
       AB_SW_VER                = scoroParam::SP_AB_SW_VER,               /* -c adapterboard software version, 4 bytes: starting with most significant byte: major nr., minor nr., 2Byte revision nr. */
       NW_JOIN                  = scoroParam::SP_NW_JOIN,                 /* -p get/set; -c n=0 for Adhoc or n=1 for Infrastructure mode. In case n=1, do set ssid and pwd previously, for reset to sap mode, write n=2 */
       NW_SSID                  = scoroParam::SP_NW_SSID,                 /* -c the ssid of an infrastructure network */
       NW_PWD                   = scoroParam::SP_NW_PWD,                  /* -c the password of an infrastructure network */
       NUM_LAMP_SLOTS           = scoroParam::SP_NUM_LAMP_SLOTS,          /* -p get; -c number of slots the device has (constant) */
       F_STOP_RANGE             = scoroParam::SP_F_STOP_RANGE,            /* -c the range in 1/10-f stops the device supports (depends on speed mode) */
       F_STOP_MAX               = scoroParam::SP_F_STOP_MAX,              /* -c the top f-stop value the device can currently display in 1/10-f stops (depends on speed and display mode) */
       UPDATE_SERVICE           = scoroParam::SP_UPDATE_SERVICE,          /* -p get; -c returns a string of comma separated ID's writen in hexadecimal format. These ID's have changed recently. Not included are static parameters e.g. serial number and all MEM labeled ID's */
       MULITCMD_SERVICE         = scoroParam::SP_MULTICOMMAND_SERVICE,    /* -p	 used to set an "ID1\t[I||S]param1\nID2\t[I||S]param2\n" String, where either 'I' for integer or 'S' for string must be written in front of each parameter. Command buffer is 14 elements long and receive buffer is 128, 512 or 2048 bytes long */
       GEN_APERTURE             = scoroParam::SP_GEN_APERTURE,            /* -p get/set; -c the aperture of the lamp/gen in 1/10-f stops */
       L1_APERTURE              = scoroParam::SP_L1_APERTURE,
       L2_APERTURE              = scoroParam::SP_L2_APERTURE,
       L3_APERTURE              = scoroParam::SP_L3_APERTURE,
       L1_SW                    = scoroParam::SP_L1_SW, 				  /* -p get/set; -c tells whether the lamp is switched on (1) or off (0) */
       L2_SW                    = scoroParam::SP_L2_SW,
       L3_SW                    = scoroParam::SP_L3_SW,
       L1_INSERTED              = scoroParam::SP_L1_INSERTED,			  /* -p get; -c tells whether the lamp is plugged in (1) or not (0) */
       L2_INSERTED              = scoroParam::SP_L2_INSERTED,
       L3_INSERTED              = scoroParam::SP_L3_INSERTED,
       REQUEST_BLOCK            = scoroParam::SP_REQUEST_BLOCK, 		  /* -p set; -c can be used to request a block from the scoro MCU, namely 101-104 to read out the memory */
       PILOTLIGHT_SW            = scoroParam::SP_PILOTLIGHT_SW,           /* -p get/set; -c the pilot light state on (1) / off (0) */
       PILOTLIGHT_MODE          = scoroParam::SP_PILOTLIGHT_MODE,         /* -c the pilot light mode: n=0 full, n=1 pmax, n=2 spar, n=3-7 prop[1-5] */
       IR_RFS_SW                = scoroParam::SP_RFS_IR_SW,				  /* -p/-c ir/rfs (3), --/rfs (2), ir/-- (1), off (0) */
       CELL_SW                  = scoroParam::SP_CELL_SW,                 /* -c photocell on (1) / off (0) */
       T01                      = scoroParam::SP_T01,					  /* -p get/set; -c the ignition duration, n=0 opt, n=80 min, n=4-9 others */
       T01_VALUE                = scoroParam::SP_T01_VALUE,				  /* -p get; -c returns the ignition duration 1/n */
       SEQUENCE                 = scoroParam::SP_SEQUENCE,			      /* -p get/set; -c the number of flashes triggered with each release (n=0 off, n<=50) */
       INTERVAL                 = scoroParam::SP_INTERVAL,                /* -c the time between two flashes if sequence>=2 in [ms] */
       DELAY                    = scoroParam::SP_DELAY,                   /* -c the delay from the trigger event to the first flash in [ms] */
       SPEED_SW                 = scoroParam::SP_SPEED_SW,                /* -c speed flash duration, on (1) / off (0) */
       ALTERNATE_NUMBER_DEVICES = scoroParam::SP_ALTERNATE_NUMBER_DEVICES,/* -c 2<=n<=4, e.g. n=2 means that 2 devices take part with alternate and therefore, the device releases every second trigger */
       ALTERNATE_DEVICE_DELAY   = scoroParam::SP_ALTERNATE_DEVICE_DELAY,  /* -c n=0 off (renders the above inactive), n=1 active, n=2-4 wait[1-3]. E.g. n=1 means release on first trigger. Always set the above in advance */
       MEMORY_STORE             = scoroParam::SP_MEMORY_STORE, 			  /* -p set; -c n=1-4 to store current settings into memory slot */
       MEMORY_RECALL            = scoroParam::SP_MEMORY_RECALL, 		  /* -p set; -c n=1-4 to recall memory slot into current settings */
       REFRESH_DATA             = scoroParam::SP_REFRESH_DATA, 			  /* -p set; -c makes scoro send every block (probably never used) */
       RESET_AUX                = scoroParam::SP_RESET_AUX, 		      /* -p set; -c make scoro reset the auxiliary settings */
       STORE_AUX                = scoroParam::SP_STORE_AUX,				  /* -p get/set; -c make scoro save the auxiliary settings */
       SENSITIVITY_PHOTO_CELL   = scoroParam::SP_SENSITIVITY_PHOTO_CELL,  /* -p get/set; -c n=0 low, n=1 normal */
       TEST_KEY_INTENSITY       = scoroParam::SP_TEST_KEY_INTENSITY,      /* -c n=0 bright, n=1 dimmed (the blue light of the test button)*/
       DISPLAY_SPEED_APERTURE   = scoroParam::SP_DISPLAY_SPEED_APERTURE,  /* -c n=1 display 10 (respectively 9 if SP_DISPLAY_APERTURE_9=1) as the maximal aperture in speed mode */
       REDUCED_PILOTLIGHT       = scoroParam::SP_REDUCED_PILOTLIGHT,      /* -c n=1 reduce the pilotlight during charging */
       SLOW_CHARGING            = scoroParam::SP_SLOW_CHARGING,           /* -c n=1 slow charging */
       DISPLAY_APERTURE_9       = scoroParam::SP_DISPLAY_APERTURE_9,      /* -c n=1 display 9 as the maximal aperture */
       RESET_FLASH_COUNTER      = scoroParam::SP_RESET_FLASH_COUNTER,	  /* -p set */
       DISPLAY_INTENSITY        = scoroParam::SP_DISPLAY_INTENSITY,		  /* -p get/set; -c n=0 enviromentlight dependent, n=1-3 low, medium, high */
       RDY_SOUND_MODE           = scoroParam::SP_RDY_SOUND_MODE,          /* -c n=0 standart, n=1 address specific */
       RDY_SOUND_VOLUME         = scoroParam::SP_RDY_SOUND_VOLUME,		  /* -c n=0 off, n=1-3 quiet, medium, noisy */
       COLOR_TEMP               = scoroParam::SP_COLOR_TEMP,			  /* -c n=0 decrement, n=1 increment, n=2 suggestion takeover*/
       COUNTRY_NR               = scoroParam::SP_COUNTRY_NR,			  /* -p get */
       SALES_YEAR               = scoroParam::SP_SALES_YEAR,			  /* -p get/set */
       SALES_MONTH              = scoroParam::SP_SALES_MONTH,
       SERVICE_YEAR             = scoroParam::SP_SERVICE_YEAR,			  /* -p get/set */
       SERVICE_MONTH            = scoroParam::SP_SERVICE_MONTH,
       STANDBY_SW               = scoroParam::SP_STANDBY_SW,			  /* -p get/set; -c n=1 device into standby */
       STROBO_SW                = scoroParam::SP_STROBO_SW,				  /* -p get/set; -c n=1 device into strobo mode */
       USER_SW                  = scoroParam::SP_USER_SW,                 /* -c user button on (1) / off (0) */
       SCORO_VERSION            = scoroParam::SP_SCORO_VERSION,			  /* -p get; -c n=37 Scoro S, n=137 Scoro E */
       SCORO_CHANGE_IDX         = scoroParam::SP_SCORO_CHANGE_IDX,
       CHARGE_CHANGE_IDX        = scoroParam::SP_CHARGE_CHANGE_IDX,
       SCORO_ENERGY_TYPE        = scoroParam::SP_SCORO_ENERGY_TYPE,		  /* -p get; -c n=3200 or n=1600*/
       SCORO_VOLTAGE_TYPE       = scoroParam::SP_SCORO_VOLTAGE_TYPE,	  /* -c n=0 switching power supply, n=1-3 conventional 230V, 117V, 100V */
       DAILY_FLASH_COUNT        = scoroParam::SP_DAILY_FLASH_COUNT,
       TOTAL_FLASH_COUNT        = scoroParam::SP_TOTAL_FLASH_COUNT,
       ALARM_REASON             = scoroParam::SP_ALARM_REASON,            /* -c n=0 afterglow, n=1 overcharge, n=2 technical issue */
       THERMO_STATE             = scoroParam::SP_THERMO_STATE,            /* -c n=1 Flashcapacity, n=2 Lamp 1, n=4 Lamp 2, n=8 Lamp 3 */
       THERMO_TIME              = scoroParam::SP_THERMO_TIME,             /* -c time in seconds */
       STUDIO_ADDR              = scoroParam::SP_STUDIO_ADDR,			  /* -p get, does already exist */
       L1_ADDR                  = scoroParam::SP_L1_ADDR,				  /* -p get/set */
       L2_ADDR                  = scoroParam::SP_L2_ADDR,
       L3_ADDR                  = scoroParam::SP_L3_ADDR,
       CHARGE_RDY               = scoroParam::SP_CHARGE_RDY,			  /* -p get */
       FLASH_TEST               = scoroParam::SP_FLASH_TEST,			  /* -p set, does already exist */
       MEM_APERTURE_L1          = scoroParam::SP_MEM_APERTURE_L1,		  /* -p get, -c first request a block (101-104) and then read the below parameters */
       MEM_APERTURE_L2          = scoroParam::SP_MEM_APERTURE_L2,
       MEM_APERTURE_L3          = scoroParam::SP_MEM_APERTURE_L3,
       MEM_SEQUENCE             = scoroParam::SP_MEM_SEQUENCE,
       MEM_ALTERNATE            = scoroParam::SP_MEM_ALTERNATE,
       MEM_T01                  = scoroParam::SP_MEM_T01,
       MEM_STROBO_SW            = scoroParam::SP_MEM_STROBO_SW,
       MEM_SPEED_SW             = scoroParam::SP_MEM_SPEED_SW,
       MEM_INTERVAL             = scoroParam::SP_MEM_INTERVAL
    };
    Q_ENUM(Ids)

    explicit DeviceScoro();
    explicit DeviceScoro(int lampAddr, bool demo = false);
    explicit DeviceScoro(std::weak_ptr<CreatorDevice> creator);
    explicit DeviceScoro(const DeviceScoro &sc);
    virtual ~DeviceScoro();


    /* the following functions are overwritten base class functions, for a description, look at base class std::weak_ptr<Device> */
    virtual eDeviceTypes getType() const override;
    virtual QList<std::shared_ptr<DevicePara> > GetAllParas() const override;
    virtual QList<std::shared_ptr<DevicePara> > GetInitParas()const override;
    virtual QList<std::shared_ptr<DevicePara> > GetPollParas()const override;
    virtual float getApertureAct() const override;
    virtual ParaSelects::SpeedMode getSpeed() const override;
    virtual bool isInit() const override;
    virtual void setInit(bool val) override;
    QString getName() const override;
    virtual bool setName(const QString &name) override;
    virtual int getStandby() const override;
    virtual bool setStandby(int val) override;
    virtual ParaSelects::AlarmType getAlarm() const override;
    virtual std::shared_ptr<DevicePara> getPara(int id )const override;
    virtual QString className() const override;
    virtual int getStudioAddress() const override;
    virtual int iDStudioAddress() const override;
    virtual int iDAudio() const override;
    virtual int getDeviceType() const override;
    virtual const QString IdString(int id) const override;
    virtual bool increasePower(float val) override;
    virtual bool decreasePower(float val) override;
    virtual bool setSpeed(ParaSelects::SpeedMode val) override;
    virtual int getModLight() const override;
    virtual bool setModLight(int val) override;
    virtual int getModLightMode() const override;
    virtual bool setModLightMode(int val) override;
    virtual void setDefaultParaId(int id) override;
    virtual QString getRemoteName() override;
    virtual float getDelay() const override;
    virtual bool setDelay(float val) override;
    virtual bool setStudioAddress(int val) override;
    virtual bool setActApertureEnergy(float val) override;
    virtual bool setAudio(int val) override;
    virtual int getAudio() const override;
    virtual int getSWVersion() const override;
    virtual float getFStopRange() const override;
    virtual int getSequence() const override;
    virtual bool setSequence(int var) const override;
    virtual bool setTest() const override;
    virtual float getPauseTime() const override;
    virtual bool setPauseTime(float val) override;
    virtual int getFlashTimeSetting() const override;
    virtual bool setFlashTimeSetting(int val) override;
    virtual int getFlashDuration() const override;
    virtual int getSlowCharge() const override;
    virtual bool setSlowCharge(int val) override;
    virtual int getNumLamps() const override;
    virtual QString getSSID() const override;
    virtual bool setSSID(QString val) override;
    virtual QString getPWD() const override;
    virtual bool setPWD(QString val) override;
    virtual bool setJoinNetwork(int val) override;
    virtual bool setColorTemp(int val) override;
    virtual int getColorTemp() const override;
    virtual bool setDimmer(int val) override;
    virtual int getDimmer() const override;
    virtual bool setCellSensitivity(int val) override;
    virtual int getCellSensitivity() const override;
    virtual bool setCell(int val) override;
    virtual int getCell() const override;
    virtual bool setIR_RFS(int val) override;
    virtual int getIR_RFS() const override;
    virtual bool setBrightnessTest(int val) override;
    virtual int getBrightnessTest() const override;
    virtual bool setRdySoundMode(int val) override;
    virtual int getRdySoundMode() const override;
//    virtual bool setKeySoundVol(int val) override;
//    virtual int getKeySoundVol() const override;
    virtual bool setDisplayIntensity(int val) override;
    virtual int getDisplayIntensity() const override;
    virtual bool resetDailyCounter() override;
    virtual int getDailyCounter() const override;
    virtual int getOverallCounter() const override;
    virtual int getSynSequence() const override;
//    virtual bool setSynSequence(int val) override;
    virtual int getApplication() const override;
    virtual bool setApplication(int val) override;
    virtual int getMaskGroup() const override;
    virtual bool setMaskGroup(int val) override;
    virtual float getMaxEnergy() const override;
    virtual bool setRemoteControl(ParaSelects::RemoteCtrl ctrl) override;
    virtual int getRemoteControl() const override;
    virtual float getApertureMax() const override;
    virtual void setPolling(bool val) override;
    virtual bool isPolling() const override;
    virtual bool checkCommandwoResponse(int id) override;
    virtual bool isTestFlash(int id) override;
    virtual void makeApplicationTransparent() override;
    virtual void startUpdateService() override;
    virtual bool isUpdateService(int id) override;
    virtual void removeRemotePara(int id) override;

    virtual int migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr) override;

    virtual bool sendLost() override;
    virtual void activateWink() override;
    virtual void deactivateWink() override;
    virtual void changeWink(bool ena) override;

    /**
     * @brief getLamps return the device* pointer of the connected lamps
     * @return
     */
    QList<std::weak_ptr<GenLamp> > getLamps();

    /**
     * @brief hasLamp checks if the lamp with nr [0..2] for lamps [1..3] are mounted
     * @param nr lamp nr [0..2]
     * @return true if connected, false if not
     */
    bool hasLamp(int nr);

    /**
     * @brief hasLamp checks if the std::weak_ptr<Device> d belongs to this scoro
     * @param d Device* pointer to check
     * @return true if it belongs, false if not
     */
    bool hasLamp(std::weak_ptr<GenLamp> d);

    /**
     * @brief addLamp adds a GenLamp* to this scoro
     * @param lamp GeneratorLamp
     */
    void addLamp(std::weak_ptr<GenLamp> lamp);

    /**
     * @brief removeLamp removes the GenLamp with the number from this generator
     * @param nr to remove [0..2]
     * @return the pointer to the removed lamp in case of success, nullptr in case of failure (lamp was not in)
     */
    std::weak_ptr<GenLamp> removeLamp(int nr);

    /**
     * @brief getNumConnectedLamps
     * @return the number of plugged lamps
     */
    int getNumConnectedLamps() const;

    void isInDeleter(bool isIn);

    bool sendCmdSequenceEnd();

signals:
    void createLamp(DeviceScoro *d, int lampNr);
    void destroyLamp(DeviceScoro *d, int lampNr);
    void l1AddrChanged(QVariant val);
    void l1ApertureChanged(QVariant val);
    void l1SwChanged(QVariant val);
    void l2AddrChanged(QVariant val);
    void l2ApertureChanged(QVariant val);
    void l2SwChanged(QVariant val);
    void l3AddrChanged(QVariant val);
    void l3ApertureChanged(QVariant val);
    void l3SwChanged(QVariant val);

public slots:
    void doRescan();

private:
    bool m_isInitialized; /**< init indicator */
    bool m_isInDeleter; /**< inform not to delete the devices in the constructor */
    QMap<int, std::shared_ptr<DevicePara> > m_remoteParas; /**< Vector with deviceParameters */
    std::shared_ptr<DefaultPara> m_defaultPara;
    QList<std::weak_ptr<GenLamp> > m_lamps;
    int m_hasLamp[3];
    int m_oldWinkIndicator; /** temp storage of testbutton during winking */
    void initParas(int addr = 0, bool demo = false);
};

inline Device::eDeviceTypes DeviceScoro::getType() const{
    return Device::eDeviceTypes::eScoro;
}

inline float DeviceScoro::getApertureAct() const{
    return getPara(GEN_APERTURE)->data().toFloat();
}

inline ParaSelects::SpeedMode DeviceScoro::getSpeed() const{
    // return only 0 or 1 (normal or speed), direct cast of answer ok
    if(getName() == "Scoro 1600 E" || getName() == "Scoro 3200 E"){
        return ParaSelects::SpeedMode::notSupported;
    }

    return static_cast<ParaSelects::SpeedMode>(getPara(SPEED_SW)->data().toInt());
}

inline bool DeviceScoro::isInit() const{
    return m_isInitialized;
}

inline void DeviceScoro::setInit(bool val){
    m_isInitialized = val;
}

inline bool DeviceScoro::setName(const QString &){
    // not supported by scoro
    return false;
}

inline int DeviceScoro::getStandby() const{
    return getPara(STANDBY_SW)->data().toInt();
}

inline std::shared_ptr<DevicePara> DeviceScoro::getPara(int id) const{
    return m_remoteParas.value(id, m_defaultPara);      // dont give nullptr in case of error otherwise crash will haplen, create a dummy device that logs the invalid id
}

inline QString DeviceScoro::className() const{
    return DeviceScoro::staticMetaObject.className();
}

inline int DeviceScoro::getStudioAddress() const{
    // take care that this para is valid
    return getPara(STUDIO_ADDR)->data().toInt();
}

inline bool DeviceScoro::hasLamp(int nr){
    if(nr > 2 || nr < 0){
        return false;
    }
    return m_hasLamp[nr];
}

inline int DeviceScoro::getNumConnectedLamps() const{
    return m_lamps.size();
}

inline void DeviceScoro::isInDeleter(bool isIn){
    m_isInDeleter = isIn;
}

inline bool DeviceScoro::sendCmdSequenceEnd(){
    emit cmdSequenceDone(shared_from_this(), Errors::Id::E_NO_ERROR);
    return true;
}

inline void DeviceScoro::doRescan(){
    getPara(UPDATE_SERVICE)->getRemote();
}

inline int DeviceScoro::getModLight() const{
    return getPara(PILOTLIGHT_SW)->data().toInt();
}

inline bool DeviceScoro::setModLight(int val){
    return getPara(PILOTLIGHT_SW)->setRemote(val);
}

inline int DeviceScoro::getModLightMode() const{
    return getPara(PILOTLIGHT_MODE)->data().toInt();
}

inline bool DeviceScoro::setModLightMode(int val){
    return getPara(PILOTLIGHT_MODE)->setRemote(val);
}

inline void DeviceScoro::setDefaultParaId(int id){
    m_defaultPara->setId(id);
}

inline QString DeviceScoro::getRemoteName(){
//    return getPara(DEVICE_NAME)->data().0toString();
    return getName();
}

inline float DeviceScoro::getDelay() const{
    return getPara(DELAY)->data().toFloat();
}

inline bool DeviceScoro::setDelay(float val){
    return getPara(DELAY)->setRemote(val);
}

inline bool DeviceScoro::setStudioAddress(int val){
    return getPara(STUDIO_ADDR)->setRemote(val);
}

inline bool DeviceScoro::setActApertureEnergy(float val){
    return getPara(GEN_APERTURE)->setRemote(val);
}

inline bool DeviceScoro::setAudio(int val){
    return getPara(RDY_SOUND_VOLUME)->setRemote(val);
}

inline int DeviceScoro::getAudio() const{
    return getPara(RDY_SOUND_VOLUME)->data().toInt();
}

inline int DeviceScoro::getSWVersion() const{
    return getPara(AB_SW_VER)->data().toInt();
}

inline float DeviceScoro::getFStopRange() const{
    return getPara(F_STOP_RANGE)->data().toFloat();
}

inline int DeviceScoro::getSequence() const{
    return getPara(SEQUENCE)->data().toInt();
}

inline bool DeviceScoro::setSequence(int var) const{
    return getPara(SEQUENCE)->setRemote(var);
}

inline bool DeviceScoro::setTest() const{
    return getPara(FLASH_TEST)->setRemote(1);
}

inline float DeviceScoro::getPauseTime() const{
    return getPara(INTERVAL)->data().toFloat();
}

inline bool DeviceScoro::setPauseTime(float val){
    return getPara(INTERVAL)->setRemote(val);
}

inline int DeviceScoro::getFlashTimeSetting() const{
    return getPara(T01)->data().toInt();
}

inline bool DeviceScoro::setFlashTimeSetting(int val){
    getPara(T01)->setRemote(val);
    getPara(T01_VALUE)->getRemote();
    return true;
}

inline int DeviceScoro::getFlashDuration() const{
    return getPara(T01_VALUE)->data().toInt();
}

inline int DeviceScoro::getSlowCharge() const{
    return getPara(SLOW_CHARGING)->data().toInt();
}

inline bool DeviceScoro::setSlowCharge(int val){
    return getPara(SLOW_CHARGING)->setRemote(val);
}

inline int DeviceScoro::getNumLamps() const{
    return getPara(NUM_LAMP_SLOTS)->data().toInt();
}

inline QString DeviceScoro::getSSID() const{
    return getPara(NW_SSID)->data().toString();
}

inline bool DeviceScoro::setSSID(QString val){
    return getPara(NW_SSID)->setRemote(val);
}

inline QString DeviceScoro::getPWD() const{
    return getPara(NW_PWD)->data().toString();
}

inline bool DeviceScoro::setPWD(QString val){
    return getPara(NW_PWD)->setRemote(val);
}

inline bool DeviceScoro::setJoinNetwork(int val){
    return getPara(NW_JOIN)->setRemote(val);
}

inline bool DeviceScoro::setColorTemp(int val){
    return getPara(COLOR_TEMP)->setRemote(val);
}

inline int DeviceScoro::getColorTemp() const{
    return getPara(COLOR_TEMP)->data().toInt();
}

inline bool DeviceScoro::setDimmer(int val){
    return getPara(REDUCED_PILOTLIGHT)->setRemote(val);
}

inline int DeviceScoro::getDimmer() const{
    return getPara(REDUCED_PILOTLIGHT)->data().toInt();
}

inline bool DeviceScoro::setCellSensitivity(int val){
    return getPara(SENSITIVITY_PHOTO_CELL)->setRemote(val);
}

inline int DeviceScoro::getCellSensitivity() const{
    return getPara(SENSITIVITY_PHOTO_CELL)->data().toInt();
}

inline bool DeviceScoro::setCell(int val){
    return getPara(CELL_SW)->setRemote(val);
}

inline int DeviceScoro::getCell() const{
    return getPara(CELL_SW)->data().toInt();
}

inline bool DeviceScoro::setIR_RFS(int val){
    return getPara(IR_RFS_SW)->setRemote(val);
}

inline int DeviceScoro::getIR_RFS() const{
    return getPara(IR_RFS_SW)->data().toInt();
}

inline bool DeviceScoro::setBrightnessTest(int val){
    return getPara(TEST_KEY_INTENSITY)->setRemote(val);
}

inline int DeviceScoro::getBrightnessTest() const{
    return getPara(TEST_KEY_INTENSITY)->data().toInt();
}

inline bool DeviceScoro::setRdySoundMode(int val){
    return getPara(RDY_SOUND_MODE)->setRemote(val);
}

inline int DeviceScoro::getRdySoundMode() const{
    return getPara(RDY_SOUND_MODE)->data().toInt();
}

inline bool DeviceScoro::setDisplayIntensity(int val){
    return getPara(DISPLAY_INTENSITY)->setRemote(val);
}

inline int DeviceScoro::getDisplayIntensity() const{
    return getPara(DISPLAY_INTENSITY)->data().toInt();
}

inline bool DeviceScoro::resetDailyCounter(){
    return getPara(RESET_FLASH_COUNTER)->setRemote();
}

inline int DeviceScoro::getDailyCounter() const{
    return getPara(DAILY_FLASH_COUNT)->data().toInt();
}

inline int DeviceScoro::getOverallCounter() const{
    return getPara(TOTAL_FLASH_COUNT)->data().toInt();
}

inline int DeviceScoro::getSynSequence() const{
    return getPara(CELL_SW)->data().toInt();
}

//inline bool DeviceScoro::setSynSequence(int){
//    int set = 0;
//    switch(val){
//    case static_cast<int>(ParaSelects::SequenceModes::DISABLED): set = 0; break;
//    case static_cast<int>(ParaSelects::SequenceModes::MASTER): set = 0; break;
//    case static_cast<int>(ParaSelects::SequenceModes::SLAVE): set = 1; break;
//    }
//    QExplicitlySharedDataPointer<CommandSequence> seq = startCommandSequence();
//    getPara(RFS_SW)->setRemote(set == 0 ? 1 : 0);
//    getPara(IR_SW)->setRemote(set == 0 ? 1: 0);
//    getPara(CELL_SW)->setRemote(set);
//    endCommandSequence(seq);
//}

inline float DeviceScoro::getMaxEnergy() const{
    return getPara(SCORO_ENERGY_TYPE)->data().toFloat();
}

inline float DeviceScoro::getApertureMax() const{
    // DISPLAY_SPEED_APERTRUE_9 seems to be wrong at the device.
    // should correspondent with speed
    return getPara(F_STOP_MAX)->data().toFloat();
}

inline void DeviceScoro::setPolling(bool){
    Device::setPolling(false);
    // do nothing, scoros are not polled
}

inline bool DeviceScoro::isPolling() const{
    return false;
}

inline bool DeviceScoro::checkCommandwoResponse(int id){
    return (static_cast<Ids>(id) == Ids::FLASH_TEST);
}

inline bool DeviceScoro::isTestFlash(int id){
    return (static_cast<Ids>(id) == Ids::FLASH_TEST);
}

inline void DeviceScoro::startUpdateService(){
    emit startRescanTimer();
}

inline bool DeviceScoro::isUpdateService(int id){
    return (static_cast<Ids>(id) == Ids::UPDATE_SERVICE);
}

inline void DeviceScoro::activateWink(){
    m_oldWinkIndicator = getPara(TEST_KEY_INTENSITY)->data().toInt();
    getPara(TEST_KEY_INTENSITY)->setRemote(m_oldWinkIndicator ? 0 : 1);
}

inline void DeviceScoro::deactivateWink(){
    getPara(TEST_KEY_INTENSITY)->setRemote(m_oldWinkIndicator);
}

inline void DeviceScoro::changeWink(bool ena){
    getPara(TEST_KEY_INTENSITY)->setRemote(m_oldWinkIndicator ? !ena : ena);
}

inline int DeviceScoro::iDStudioAddress() const{
    return STUDIO_ADDR;
}

inline int DeviceScoro::iDAudio() const{
    return RDY_SOUND_MODE;
}

inline int DeviceScoro::getDeviceType() const{
    return Device::eTypes::eGeni;
}

inline QList<std::weak_ptr<GenLamp> > DeviceScoro::getLamps(){
    return m_lamps;
}

}

Q_DECLARE_METATYPE(Framework::DeviceScoro)

#endif // DEVICESCORO_H
