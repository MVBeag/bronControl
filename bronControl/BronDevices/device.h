/**
 * @author      Jörg Roth; Bron Elektronik AG
 * @date        12.07.2016
 * @version     0.0.90
 * @abstract base class for all bron devices, this class handles the communication with the remote device.
 * A device have parameters class DevicePara and Serivce (registered runtime functions). A DeviceParameter
 * normally has no dependencies. If functions use more than one parameter, a service is created. We use services
 * instead of impelemnted base class functions, because we want a small API.
 *
 * The device has the ability of automatic storing of changed parameters (the original value). To use this, feature, there is a small
 * helper class ParamLocker. In the visibility range of Para locker, all parameters are stored. This is useful, if you do big setups and want to
 * reset them. If you want to use this feature, do it like this:
 *
 * ...{
 *      ParamLocker(*this);
 *      getParam(para)->setRemote(value);
 *      ...
 *    }
 *
 * all setRemote(value) values are stored temporary. To restore this parameters, call restoreTempParas(), thats all.
 * It is important to know, that there is only one temp storage in the device. All Parameters, stored in the storage are written back in the call!!
 *
 * The communication is driven from paramters, if a setRemote/getRemote Para was successful, the value is stored
 * in the parameter, this causes a valuesChanged signal of the parameter. Within these mechanism, you can
 * use automatic data binding in qt/QML applications
 */


#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QTimer>
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QPointer>
#include "devicepara.h"
#include <QHostAddress>
#include "errors.h"
#include "remoteClientSM.h"
#include "paraSelectables.h"
#include "ethaddress.h"
#include "remoteaddressinfo.h"
#include <QDateTime>
#include "ipersistency.h"
#include <math.h>
#include "commandsequence.h"
#include "networkapi.h"


#define MW_DEVICE_DEFAULT_SERVICE_PORT      (8185)
#define MW_UDP_PORT_FOR_REQUESTS            MW_DEVICE_DEFAULT_SERVICE_PORT
#define MW_MAX_DEVICES                      (5)
#define MW_MAX_OUTSTANDING_REQUESTS         (2 * MW_MAX_DEVICES)

class NetworkAPI;
class NetworkAPIImpl;

namespace Framework{

class DevicePara;
class Device;
class ScanDevice;
class Request;
class ParamLocker;


/**
 * @brief The Device class is the abstract base class for accessing remote device functions. It acts as a kind of proxy. Beside
 * this, it holds all the parameters, the device has. A Parameter (DevicePara has no dependency to other parameters. If there are
 * parameters, the device has a function (here called service) to do the work.
 */
class Device : public QObject, public IPersistency, public std::enable_shared_from_this<Device>{
    Q_OBJECT
    friend class DevicePara; /** give DevicePara access to members */
    friend class RemoteClientSM;
    friend class ::NetworkAPI;
    friend class ::NetworkAPIImpl;
    friend class ParamLocker;

public:

    enum eTypes{
        eCompact,
        eGeni,
        eGeniLamp
    };
    Q_ENUM(eTypes)

    enum eSettings{
        eRetries = 3,
        eAutodeleteTimeout = 20000
    };

    // the eDeviceTypes has to contain all supported devices used as reflection of the derived class
    enum eDeviceTypes{
        eScan,
        eCreator,
        eSiros,
        eSirosS,
        eSirosL,
        eScoro,
        eGenLamp
    };


    explicit Device(bool demo);
    explicit Device();
    virtual ~Device();


    /**
     * @brief getType get the device type, can used as fast reflection function in derived classes
     * @return type, one of eDeviceTypes
     */
    virtual eDeviceTypes getType() const = 0;

    /**
     * @brief setHostAddress setter in one call
     * @param ha
     * @param port
     * @param resolveMac true for mac address resolution
     */
    void setHostAddress(const QHostAddress &ha, const uint16_t port, bool resolveMac = true);

    /**
     * @brief getHostAddress getter
     * @return QHostAddress of the device
     */
    const QHostAddress getHostAddress() const;

    /**
     * @brief getHostPort getter
     * @return the actual HostPort
     */
    uint16_t getHostPort() const;

    /**
     * @brief setParameter write a parameter to the remote device
     * @param id communication identifier for the parameter
     * @param val value to send
     * @return
     */
    bool setParameter(int id, const QVariant &val);

    /**
     * @brief Test function, typically initiates a flash
     */
    virtual bool setTest() const;

    /**
     * @brief getRetries return the acutal max retry count for the communication
     * @return device retries in case of a communication problem
     */
    int getRetries() const;

    /**
     * @brief setRetries sets the communication retry count, default is 3
     * @param retries
     */
    void setRetries(int retries);

    /**
     * @brief retryRequest put a request to the retry queue
     * @param r
     */
    void retryRequest(Request *r);

    /**
     * @brief getRequests delivers the actual running requests
     * @return
     */
    int getRequests() const;

    /**
     * @brief incRequests increments the actual running requests
     */
    void incRequests();

    /**
     * @brief decRequests decrements the actual running requests, request counter is always >= zero
     */
    void decRequests();

    /**
     * @brief isMaxRequestsReached check if more requests are allowed. The behaviour is dependent of the device implementation. Up to now,
     * Bron devices are able to buffer up to ten parallel requests. Typically a amount of requests are occupied with polling requests.
     * isMaxRequestReached, limits the max number of polling requests to five (hard coded)
     * @return true if five polling requests are in progress, false if not
     */
    bool isMaxRequestsReached() const;
    virtual void setRequestLimit(int limit);


    /**
     * @brief isPolling check if parameter polling is enabled
     * @return true if polling is active, false if not
     */
    virtual bool isPolling() const;

    /**
     * @brief setPolling enables or disables polling of the parameters
     * if polling is disabled, the autodelete mechanism is switched off
     * @param val true -> polling switched on, false -> polling off
     */
    virtual void setPolling(bool val);

    /**
     * @brief getCommState return the acutal commState
     * @return
     */
    ParaSelects::DeviceCommState getCommState() const;

    /**
     * @brief getRemoteName
     * @return returns the name, the device delivers, be careful, this might not be the device name
     */
    virtual QString getRemoteName();

    /**
     * @brief getLampAddress
     * @return return the lamp address of the device, if no lamp address is available, -1 is returned
     */
    virtual int getLampAddress() const;

    /**
     * @brief setLampAddress sets the lampAddress of the device, lampAddress is used to assign the device to a group
     * @param val new lampAddress (range could be device dependent)
     * @return
     */
    virtual bool setLampAddress(int val);

