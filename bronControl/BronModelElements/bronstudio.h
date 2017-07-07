/**
  * @brief BronStudio is the main entry point for the studio. This class handles also the devices and
  * groups (logically grouped devices by the so called lamp address).
  * This class hold a list of devices (m_deviceList -- contains all known devices in the studios) and a list
  * of groups (m_groupList -- all known groups in the device. The groups references also to the devices in
  * the device list. To take care about this references, insert a device first in the m_deviceList and after
  * that into the m_groupList. At removal, remove first from m_groupList and afterwards from m_deviceList.
  */



#ifndef BRONSTUDIO_H
#define BRONSTUDIO_H

#include <QObject>
#include <QString>
#include <QList>
#include <QNetworkSession>
#include <QNetworkConfiguration>
#include "brongroup.h"
#include "NetWorkScanner.h"
#include "deviceFinder.h"
#include "studiosequence.h"


namespace Framework{

/**
 * @brief The BronStudio class this class represents real studio objects,
 * after selecting a studio, every information passed to and from the user is handled via this class
 * it acts as a "view" to the underlying devices and groups,...
 *
 */

class BronStudio : public QObject{
    Q_OBJECT
public:

    enum class eNetworkState : uint8_t{
        eUnknown = 0,               //< not known to system,
        eKnownNotVisible,       //< known Config, but actual not visible (e.g. not in range)
        eKnownAccessable,       //< known, can be used to start a session
        eActive                 //< known and active
    };

    enum class eData : uint8_t{
        eAddress,
        eSpecialModes
    };

    enum eSpecialModes{
        eNone = 0,
        eSequence,
        eFreemask,
        eAlternate
    };

    BronStudio(bool demo = false, QObject *parent = NULL);
    BronStudio(const QNetworkConfiguration &n, bool demo, QObject *parent);
    ~BronStudio();

    /**
     * @brief Name retrieves the name of the module active yet
     * @return
     */
    virtual QString Name()const;

    /**
     * @brief setName sets the studio name
     * @param s name to set
     */
    bool setName(const QString &s);

    /**
     * @brief setNetwork sets a network configuration
     * @param n
     */
    void setNetwork(const QNetworkConfiguration &n);

    /**
     * @brief migrateToNewNetwork informs all underlying devices to swicht to the new network. The sap providing device is switched at last
     * in this case, the autodelete timer is not running, because the studios should stay active in an using business layer
     * @param enterprize true for enterpirze mode, false for private mode
     * @param ssid of the target network
     * @param pw of the target network
     * @param encr of the target network
     * @return true if commands are successful sended. if the devices are really migrating is not checked here! false if not
     */
    bool migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr);

    /**
     * @brief Ssid returns the used ssid as string
     * @return
     */
    QString Ssid() const;

    /**
     * @brief setSsid set the ssid
     * @param s ssid
     */
    bool setSsid(const QString s);

    /**
     * @brief Password returns the password used to lock into wifi
     * @return
     */
    QString Password() const;

    /**
     * @brief setPassword sets password for wifi
     * @param s
     */
    bool setPassword(const QString s);

    /**
     * @brief EncryptionMode
     * @return the selected encryption mode for this studio (WEP, WPA or WPA2)
     */
    int EncryptionMode() const;

    /**
     * @brief setEncrypionMode the desired encryption mode for this studio
     * @param mode
     */
    void setEncrypionMode(int mode);

    /**
     * @brief GetGroups delivers a List with the used groups (all items with the same logic address -- Bron convention
     * @return
     */
    QList<BronGroup *> &GetGroups();

    /**
     * @brief setAddress sets the address of this studio
     * @param addr
     */
    bool setAddress(const int addr);

    /**
     * @brief getAddress
     * @return the address of this studio
     */
    int getAddress() const;

    /**
     * @brief switchNetwork trys to start and switch the network of this studio, in case of this network is already running,
     * nothing happens. If a new network is successful startet, a networkSwitchec signal is emitted
     * @param online denotes if we want to go online (true) or switch off the connection (false)
     */
    void switchNetwork(bool online);

    /**
     * @brief addDevice adds a device to this studio, groups are handled
     * @param d device to add
     * @return true if the device was added successful, else if not (the device was already in)
     */
    bool addDevice(std::weak_ptr<Device> dev);

    /**
     * @brief removeDevice removed a device from this studio, groups are handled
     * @param d device to remove
     * @return true if successful removed, else false
     */
    bool removeDevice(std::weak_ptr<Device> dev);


    /**
     * @brief operator == compare operator for devcie search
     * @param s
     * @return true if equeal, false if not
     */
    bool operator==(const BronStudio &s) const;

    /**
     * @brief getOnline gets the online state of this studio, online is discovered and connected
     * @return true if online, false if not
     */
    bool getOnline()const;

    /**
     * @brief setOnline the online state of thie
     * @param val
     */
    void setOnline(bool val);

