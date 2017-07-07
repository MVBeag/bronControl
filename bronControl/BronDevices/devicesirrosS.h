#ifndef DEVICESIRROSS_H
#define DEVICESIRROSS_H

#include <QObject>
#include "devicesirros.h"
#include "creatorDevice.h"

namespace Framework{

class DeviceSirosS : public DeviceSiros{
    Q_OBJECT
public:

    enum Ids{
        DEVICE_DESC               = PARAM_DEVICE_DESC,              // s, g        ; json file
        STUDIO_ADDRESS            = PARAM_STUDIO_ADDRESS,           // i, g,s      ; [1..99]
        NUM_OF_LAMPS              = PARAM_NUM_OF_LAMPS,             // i, g        ; 1
        DEVICE_NAME               = PARAM_DEVICE_NAME,              // s, g        ; Siros
        SOFTWARE_VERSION          = PARAM_SOFT_VERSION,             // i, g        ; 4900 + Param_getSoftwareVersionRelease(); /* 49xx because of Siros */
        LAMP_ADDRESS              = PARAM_WLAN_LAMP_ADDRESS,        // i, g,s      ; lampAddress [1..40]
        ALARM                     = PARAM_WLAN_ALARM,               // i, g        ; alarm
        MAX_ENERGY                = PARAM_WLAN_MAX_ENGERGY,         // i, g        ; 400 or 800
        F_STOP_RANGE              = PARAM_WLAN_F_STOP_RANGE,        // i, g        ; 8(SirosS) or 6(Siros)
        STANDBY                   = PARAM_WLAN_SIROS_IN_STANDBYE,   // i, g,s      ; 0->active, 1->standby, 2->thermo standby
        SPEED                     = PARAM_WLAN_SIROS_IN_SPEED,
        AUDIO                     = PARAM_WLAN_SIROS_IN_AUDIO,      // i, g,s      ; 0->disabled, 1->enabled
        SEQUENCE                  = PARAM_WLAN_SEQUENZ,             // i, s        ; set sequence value [1..50]
        SYNCHRONIZED_SEQUENCE     = PARAM_SYNCRONIZED_SEQUENCE,     // i, g,s      ; synchronizes Sequence Mode 0->off, 1->master, 2->slave
        PAUSE_TIME                = PARAM_CHARGE_TIME,
        FLASH_DURATION            = PARAM_FLASH_DURATION,           // i, g        ; flash duration time 1/x (x is returned)
        DELAY_TIME                = PARAM_DELAYTIME,                // i, g,s      ; delay time [1..10000] ms
        COG_LIGHT_ENA             = PARAM_WLAN_COG_LIGHT_ENA,       // i, g,s      ; 0->disabled, 1->enabled
        COG_LIGHT_ACT             = PARAM_WLAN_COG_LIGHT_ACT,       // i, s        ; activates cognition light
        COG_LIGHT_COL             = PARAM_WLAN_COG_LIGHT_COL,
//        SLOW_CHARGE               = PARAM_MANUAL_SLOW_CHARGE,
        FLASH                     = PARAM_WLAN_TEST,                // i, s        ; raises an flash
        MODEL_LIGHT_ENA           = PARAM_MODEL_LIGHT_ENA,          // i, g        ; 0->disabled, 1->enabled
        MODEL_LIGHT_MODE          = PARAM_MODEL_LIGHT_MODE,         // i, g        ; 0->full, 1->prop
        ENERGY_TOTAL_APPERTURE    = PARAM_APERTURE_ALL_LAMPS,       // i, g        ; aperture Lamp 1
        APPLICATION        = PARAM_FREEMASK_ALTERMATE_ENA,          // i, g,s      ; if value != 0, activates also SYNC_MODE_RF
        MASKGROUP                 = PARAM_MASKGROUP,                // i, g,s      ; MaskGroup
//                                  PARAM_WLAN_F_CONTROL_MODE       // i, g,s      ; 0->no speed, 1->Speed, 3->HS(only on 800J devices)
//                                  PARAM_RF_MODE                   // i, g        ; rfMode
//                                  PARAM_SS_CHARGE_TIME            // i, g
        /* Device network parameters */
        JOIN_NETWORK              = PARAM_JOIN_NETWORK,             // i, g        ; has only effect if ssid and pwd not empty,
        WLAN_LOGIN1_SSID          = PARAM_WLAN_LOGIN1_SSID,         // s, g,s      ; ssid
        WLAN_LOGIN1_PWD           = PARAM_WLAN_LOGIN1_PWD,          // s, g,s      ; pwd
        WLAN_ENC_MODE             = PARAM_WLAN_LOGIN1_ENC_TYPE,     // i, g,s      ; wifi enc type

        // new in SW >= xx.92
        RF_SELECT                 = PARAM_WLAN_RF_SELECT,
        TAG                       = PARAM_WLAN_TAG,
        UPDATE_SERVICE            = PARAM_WLAN_UPDATE_SERVICE

    };
    Q_ENUM(Ids)


    explicit DeviceSirosS();
    explicit DeviceSirosS(int addr, bool demo = false);
    explicit DeviceSirosS(std::weak_ptr<CreatorDevice> creator);

    virtual ~DeviceSirosS();
    virtual const QString IdString(int id) const override;
    virtual QString className() const override;
    virtual eDeviceTypes getType() const override;

};

inline QString DeviceSirosS::className() const{
    return DeviceSirosS::staticMetaObject.className();
}

inline Device::eDeviceTypes DeviceSirosS::getType() const{
    return Device::eDeviceTypes::eSirosS;
}



}

Q_DECLARE_METATYPE(Framework::DeviceSirosS)

#endif // DEVICESIRROSS_H