    /**
     * @brief getCountLowEnergyFlash
     * @return return the number of flash
     */
    virtual int getCountLowEnergyFlash() const;

    /**
     * @brief getDelay return the delay from trigger to flash in ms
     * @return
     */
    virtual float getDelay() const;
    /**
     * @brief setDelay sets the delay from trigger signal to flash in ms
     * @param val
     * @return
     */
    virtual bool setDelay(float val);

    /**
     * @brief studioAddress
     * @return return the studio address of the device, the studio address denotes to which studio the device belongs second criteria
     * after the choosen network. All devices within the same network with the same StudioAddress belonging to the same studio
     */
    virtual int getStudioAddress() const;

    /**
     * @brief setStudioAddress sets the studio address
     * @param val new studio address, (range is device dependent)
     * @return
     */
    virtual bool setStudioAddress(int val);

    /**
     * @brief setName setter for device Name emits NameChanged event. In cases, were the device doesn't support name change, this parameter
     * is stored locally
     * @param name
     */
    virtual bool setName(const QString &name);

    /**
     * @brief Name getter
     * @return
     */
    virtual QString getName() const = 0;

    /**
     * @brief IsOnline returns the online status
     * @return
     */
    bool IsOnline() const;

    /**
     * @brief SetOnline sets the online state and starts the worker thread
     * @param val
     */
    void setOnline(bool val);

    /**
     * @brief init call after constructor (not in, causes an debug error)
     */
    void init();

    /**
     * @brief getApertureMin returns the minimal aperture of the device
     * @return
     */
    virtual float getApertureMin() const final;

    /**
     * @brief getApertureMax returns the max apertures 10.0f
     * @return
     */
    virtual float getApertureMax() const;

    /**
     *  @brief getApertureAct returns the act energy setting of the device in apertures
     */
    virtual float getApertureAct() const;

    virtual bool setActApertureEnergy(float val);

    /**
     * @brief set/getAudio returns the audio settings of the device
     * 0 --> off
     * 1 --> on, depends on device
     */
    virtual bool setAudio(int val);
    virtual int getAudio() const;

    /**
     * @brief speed returns the speed settings of the device
     * @return 0 --> off; other are device specific
     */
    virtual ParaSelects::SpeedMode getSpeed() const;

    /**
     * @brief setSpeed controls the speed settings of the device
     * @param val
     * @return true in case of success, else false
     */
    virtual bool setSpeed(ParaSelects::SpeedMode val);

    /**
     * @brief setColorTemp controls the color temperature circuit
     * @param val device specific setting
     * @return
     */
    virtual bool setColorTemp(int val);

    /**
     * @brief getColorTemp returns the value of the color temp control
     * @return
     */
    virtual int getColorTemp() const;

    /**
     * @brief setDimmer controls the pilotlight reduction during charging
     * @param val 0 disabled, 1 enabled
     * @return true in case of success, else false
     */
    virtual bool setDimmer(int val);

    /**
     * @brief getDimmer the actual setting of the pilotlight reduction
     * @return 0 switched off, 1 active
     */
    virtual int getDimmer() const;

    /**
     * @brief setCellSensitivity controls the sensitivity of the photo coll
     * @param val
     * @return true if successful, fasle if not
     */
    virtual bool setCellSensitivity(int val);

    /**
     * @brief getCellSensitivity return the acutal adjusted photo cell sensitvity
     * @return
     */
    virtual int getCellSensitivity() const;

    /**
     * @brief setCell enabled or disables the photo coll
     * @param val 0 to switch off, 1 to switch on
     * @return true in case of success, else false
     */
    virtual bool setCell(int val);

    /**
     * @brief getCell returns if the photo cell is on or off
     * @return 0 for off, 1 for on
     */
    virtual int getCell() const;

    /**
     * @brief setRFS enables or disables the RFS communication
     * @param val 0 rfs/ir, 1->ir/--, 2->--/rfs, 3--/-- off
     * @return true if successful, else false
     */
    virtual bool setIR_RFS(int val);

    /**
     * @brief getRFS return if the RFS com channel is active
     * @return 0 rfs/ir, 1->ir/--, 2->--/rfs, 3--/-- off
     */
    virtual int getIR_RFS() const;

    /**
     * @brief setBrightnessTest control the brightness of the test button at the device
     * @param val 0 for bright, 1 for dimmed
     * @return true in case of success, else false
     */
    virtual bool setBrightnessTest(int val);

    /**
     * @brief getBrightnessTest get the actual brightness setting of the test button
     * @return 0 for bright, 1 for dimmed
     */
    virtual int getBrightnessTest() const;

    /**
     * @brief setRdySoundMode conrols the actual mode for the ready sound signal
     * @param val
     * @return
     */
    virtual bool setRdySoundMode(int val);

    /**
     * @brief getRdySoundMode returns the acutal setting for ready sound signal
     * @return
     */
    virtual int getRdySoundMode() const;

//    virtual bool setKeySoundVol(int val);
//    virtual int getKeySoundVol() const;

    /**
     * @brief setDisplayIntensity controls the display intensity
     * @param val device dependent
     * @return true in case of success, else false
     */
    virtual bool setDisplayIntensity(int val);

    /**
     * @brief getDisplayIntensity returns the actual setting of the display intensity control
     * @return
     */
    virtual int getDisplayIntensity() const;

    /**
     * @brief resetDailyCounter resets the daily flash counter
     * @return
     */
    virtual bool resetDailyCounter();

    /**
     * @brief getDailyCounter get the actual daily flash counter
     * @return
     */
    virtual int getDailyCounter() const;

    /**
     * @brief getOverallCounter get the overall flash counter
     * @return
     */
    virtual int getOverallCounter() const;

    /**
     * @brief isAvailable check if the device is not in standby, is initialized and has no warnings
     * @return true if the above is valid, false if not
     */
    bool isAvailable() const;

    /**
     * @brief GetAllParas
     * @return a vecor containing all paras
     */
    virtual QList<std::shared_ptr<DevicePara> > GetAllParas() const = 0;

    /**
     * @brief GetInitParas
     * @return a vector containing the init paras, read first
     */
    virtual QList<std::shared_ptr<DevicePara> > GetInitParas()const = 0;

    /**
     * @brief GetPollParas
     * @return a vector containing the polling paras
     */
    virtual QList<std::shared_ptr<DevicePara> > GetPollParas() const = 0;