    /**
     * @brief getDevices
     * @return returns the devices, registered in this studio
     */
    QList<std::shared_ptr<Device> > getDevices();

    float getCurStudioPower() const;

    /**
     * Get the lowest cur value from the lamplist
     */
    float getMinCurStudioPower() const;

    /**
     * Get the highest cur value from the lamlplist
     */
    float getMaxCurStudioPower() const;

    /**
     * Calculate the lowst possible value from the lamplist
     * @param withOffset
     */
    float getMinStudioPower() const;

    /**
     * Get the highest possible power value from the lamplist
     */
    float getMaxStudioPower() const;

    /**
     * @brief setPolling enables or disables polling of remote parameters of the underlying devices in this studio
     * @param val true enables polling, false disables polling
     */
    void setPolling(bool val);

    /**
     * @brief getPolling
     * @return returns if the studio is allowed to poll parameters in the background
     */
    bool getPolling() const;

    /**
     * @brief isDeviceAlreadyIn check if device is already in studio
     * @param dev to check
     * @return true if in, false if not
     */
    bool isDeviceAlreadyIn(std::weak_ptr<Device> d);

    /**
     * @brief setDeviceAwareness set the behaviour of the devices in the studio
     * if true, devices are identified by ethernet address, if false, they are
     * identified by ip address (can change during startup)
     * @param val
     * @return
     */
    void setDeviceAwareness(bool val);
    bool getDeviceAwareness() const;


    bool setIncPower(float increment);
    bool setDecPower(double decrement);
    bool setSwitchModlight(int val);
    int getSwitchModlight();

    bool setStandby(int val);
    int getStandby();

    bool setSwitchSpeed(int val);
    int getSwitchSpeed();

    bool setCogniLight(int val);
    int getCogniLight();



    bool setTest(bool val);

    void deleteDevices();
    bool deleteDevice(std::weak_ptr<Device> dev);

    int netWorkState() const;

    bool setAudio(bool val);
    int getAudio();

    bool setApplication(QVariantMap val);
    int getApplication() const;

    bool setShowCogni();
//    int getShowCogni() const;

    bool setCogniEnable(bool ena);
    int getCogniEnable();

    /**
     * @brief setRemoteSwitch
     * @param sel one of ParaSelects::RemoteCtrl
     * @return
     */
    bool setRemoteSwitch(int sel);
    int getRemoteSwitch();

    bool setSequence(QVariantMap val);

    /**
     * @brief disableDumbDevices disables all devices, not able to take part in an application (e.g. Scoro E in Sequence)
     */
    void disableDumbDevices();

    /**
     * @brief startStudioSequence starts an sequence on the devices, between startStudioSequence and endStudioSequence
     * @param name of the sequence
     * @return StudioSequence class
     */
    std::shared_ptr<StudioSequence> startStudioSequence(QString name);

    /**
     * @brief endStudioSequence end the sequence
     */
    void endStudioSequence(std::shared_ptr<StudioSequence>);



    QNetworkConfiguration getNetwork() const;
    QNetworkInterface getInterface() const;

    uint getInterfaceIx() const;
    void setInterfaceIx(const uint &interfaceIx);

    /**
     * @brief getBackgroundUpdate get the state of the background update (studios are always consistent
     * to discovered devices --> background communiction)
     * @return true if background update on, else flase
     */
    bool getBackgroundUpdate() const;

    /**
     * @brief setBackgroundUpdate switch background update on or off for this studio
     * @param val
     */
    void setBackgroundUpdate(bool val);


    /**
     * @brief freeze stops everything behind (devicetreads, communication, ...)
     */
    void freeze();

    /**
     * @brief unfreeze restarts everything behind (device threads, communication, ...)
     */
    void unfreeze();


public slots:

    /**
     * @brief onInitParasRead used to insert into studio, it has to be an initialized device,
     * the management of the groups for example is based on the real device data
     * @param dev
     */
    void onInitParasRead(std::weak_ptr<Device> d);


    /**
     * @brief onDeviceLost slot if device is lost
     */
    void onDeviceLost(std::weak_ptr<Device> d);

    /**
     * @brief onDeviceDataChanged called if a device has changed data,
     * use it as filter to emit further signals dependend on data of your
     * application
     * @param dp
     * @param d
     */
    void onDeviceDataChanged(std::weak_ptr<DevicePara> dp, std::weak_ptr<Device> d);

    /**
     * @brief startDiscovery starts the discovery process for this studio
     */
    void startDiscovery();

    /**
     * @brief stopDiscovery stops the discovery process for this studio
     */
    void stopDiscovery();

    void onNetConfigChanged(const QNetworkConfiguration cfg);

    void onNetworkError(QNetworkSession::SessionError error);

    void onHigherEnergy(float val);
    void onLowerEnergy(float val);


    void onDevSequenceDone(std::shared_ptr<StudioSequence> dev);

