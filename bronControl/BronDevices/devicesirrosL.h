#ifndef DEVICESIRROSL_H
#define DEVICESIRROSL_H

#include <QObject>
#include "devicesirros.h"
#include "creatorDevice.h"

namespace Framework{

class DeviceSirosL : public DeviceSiros{
    Q_OBJECT
public:

    enum Ids{
        DEVICE_DESC               = PARAM_DEVICE_DESC,                  // s, g         ; json descripter
        STUDIO_ADDRESS            = PARAM_STUDIO_ADDRESS,               // i, g         ; studio address [1..99]
        NUM_OF_LAMPS              = PARAM_NUM_OF_LAMPS,                 // i, g         ; 1
        DEVICE_NAME               = PARAM_DEVICE_NAME,                  // s, g         ; SirosL
        SOFTWARE_VERSION          = PARAM_SOFT_VERSION,                 // i, g         ; 5200 + Param_getSoftwareVersionRelease()
        LAMP_ADDRESS              = PARAM_WLAN_LAMP_ADDRESS,            // i, g,s       ; lamp address [1..40]
        ALARM                     = PARAM_WLAN_ALARM,                   // i, g         ; alarms
        MAX_ENERGY                = PARAM_WLAN_MAX_ENGERGY,             // i, g         ; 400 or 800
        F_STOP_RANGE              = PARAM_WLAN_F_STOP_RANGE,            // i, g         ; 6 or 8
        STANDBY                   = PARAM_WLAN_SIROS_IN_STANDBYE,       // i, g,s       ; 0->not in standby, 1->in standby, 2->thermoStandby, 3->deeper standby phase
        SPEED                     = PARAM_WLAN_SIROS_IN_SPEED,          // i, g,s       ; 0->disabled, 1->speedMode, 3->HSMode
        AUDIO                     = PARAM_WLAN_SIROS_IN_AUDIO,          // i, g,s       ; 0->disabled, 1->enabled
        SEQUENCE                  = PARAM_WLAN_SEQUENZ,                 // i, g,s       ; actual sequence value [1..50]
        SYNCHRONIZED_SEQUENCE     = PARAM_SYNCRONIZED_SEQUENCE,         // i, g,s       ; 0->disabled, 1->master, 2->slave
        PAUSE_TIME                = PARAM_CHARGE_TIME,                  // i, g,s       ; charge time [1..10000]
        FLASH_DURATION            = PARAM_FLASH_DURATION,               // i, g         ; actual flash time 1/x (fractional part x)
        DELAY_TIME                = PARAM_DELAYTIME,                    // i, g,s       ; delayTime [1..10000]
        COG_LIGHT_ENA             = PARAM_WLAN_COG_LIGHT_ENA,           // i, g,s       ; 0->disabled, 1->enabled
        COG_LIGHT_ACT             = PARAM_WLAN_COG_LIGHT_ACT,           // i, s         ; switches the cognition light
        COG_LIGHT_COL             = PARAM_WLAN_COG_LIGHT_COL,           // i, g         ; cognition color depending on address
        SLOW_CHARGE               = PARAM_MANUAL_SLOW_CHARGE,           // i, g,s       ; slow charge 1->enable, 0->disable
        FLASH                     = PARAM_WLAN_TEST,                    // i, s         ; raises a test flash
        MODEL_LIGHT_ENA           = PARAM_MODEL_LIGHT_ENA,              // i, g,s       ; 0->enabled, 1->disabled
//        MODEL_LIGHT_MODE          = PARAM_MODEL_LIGHT_MODE,
        ENERGY_TOTAL_APPERTURE    = PARAM_APERTURE_ALL_LAMPS,           // i, g,s       ; act aperture of lamp 1
        APPLICATION        = PARAM_FREEMASK_ALTERMATE_ENA,              // i, g,s       ; freemask alternate enable
        MASKGROUP                 = PARAM_MASKGROUP,                    // i, g,s       ; maskgroup

        /* Device network parameters */
        JOIN_NETWORK              = PARAM_JOIN_NETWORK,                 // i, g,s       ; join network
        WLAN_LOGIN1_SSID          = PARAM_WLAN_LOGIN1_SSID,             // s, g,s       ; ssid
        WLAN_LOGIN1_PWD           = PARAM_WLAN_LOGIN1_PWD,              // s, g,s       ; pwd
        WLAN_ENC_MODE             = PARAM_WLAN_LOGIN1_ENC_TYPE,         // i, g,s       ; encoding type

        // new in SW >= xx.92
        RF_SELECT                 = PARAM_WLAN_RF_SELECT,
        TAG                       = PARAM_WLAN_TAG,
        UPDATE_SERVICE            = PARAM_WLAN_UPDATE_SERVICE

    };
    Q_ENUM(Ids)


    explicit DeviceSirosL();
    explicit DeviceSirosL(int addr, bool demo = false);
    explicit DeviceSirosL(std::weak_ptr<CreatorDevice> creator);

    virtual ~DeviceSirosL();

    virtual eDeviceTypes getType() const override;
    virtual const QString IdString(int id) const override;
    virtual int getModLightMode() const override;
    virtual bool setModLightMode(int val) override;
    virtual QString className() const override;


};

inline Device::eDeviceTypes DeviceSirosL::getType() const{
    return Device::eDeviceTypes::eSirosL;
}

inline int DeviceSirosL::getModLightMode() const{
    return -1;
}

inline bool DeviceSirosL::setModLightMode(int val){
    Q_UNUSED(val);
    return false;
}

inline QString DeviceSirosL::className() const{
    return DeviceSirosL::staticMetaObject.className();
}

}

Q_DECLARE_METATYPE(Framework::DeviceSirosL)

#endif // DEVICESIRROSL_H