    /**
     * @brief isInit checks if device was initialized
     * @return
     */
    virtual bool isInit() const = 0;

    /**
     * @brief setInit mark device as initialized
     * @param val
     */
    virtual void setInit(bool val) = 0;

    /**
     * @brief getStandby check if remote device is in standby
     * @return
     */
    virtual int getStandby() const;
    virtual bool setStandby(int val);

    /**
     * @brief getAlarm returns the actual alarm state of the device
     * @return
     */

    virtual ParaSelects::AlarmType getAlarm() const;

    /**
     * @brief GetParaLocal
     * @param id
     * @return local parameter without communication
     */
    virtual std::shared_ptr<DevicePara> getPara(int id )const = 0;

    /**
     * @brief getEthernetAddress
     * @return return the remote ethernet address, if not read (getIsDeviceAwareness() = false)
     * returns an empty string
     */
    QString getEthernetAddress() const;

    /**
     * @brief setEthernetAddress sets the remote ethernet address
     * @param ethernetAddress
     */
    void setEthernetAddress(const QString &ethernetAddress);

    /**
     * @brief getIsDeviceAwareness
     * @return true, if devices are identified by ethernet address
     * false if identified by ipaddress
     */
    bool getIsDeviceAwareness() const;

    /**
     * @brief setIsDeviceAwareness sets the device identification, on target Q_OS_IOS, is the
     * device awareness not running, due to the fact, that ios doesn't support this
     * @param isDeviceAwareness true: identified by ethernet address
     * false: identified by ip address
     */
    void setIsDeviceAwareness(bool isDeviceAwareness);

    /**
     * @brief setScanDevice controls, if we are scanning device (cyclic request  way to check if we are in scan device
     * @param val
     */
    void setScanDevice(bool val);

    /**
     * @brief startSM starts the underlying state machine with a signal, every device state machine runs in a own thread
     */
    void startFSM();

    /**
     * @brief className
     * @return returns the name of the class as readable string
     */
    virtual QString className() const;

    /**
     * @brief getDeviceType
     * @return the device type, overwrite this function if it is no compact (Siros) type. For generators, return eGeni, for generator Lamps, return eGeniLamp
     */
    virtual int getDeviceType() const;

    /**
     * @brief iDLampAddress can be used to compare if the command was concerning a lamp address. Needed because the ids are device specific
     * @return the id of the lamp address parameter
     */
    virtual int iDLampAddress() const;

    /**
     * @brief iDStudioAddress can be used to compare if the command was concerning the studio address. Needed because the ids are device specivic
     * @return
     */
    virtual int iDStudioAddress() const;

    /**
     * @brief iDAudio can be used to compare if the command was concerning the studio address. Needed because the ids are device specific
     * @return
     */
    virtual int iDAudio() const;


    /**
     * @brief increasePower function to increase power val
     * @param val increment typically 0.1 or 1.0 apertures
     * @return true in case of success, false if not
     */
    virtual bool increasePower(float val);

    /**
     * @brief decreasePower function to decrease power val
     * @param val decrement typically 0.1 or 1.0 apertures
     * @return
     */
    virtual bool decreasePower(float val);

    /**
     * @brief getChannel returns the channel of the connected lamp
     * @return [0 .. getNumLamps()-1]
     */
    virtual int getChannel() const;

    /**
     * @brief getMinEnergy returns the minimal energy in J
     * @return
     */
    virtual float getMinEnergy() const final;

    /**
     * @brief getMaxEnergy returns the maximal possible energy in J
     * @return
     */
    virtual float getMaxEnergy() const;

    /**
     * @brief getApertureJ
     * @return return the actual aperture in J
     */
    virtual float getActEnergy() const final;

    /**
     * @brief getModLight returns the state of the modeling light
     * @return 0 in case of off, 1 in case of on
     */

    virtual int getModLight() const;

    /**
     * @brief setModLight controls the modeling light
     * @param val 0 switches off, 1 switches on (at least)
     * @return
     */
    virtual bool setModLight(int val);

    /**
     * @brief getModLightMode get the working mode of the modeling light
     * @return
     */
    virtual int getModLightMode() const;

    /**
     * @brief setModLightMode sets the mode of the modeling light
     * @param val mode to set
     * @return
     */

    virtual bool setModLightMode(int val);

    /**
     * @brief IdString small helper to get the Ids enums as string, useful in debugging
     * @return String of enum
     */
    virtual const QString IdString(int id) const = 0;

    /**
     * @brief setDefaultParaId can be used for debugging purposes, if a parameter id
     * isn't found during search, a default para will be returned. This default para
     * lacks of the id. To overcome this, you can set the id to the default para before
     * you start the id search
     * @param id
     */
    virtual void setDefaultParaId(int id);

    /**
     * @brief getSWVersion returns the device firmware as int
     * @return
     */
    virtual int getSWVersion() const;

    /**
     * @brief triggerSwVersionReadout used to get the version at an identified timepoint
     * @return
     */
    virtual bool triggerSwVersionReadout();
    /**
     * @brief getFStopRange
     * @return returns the adjustable aperture range of the device
     */
    virtual float getFStopRange() const;

    /**
     * @brief getSequence
     * @return return the sequence common to all devices is 0 means off
     */
    virtual int getSequence() const;

    /**
     * @brief setSequence sets the sequence (series of flashes with a pause inbetween)
     * @param var number of flashes (limited by device)
     * @return
     */
    virtual bool setSequence(int var) const;

    /**
     * @brief getSynSequence
     * @return  get the mode for synchronized sequence
     */
    virtual int getSynSequence() const;

    /**
     * @brief setSynSequence set the mode for synchronized sequence
     * @param val
     * @return
     */
    virtual bool setSynSequence(int val);

    /**
     * @brief getPauseTime
     * @return the delay between the flashes in the sequence in ms
     */
    virtual float getPauseTime() const;

    /**
     * @brief setPauseTime sets the pause time in s
     * @param val
     * @return
     */
    virtual bool setPauseTime(float val);

    /**
     * @brief getFlashTimeSetting
     * @return returns the actual setting of the flash time control
     */
    virtual int getFlashTimeSetting() const;

    /**
     * @brief setFlashTimeSetting sets the actual setting of flash time control
     * @param val
     * @return true in case of success, false if not
     */
    virtual bool setFlashTimeSetting(int val);