    /**
     * @brief onNetworkStateChanged called if networkSession state is changed
     * @param state
     */
    void onNetworkStateChanged(QNetworkSession::State state);



signals:
    /**
     * @brief bronStudioNewDevice is called after adding a initialized device to the studio
     * @param d
     */
    void bronStudioNewDevice(std::weak_ptr<Device> d );

    /**
     * @brief newDeviceUp is called on end of the initialisation of the new found device, from this point, you can use
     * the device
     * @param d
     */
    void newDeviceUp(std::weak_ptr<Device>d);

    /**
     * @brief bronStudioGoneDevice a studio device is gone
     * @param d
     */
    void bronStudioGoneDevice(std::shared_ptr<Device> d);
    void bronStudioGroupAdded(BronGroup *g);
    void bronStudioGroupRemoved(BronGroup *g);
    void dataChanged(BronStudio::eData d);

    /**
     * @brief serviceError is emitted if a not existing service is called.
     * @param object classname as string
     * @param service name of the servce
     */
    void serviceError(QString object, QString service);

    /**
     * @brief netConfigChanged called, if the stored networkConfiguration has changed
     */
    void netConfigChanged();

    /**
     * @brief error is signaled
     * @param errorText describes the error (to user interface)
     */
    void error(QString errorText);

    void newEnergy(float val);

    /**
     * @brief studioEmpty signaled if after a device removal, no more devices are left
     */
    void studioEmpty();


private:
    bool removeDeviceFromGroup(std::weak_ptr<Device> dev);
    bool addDeviceToGroup(std::weak_ptr<Device> d);

    int m_address;  /**< studio address */
    bool m_isOnline;
    bool m_isPollingAllowed;
    bool m_isDeviceAwareness;
    bool m_isDemo;
    bool m_runBackgroundUpdate;
    QString m_name;
    QString m_ssid;
    QString m_password;
    int m_encr;
    eSpecialModes m_specialMode;

    //  THE ONLY INSTANCDE HOLDING THE ORIGINAL POINTERS AS SHARED PTRS
    QList<std::shared_ptr<Device> > m_deviceList; /**< main list for known devices in this studio */
    QList<BronGroup *> m_groupList; /**< main list for known groups */
    QList<std::shared_ptr<StudioSequence> > m_sequences;

    QNetworkConfiguration m_myNetworkConfig;
    QNetworkSession *m_mySession;
    uint32_t m_sequenceNr;
    uint m_interfaceIx;

};

inline bool operator== (const BronStudio &a, const BronStudio &b){
    return a.Name() == b.Name();
}

inline QString BronStudio::Name() const{
    return m_name;
}

inline bool BronStudio::setName(const QString &s){
    m_name = s;
    return true;
}

inline void BronStudio::setNetwork(const QNetworkConfiguration &n){
    m_myNetworkConfig = n;
    setSsid(n.name());
    // signal, that the configuration of the studio has changed, otherwise, other parts of the software
    // are not informed about this. Only changes are signaled! If the network belonging to this studio
    // is online already, this is not recognized without the following signal.
    emit netConfigChanged();
}

inline QString BronStudio::Ssid() const{
    return m_ssid;
}

inline bool BronStudio::setSsid(const QString s){
    m_ssid = s;
    return true;
}

inline QString BronStudio::Password() const{
    return m_password;
}

inline bool BronStudio::setPassword(const QString s){
    m_password = s;
    return true;
}

inline int BronStudio::EncryptionMode() const{
    return m_encr;
}

inline void BronStudio::setEncrypionMode(int mode){
    m_encr = mode;
}

inline QList<BronGroup *> &BronStudio::GetGroups(){
    return m_groupList;
}

inline int BronStudio::getAddress() const{
    return m_address;
}

inline bool BronStudio::operator==(const BronStudio &s) const{
    return this->Name() == s.Name();
}

inline bool BronStudio::getOnline()const{
    if(m_isDemo){
        if(m_name == "Bron-Demo0"){
            return true;
        }
        return m_isOnline;
    }
    else{
        return m_isOnline;// && m_myNetworkConfig.state().testFlag(QNetworkConfiguration::StateFlag::Active);
    }
}

inline QList<std::shared_ptr<Device> > BronStudio::getDevices(){
    return m_deviceList;
}

inline float BronStudio::getCurStudioPower() const{
    return getMinCurStudioPower();
}

inline bool BronStudio::getPolling() const{
    return m_isPollingAllowed;
}

inline bool BronStudio::getDeviceAwareness() const{
    return m_isDeviceAwareness;
}

inline QNetworkConfiguration BronStudio::getNetwork() const{
    return m_myNetworkConfig;
}

inline bool BronStudio::getBackgroundUpdate() const{
    return m_runBackgroundUpdate;
}

inline void BronStudio::setBackgroundUpdate(bool val){
    m_runBackgroundUpdate = val;
}

inline void BronStudio::onHigherEnergy(float val){
    setIncPower(val);
}

inline void BronStudio::onLowerEnergy(float val){
    setDecPower(val);
}


}

#endif // BRONSTUDIOS_H