    /**
     * @brief getFlashDuration returns the fractional part (n) of the flash duration n (1/n s)
     * @return
     */
    virtual int getFlashDuration() const;

    /**
     * @brief getSlowCharge returns the state of the charging circuit
     * @return
     */
    virtual int getSlowCharge() const;

    /**
     * @brief setSlowCharge controls the charging circuit
     * @param val
     * @return true in case of success, false if not
     */
    virtual bool setSlowCharge(int val);

    /**
     * @brief getApplication returns if a special application mode is switched on
     * @return
     */
    virtual int getApplication() const;

    /**
     * @brief setApplication controls freemask/alternate application number of devices
     * @param val
     * @return true in case of success, false if not
     */
    virtual bool setApplication(int val);

    /**
     * @brief getMaskGroup retuns the number of wait cycles (how many devices take pare 0,2,3,4)
     * 0 off, 2 every second flash (freemask), 3 every third flash and four every fourth flash
     * @return true in case of success, false if not
     */
    virtual int getMaskGroup() const;

    /**
     * @brief setMaskGroup controls the freemask/Altenate application
     * @param val 0->switched off, 2 every second flash, 3 every third flash and four every fourth flash
     * @return true in case of success, false if not
     */
    virtual bool setMaskGroup(int val);

    /**
     * @brief getNumLamps
     * @return return the number of "flashes"  the device has, e.g. Siros 1, Scoro [0..3]
     */
    virtual int getNumLamps() const;

    /**
     * @brief getSSID
     * @return returns the network name (if wifi, the ssid)
     */
    virtual QString getSSID() const;

    /**
     * @brief setSSID sets the desired network to use by name, on Wifi, typically ssid
     * @param val name of the network
     * @return true in case of success, false if not
     */
    virtual bool setSSID(QString val);

    /**
     * @brief getPWD returns the actual password of the network, the password is default or
     * given by the user via an remote interface, we don't read the operating sytems passwords
     * but they have to fit for a successful connection attempt
     * @return
     */
    virtual QString getPWD() const;

    /**
     * @brief setPWD sets the device password, used to log into the desired network
     * @param val passord as string
     * @return true in case of success, false if not
     */
    virtual bool setPWD(QString val);

    /**
     * @brief getEncryptionMode return the encryption mode (in case of Wifi)
     * @return WEP, WPA or WPA2
     */
    virtual int getEncryptionMode() const;

    /**
     * @brief setEncryptionMode sets the desired encryption mode for wifi network
     * @param val desired encryption mode
     * @return
     */
    virtual bool setEncryptionMode(int val);

    /**
     * @brief setJoinNetwork command switch to join the network, make
     * sure to setup correct befor with setSSID(), setPWD() and setEncryptionMode()
     * @param val
     * @return true in case of success, false if not
     */
    virtual bool setJoinNetwork(int val);

    /**
     * @brief getHiddenId get the static instance counter
     * @return instance of this object, can be used to local compare two devices (very fast) and
     * guaranteed to be unique (in the range of int)
     */
    int getHiddenId() const;

    /**
     * @brief getDemo get information, if device is a demo device (no real communication used)
     * @return true if demo, false if not
     */
    bool getDemo() const;

    /**
     * @brief Version
     * @return returns the version of the driver for the device
     */
    virtual const QString getDriverVersion() const { return "0.99.00";}

    /**
     * @brief getTag
     * @return the stored Tag, used to identify the position in the studio
     */
    virtual const QString getTag() const;

    /**
     * @brief setTag stores the tag in the device
     * @param tag
     * @return true in case of success, false if not
     */
    virtual bool setTag(const QString &tag);


    /**
     * @brief WriteData writes data to the udp socket, in addition to this, the checksum is added at the last byte, calculated with
     * crc8_messagecalc.
     * @param remoteAddress address to send
     * @param data with calculated checksum
     * @param length of the data (will be expanded by one because of the checksum)
     */
    void writeData(RemoteAddressInfo remoteAddress, const char* data, uint16_t length);

    /**
     * @brief migrateToNewNetwork a device network change, does not check if the change was successful
     * @param enterprize 0 for sapMode, 1 for enterprize mode
     * @param ssid of the target network
     * @param pw of the targete network
     * @param encr mode of the target network
     * @return 1 in case of success, 0 in case of an error, -1 in case of not supported
     */
    virtual int migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr);

    /**
     * @brief makeApplicationTransparent not all devices are able to take part within applications (e.g Scoro E in Sequence)
     * This function disables the device (normally switches off remote control -- total)
     */
    virtual void makeApplicationTransparent();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////// middleware related functions
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief setConnectionCBs internal function to interface the holy middleware
     * @param connectionCBs
     */
//    void setConnectionCBs(const MWConnectionCallbacks &connectionCBs);

    /* functions, provided by middleware
     * the next four functionc have two ways to signal, one is direct in the return value, if something goes wrong during
     * the call hyrachie it returns immediatly an error code (e.g. too many requests), if not, the application is informed via an callback (e.g Timout
     * or success)
     */

    /**
     * @brief GetRemoteParamInt starts a getRequest for an int parameter
     * @param req  request, storing some control values
     * @return Errors::Id
     */
    Errors::Id GetRemoteParamInt(Request *req);

    /**
     * @brief GetRemoteParamString starts a getRequest for an string parameter
     * @param req request, storing some control values
     * @return Errors::Id
     */
    Errors::Id GetRemoteParamString(Request *req);

    /**
     * @brief SetRemoteParamInt starts a setRequest for an int parameter
     * @param nSetValue value to set
     * @param req request, storing some control values
     * @return Errors::Id
     */
    Errors::Id SetRemoteParamInt(uint32_t nSetValue, Request *req);

    /**
     * @brief SetRemoteParamString starts a setRequest for an string parameter
     * @param cpBuffer buffer to send
     * @param nBufferSize size of the buffer to transfer
     * @param req request, storing some control values
     * @return Errors::Id
     */
    Errors::Id SetRemoteParamString(const char* cpBuffer,uint16_t nBufferSize,Request *req);

    /**
     * @brief initMW initializes the holy middleware, precondition is that a hostAddress is set, if not, no init will be done
     */
    void initMW();

    /**
     * @brief startCommandSequence starts a sequence, all commands between s = startCommandSequence() and
     * endCommandSequence(s), are belonging to a sequence. During a sequence, no polling is done. The sequence is
     * ready, if all commands got a response. After this, an signal sequenceReady(int s, bool error) is called.
     * Commands are doning retries, after three retries without an response, sequence ready with error true is called.
     * This function stops the polling requests temporary (if used), polling is reenabled if the last parameter of the sequence
     * got a response (or timeout)
     * @param name of the sequence
     * @return the sequence, use to end the sequence. All startCommandSequence() functions has to be finished
     * with an endCommandSequence(sequence).
     */
    QExplicitlySharedDataPointer<CommandSequence> startCommandSequence(QString name);

    /**
     * @brief endCommandSequence stops the sequence command storing
     * @param seq sequence given by startCommandSequence
     */
    void endCommandSequence(QExplicitlySharedDataPointer<CommandSequence> seq);

    /**
     * @brief getInterfaceIx
     * @return the index of the used interface (os specific)
     */
    uint getInterfaceIx() const;

    /**
     * @brief setInterfaceIx set the index of the used interface
     * @param ix
     */
    void setInterfaceIx(uint ix);

    ///////////////////////////////////////////////////////////////////////
    // implementation of IPersistency
    ///////////////////////////////////////////////////////////////////////
    virtual void read(const QJsonObject &device) override;
    virtual void write(QJsonObject &device) const override;
    virtual void ignore() override{}
    virtual void abort() override{}
    virtual void rollback() override{}

    virtual void response(Errors::Id errorCode, uint32_t nId, QVariant data,Request *req);

    /**
     * @brief comCaller makes real calls to remote device,
     * @param r Request to call
     * @return NO_ERROR in case of no error, later is callback called to inform the generator of the request
     *         MANY_REQUESTS in case of to much requests to --> slow down
     */
    Errors::Id comCaller(Request *r);

    void initCommunication();
    /**
     * @brief Shutdown has to be called as long at least one shared ptr of this device exists!!
     */
    void Shutdown();


    /* functions, provided by middleware */
    Errors::Id GetRemoteParamInt(RemoteAddressInfo deviceAddress, Request* req);
    Errors::Id GetRemoteParamString(RemoteAddressInfo deviceAddress, Request* req);
    Errors::Id SetRemoteParamInt(RemoteAddressInfo deviceAddress, uint32_t nSetValue, Request* req);
    Errors::Id SetRemoteParamString(RemoteAddressInfo deviceAddress, const char* cpBuffer, uint16_t nBufferSize, Request* req);

    Errors::Id LocalSetParamString(uint32_t, const char*, uint16_t); /* sLocalSetParamString */
    Errors::Id LocalGetParamString(uint32_t, char*, uint16_t, uint16_t*);
    Errors::Id LocalSetParamInt(uint32_t nId, uint32_t nSetValue);
    Errors::Id LocalGetParamInt(uint32_t nId, uint32_t* pRetValue);


    /**
     * @brief receiveData is called when receiving data
     * @param remoteAddress address from
     * @param data received
     * @param length length of the data received
     */
    void receiveData(RemoteAddressInfo remoteAddress, const char* data, uint16_t length);
    void CloseCallback(Errors::Id errorCode, RemoteAddressInfo connectionID);
    /**
     * @brief outstandingRequestTimeout called if a timeout occours
     * @param req with the timeout
     */
    void outstandingRequestTimeout(Request* req);

    virtual bool setCogniEnable(bool ena);
    virtual int getCogniEnable() const;
    /**
     * @brief setRemoteControl sets the remote control channel and stores the old settings
     * @param ctrl one of ParaSelects::RemoteCtrl
     * @return
     */
    virtual bool setRemoteControl(ParaSelects::RemoteCtrl ctrl);
    /**
     * @brief getRemoteControl
     * @return returns the actual state of the rfs on (1) or off (0)
     */
    virtual int getRemoteControl() const;
    /**
     * @brief storeRemoteControl stores the actual remoteControl parameter
     */
    void storeRemoteControl();
    /**
     * @brief restoreRemoteControl restores the old remoteControl para back
     */
    void restoreRemoteControl();
    /**
     * @brief startScanTimer newer firmware of bronDevices allows a update service, which informs you about parameters to read out because of change! With this, the device
     * don't need to be polled (dramatically reduces communication overhead). Use this function to start the scan process
     */
    virtual void startScanTimer();

    /**
     * @brief removeRemoteParan removes a registered parameter (typical in response: "PARAM_INEXISTENT"
     * @param id of the parameter
     */
    virtual void removeRemotePara(int id);

    /**
     * @brief storePara checks if the para should be temp stored, if yes, it saves the actual value withing the temp parameter storage
     * @param para to save
     */
    void storePara(DevicePara *para);

    /**
     * @brief restoreTempParas resets the paras to the old settings after an application
     */
    void restoreTempParas();

    /**
     * @brief stopBackgroundUpdate disables the polling and/or update service
     */
    void stopBackgroundUpdate();

    /**
     * @brief startBackgroundUpdate resets the polling and/or update service to the old values
     */
    void startBackgroundUpdate();

    /**
     * @brief checkCommandwoResponse checks if the command don't need a response (test flash)
     * @param id to check
     * @return true if command needs no response, false if command need a response
     */
    virtual bool checkCommandwoResponse(int id);

    /**
     * @brief isUpdateService check if id is an update service
     * @param id to check
     * @return true if it is an update service
     */
    virtual bool isUpdateService(int id);

    /**
     * @brief startUpdateService for devices, supporting the update service, this function starts the service
     */
    virtual void startUpdateService();

    /**
     * @brief isTestFlash checks if the id was a test flash
     * @param id to check
     * @return true if id was test flash, false if not
     */
    virtual bool isTestFlash(int id);


    /**
     * @brief areOpenSequences checks if there are sequences open
     * @return
     */
    bool areOpenSequences() const;

    /**
     * @brief sendLost small helper to emit a lostDevice
     * @return
     */
    virtual bool sendLost();

    /**
     * @brief freeze stops everything behind (devicetreads, communication, ...)
     */
    void freeze();

    /**
     * @brief unfreeze restarts everything behind (device threads, communication, ...)
     */
    void unfreeze();

    /**
     * @brief correctNetworkSettings some siros devices doesn't get the correct network settings
     * on default (if no network was set in the past), so do it manually in this function
     */
    virtual void correctNetworkSettings();

    // functions for handling optical feedback in some cases
    void startWink();
    void stopWink();
    void toggleWink();

    virtual void activateWink();
    virtual void deactivateWink();
    virtual void changeWink(bool ena);

signals:
    /**
     * @brief startWork signal to start doWork in worker
     */
    void startWork();
    /**
     * @brief stopWork signal to stop the worker
     */
    void stopWork();

    /**
     * @brief handleRx signal FSM that an response was received
     */
    void handleRx(RemoteAddressInfo ri, QByteArray datagram);


    /**
     * @brief onlineChanged emittet if online status is changed
     */
    void onlineChanged(const QVariant &val = QVariant(false));

    /**
     * @brief initParasRead is signaled after all as init marked deviceParas (DevicePara::settings::eInit) Flag
     * are read out --> local database is not correct!
     */
    void initParasRead(std::weak_ptr<Device> d);

    /**
     * @brief rxResponse is emitted on every real response
     */
    void rxResponse();

    /**
     * @brief commStateChanged emitted if the com state changes
     * @param state
     */
    void commStateChanged(ParaSelects::DeviceCommState state);

    /**
     * @brief deviceLost is emitted if no telegramms from the connected host are
     * are received after 10 seconds
     */
    void deviceLost(std::weak_ptr<Device> d);

    /**
     * @brief dataDataChanged called if data of this device has changed
     * @param d
     */
    void deviceDataChanged(std::weak_ptr<DevicePara> dp, std::weak_ptr<Device> d);

    /**
     * @brief udpReadData signaled, it new udp data was received for this device
     * @param ri address info of the sender
     * @param datagram bytes received
     */
    void udpReadData(RemoteAddressInfo ri, QByteArray datagram);

    /**
     * @brief queues are belonging to the state machine, the state machine is running in a own thread, therefore we communicate via signals
     * (threadsafe)
     */

    /**
     * @brief enqueueInitQueue signal to put an element to the init queue
     * @param r
     */
    void enqueueInitQueue(Request *r);

    /**
     * @brief enqueuePollingQueue signal to put an element to the polling queue
     * @param r
     */
    void enqueuePollingQueue(Request *r);

    /**
     * @brief enqueueCommandQueue signal to put an element to the command queue
     * @param r
     */
    void enqueueCommandQueue(Request *r);

    /**
     * @brief enqueueRetryQueue signal to put an element to the retry queue. Retries are only initiated for init and command parameters, polling
     * paras are cyclic polled!
     * @param r
     */
    void enqueueRetryQueue(Request *r);

    /**
     * @brief enqueueSequenceQueue signal to put an element to the sequence queue
     * @param r
     */
    void enqueueSequenceQueue(Request *r);


    /**
     * @brief dequeueRunningRequest signal to remove an element (id) from the running request queue
     * @param id
     */
    void dequeueRunningRequest(int id);

    void send(RemoteAddressInfo remoteAddress, QByteArray, uint16_t length);

    /**
     * @brief startRescanTimer starts the rescan mechanism
     */
    void startRescanTimer();

    /**
     * @brief stopRescanTimer stops the rescan mechanism
     */
    void stopRescanTimer();

    /**
     * @brief error used to signal an error
     * @param text error text
     */
    void error(QString text);

    void cmdSequenceDone(std::weak_ptr<Device> dev, Errors::Id error);

    /**
     * @brief removeParaFromQueue removes a parameter form the queue defined by type
     * @param type of the queue
     * @param id parameter id to remove
     */
    void removeParaFromQueue(Request::eQueueType type, uint32_t id);

    /**
     * @brief swVersionRead could be emitted after swVersion is read (from the parameter), has to be hard coded in the several device. Look at DeviceSiros.cpp
     * @param dev
     */
    void swVersionRead(std::shared_ptr<Device> dev);

public slots:

    /**
     * @brief paraDataChanged called, if a underlying devicePara has changed data
     * @param dp
     */
    void onDeviceParaDataChanged(std::weak_ptr<DevicePara> dp);

        /**
     * @brief Timeout slot called by state machine in cases we need to wait (wait time is 1000ms fix)
     */
    void Timeout();

    /**
     * @brief retriggerAutodeleteTimeout retriggers the autodelete monoflop
     */
    void retriggerAutodeleteTimeout();

    /**
     * @brief autoDeleteTimeout is called if no response is received any more
     */
    void autoDeleteTimeout();

    /**
     * @brief onWinkTimeout is used during search of a device (visual feedback)
     */
    void onWinkTimeout();

    /**
     * @brief onEthAddressResolved slot, called if the arp has resolved the ethernet address
     * @param eth
     */
    void onEthAddressResolved(ethAddress *eth);

    /**
     * @brief onSequenceDone is called, if the sequence with seqNr has finished (normally or with error)
     * @param seqNr number of the finished sequence
     * @param error true in case of error
     */
    void onSequenceDone(uint32_t seqNr, Errors::Id error);


private:
    static const float MAX_APERTURE; /**< max Aperture for device, per definition 10 up to now */
    bool m_isOnline;           /**< flag to signal if the device is online */
    bool m_demo;               /**< flag to signal if the device is in demo mode */
    RemoteClientSM m_sm;       /**< FSM controlling the communication */
    QThread *m_workerThread;   /**< thread for m_sm and m_com */
    int m_retries;             /**< max retries allowed for this device */
    int m_requests;            /**< running request counter */
    int m_reqLimit;            /**< allowec limt for requests */
    uint m_interfaceIx;        /**< used interface index (os) for communication */
    bool m_runThread;          /**< thread control flag */
    QTimer m_autoDeleteTimer;  /**< timer deleting the device if no date is recveived anymore */
    QTimer m_winkTimer;        /**< timer, used to search device e.g. blinking cogniColor or TestButton in case of scoro */
protected:
    QTimer *m_rescanTimer;      /**< timer used to scan for update service in newer devices */
private:
    QMap<int, QVariant> m_appParaStoreage; /**< in case of an application setup, here are the old settings stored */
    bool m_storeTempParas;     /**< control variable to inform device to store parameters temporary , use ParamLocker to control this */
    bool m_isScanDevice;       /**< flag to show if the device is a scanning device */
    bool m_isPolling;          /**< flag to signal, if the device is in polling mode */
    bool m_tPolling;           /**< stores the original polling flag during a sequence*/
    bool m_tRescanTimerActive; /**< holds the state of the rescan timer */
    int m_nestedSequencesLevel;/**< counts how often there are neseted sequences, update is started only at level 0 */
    int m_hiddenId;            /**< unique internal object, instance counter */
    ParaSelects::RemoteCtrl m_oldRemoteCtrl; /**< holds the remote control parameter at start of an application selection */
    bool m_winkActive;
    int m_winkCounter;
    std::weak_ptr<NetworkAPI> m_network;     /**< pointer to the real interface (holding the communication udpSocket */
    QString m_ethAddress;      /**< ethernet address as string */
    QHostAddress m_hostAddress; /**< host address of the remote device */
    uint16_t m_hostPort;       /**< host port */
    bool m_devAwareness;        /** deviceAwareness controls if comparissons are made by ethernet addresses */
    QJsonObject m_backup;     /** stores the actual parameter setting in a jsonObject */
    QMap<uint32_t, QExplicitlySharedDataPointer<CommandSequence> > m_runningSequences;
    uint32_t m_nLastSequence{0UL};
    QMap<uint32_t, Request *> m_runningRequests;
    uint32_t m_sequence;
    QMutex m_mutex;
    /**
     * @brief SetPara starts a remote communication
     * @param para parameter to communicate
     * @param type type of the parameter
     */
    virtual bool SetPara(std::weak_ptr<DevicePara> para, DevicePara::Types type, QList<std::function<bool()> > postCmd = QList<std::function<bool()> >());

    /**
     * @brief GetPara starts a remote communication
     * @param para parameter to communicate
     * @param type type of the parameter
     */
    virtual void GetPara(std::weak_ptr<DevicePara> para, DevicePara::Types type);

    /**
     * @brief construct helper function to have the stuff needed in the constructor only once
     */
    void construct();   


    Errors::Id SendRequest(Request* pRequest, RemoteAddressInfo deviceAddress, const char* cpBuffer, int nUsedBufferSize);
    void CreateServerConnection();


    Request* findOutstandingRequest(uint32_t nSequence);

};


/**
 * @brief The ParamLocker class eases the used of storing temporary parameters (e.g.: Application setups). In the visibility range of the
 * Locker, all paras are stored withing the tempStorage of the device.
 */

class ParamLocker{
public:
    explicit inline ParamLocker(Device &d)
        : m_device(d){
        m_device.m_storeTempParas = true;
    }
    inline ~ParamLocker(){
        m_device.m_storeTempParas = false;
    }

private:
    Device &m_device;
    ParamLocker(const ParamLocker &) = delete;
    ParamLocker &operator=(const ParamLocker &) = delete;
};

inline bool operator==(const std::weak_ptr<Device> &lhs, const std::weak_ptr<Device> &rhs){
    if(auto alhs = lhs.lock()){
        if(auto arhs = rhs.lock()){
            return arhs->getHiddenId() == alhs->getHiddenId();
        }
    }
    return false;
}

inline bool operator!=(const std::weak_ptr<Device> &lhs, const std::weak_ptr<Device> &rhs){
    return !(lhs==rhs);
}


/**
 * @brief operator ==
 * @param lhs
 * @param rhs
 * @return true, if HostAddress and HostPort are the same, otherwise false
 */
inline bool operator==(Device& lhs, Device& rhs){
    return lhs.getHiddenId() == rhs.getHiddenId();
}

/**
 * @brief operator !=
 * @param lhs
 * @param rhs
 * @return
 */
inline bool operator!=(Device& lhs, Device& rhs){ return !(lhs == rhs); }

inline bool Device::setTest() const{
    return false;
}

inline int Device::getRetries() const{
    return m_retries;
}

inline void Device::setRetries(int retries){
    m_retries = retries;
}

inline int Device::getRequests() const{
    return m_requests;
}

inline void Device::incRequests(){
    m_requests++;
}

inline void Device::decRequests(){
    if(m_requests > 0){
        m_requests--;
    }
}

inline bool Device::isMaxRequestsReached() const{
    return m_requests >= m_reqLimit;
}

inline void Device::setRequestLimit(int limit){
    m_reqLimit = limit;
}

inline bool Device::isPolling() const{
    return m_isPolling;
}

inline void Device::setPolling(bool val){
    m_isPolling = val;
    if(val || m_rescanTimer->isActive()){
        m_autoDeleteTimer.start();
    }
    else{
        m_autoDeleteTimer.stop();
    }
}

inline QString Device::getRemoteName(){
    return "undefined";
}

inline int Device::getLampAddress() const{
    return -1;
}

inline bool Device::setLampAddress(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getCountLowEnergyFlash() const{
    return -1;
}

inline float Device::getDelay() const{
    return -1;
}

inline bool Device::setDelay(float val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getStudioAddress() const{
    return -1;
}

inline bool Device::setStudioAddress(int val){
    Q_UNUSED(val);
    return false;
}

inline bool Device::setName(const QString &name){
    Q_UNUSED(name);
    return false;
}

inline float Device::getApertureMin() const{
    return getApertureMax() - getFStopRange();
}

inline float Device::getApertureAct() const{
    return 0.0f;
}

inline bool Device::setActApertureEnergy(float val){
    Q_UNUSED(val);
    return false;
}

inline bool Device::setAudio(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getAudio() const{
    return -1;
}

inline ParaSelects::SpeedMode Device::getSpeed() const{
    return ParaSelects::SpeedMode::notSupported;
}

inline int Device::getStandby() const{
    return -1;
}

inline bool Device::setStandby(int val){
    Q_UNUSED(val);
    return false;
}

inline ParaSelects::AlarmType Device::getAlarm() const{
    return ParaSelects::AlarmType::NONE;
}

inline void Device::onDeviceParaDataChanged(std::weak_ptr<DevicePara> dp){
    emit deviceDataChanged(dp, shared_from_this());
}

inline void Device::retriggerAutodeleteTimeout(){
    if(m_isPolling || m_rescanTimer->isActive()){
        m_autoDeleteTimer.start();
    }
}

inline void Device::autoDeleteTimeout(){
    if(m_isPolling || m_rescanTimer->isActive()){
        if(false == m_demo){
            emit deviceLost(shared_from_this());
        }
    }
}

inline void Device::onWinkTimeout(){
    toggleWink();
}

inline void Device::setScanDevice(bool val){
    m_isScanDevice = val;
}

inline QString Device::className() const{
    return QString();
}

inline int Device::getDeviceType() const{
    return eCompact;
}

inline int Device::iDLampAddress() const{
    return -1;
}

inline int Device::iDStudioAddress() const{
    return -1;
}

inline int Device::iDAudio() const{
    return -1;
}

inline bool Device::increasePower(float){
    return false;
}

inline bool Device::decreasePower(float){
    return false;
}

inline bool Device::setModLight(int){
    return false;
}

inline bool Device::setSpeed(ParaSelects::SpeedMode){
    return false;
}

inline bool Device::setColorTemp(int){
    return false;
}

inline int Device::getColorTemp() const{
    return -1;
}

inline bool Device::setDimmer(int){
    return false;
}

inline int Device::getDimmer() const{
    return -1;
}

inline bool Device::setCellSensitivity(int){
    return false;
}

inline int Device::getCellSensitivity() const{
    return -1;
}

inline bool Device::setCell(int){
    return false;
}

inline int Device::getCell() const{
    return -1;
}

inline bool Device::setIR_RFS(int){
    return false;
}

inline int Device::getIR_RFS() const{
    return -1;
}

inline bool Device::setBrightnessTest(int){
    return false;
}

inline int Device::getBrightnessTest() const{
    return -1;
}

inline bool Device::setRdySoundMode(int){
    return false;
}

inline int Device::getRdySoundMode() const{
    return -1;
}

inline bool Device::resetDailyCounter(){
    return false;
}

inline int Device::getDailyCounter() const{
    return -1;
}

inline int Device::getOverallCounter() const{
    return -1;
}

inline bool Device::setDisplayIntensity(int){
    return false;
}

inline int Device::getDisplayIntensity() const{
    return -1;
}

inline int Device::getChannel() const{
    return 0;
}

inline int Device::getModLight() const{
    return -1;
}

inline int Device::getModLightMode() const{
    return -1;
}

inline bool Device::setModLightMode(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getSWVersion() const{
    return -1;
}

inline bool Device::triggerSwVersionReadout(){
    return false;
}

inline float Device::getMaxEnergy() const{
    return 0.0f;
}

inline float Device::getFStopRange() const{
    return -1;
}

inline int Device::getSequence() const{
    return -1;
}

inline bool Device::setSequence(int var) const{
    Q_UNUSED(var);
    return false;
}

inline int Device::getSynSequence() const{
    return -1;
}

inline bool Device::setSynSequence(int val){
    Q_UNUSED(val);
    return false;
}

inline float Device::getPauseTime() const{
    return -1.0;
}

inline bool Device::setPauseTime(float val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getFlashTimeSetting() const{
    return -1;
}

inline bool Device::setFlashTimeSetting(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getFlashDuration() const{
    return -1;
}

inline int Device::getSlowCharge() const{
    return -1;
}

inline bool Device::setSlowCharge(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getApplication() const{
    return -1;
}

inline bool Device::setApplication(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getMaskGroup() const{
    return -1;
}

inline bool Device::setMaskGroup(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getNumLamps() const{
    return -1;
}

inline QString Device::getSSID() const{
    return QString();
}

inline bool Device::setSSID(QString val){
    Q_UNUSED(val);
    return false;
}

inline QString Device::getPWD() const{
    return QString();
}

inline bool Device::setPWD(QString val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getEncryptionMode() const{
    return -1;
}

inline bool Device::setEncryptionMode(int val){
    Q_UNUSED(val);
    return false;
}

inline bool Device::setJoinNetwork(int val){
    Q_UNUSED(val);
    return false;
}

inline int Device::getHiddenId() const{
    return m_hiddenId;
}

inline bool Device::getDemo() const{
    return m_demo;
}

inline const QString Device::getTag() const{
    return QString("not supported");
}

inline bool Device::setTag(const QString &tag){
    Q_UNUSED(tag);
    return false;
}

inline void Device::setDefaultParaId(int){
}

inline int Device::migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr){
    emit cmdSequenceDone(shared_from_this(), Errors::Id::E_NO_ERROR);
    Q_UNUSED(enterprize);
    Q_UNUSED(ssid);
    Q_UNUSED(encr);
    Q_UNUSED(pw);
    return -1;
}

inline void Device::makeApplicationTransparent(){
}

inline uint Device::getInterfaceIx() const{
    return m_interfaceIx;
}

inline void Device::setInterfaceIx(uint ix){
    m_interfaceIx = ix;
}

inline Errors::Id Device::LocalSetParamString(uint32_t, const char *, uint16_t){
    return Errors::Id::PARAM_INEXISTENT;
}

inline Errors::Id Device::LocalGetParamString(uint32_t, char *, uint16_t, uint16_t *){
    return Errors::Id::PARAM_INEXISTENT;
}

inline bool Device::setCogniEnable(bool){
    return false;
}

inline int Device::getCogniEnable() const{
    return -1;
}

inline bool Device::setRemoteControl(ParaSelects::RemoteCtrl){
    return false;
}

inline int Device::getRemoteControl() const{
    return -1;
}

inline void Device::storeRemoteControl(){
    m_oldRemoteCtrl = static_cast<ParaSelects::RemoteCtrl>(getRemoteControl());
}

inline void Device::restoreRemoteControl(){
    setRemoteControl(m_oldRemoteCtrl);
    m_oldRemoteCtrl = ParaSelects::RemoteCtrl::RcOff;
}

inline void Device::startScanTimer(){
}

inline void Device::removeRemotePara(int id){
    Q_UNUSED(id)
}

inline bool Device::checkCommandwoResponse(int){
    return false;
}

inline bool Device::isUpdateService(int){
    return false;
}

inline void Device::startUpdateService(){
}

inline bool Device::isTestFlash(int){
    return false;
}

inline bool Device::sendLost(){
    emit deviceLost(shared_from_this());
    return true;
}

inline void Device::correctNetworkSettings(){
    return;
}

inline void Device::startWink(){
    m_winkCounter = 9;
    m_winkActive = true;
    m_winkTimer.start();
    activateWink();
}

inline void Device::stopWink(){
    m_winkTimer.stop();
    m_winkActive = false;
    deactivateWink();
}

inline void Device::toggleWink(){
    m_winkActive = !m_winkActive;
    changeWink(m_winkActive);
    if(m_winkCounter-- == 0) stopWink();
}

inline void Device::activateWink(){
}

inline void Device::deactivateWink(){
}

inline void Device::changeWink(bool){
}

inline void Device::setEthernetAddress(const QString &val){
    m_ethAddress = val;
}

inline QString Device::getEthernetAddress() const{
    return m_ethAddress;
}


}

Q_DECLARE_METATYPE(std::weak_ptr<Framework::Device>)
Q_DECLARE_METATYPE(std::shared_ptr<Framework::Device>)
#endif // DEVICE_H
