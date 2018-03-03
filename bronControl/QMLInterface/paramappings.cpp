#include "paramappings.h"
#include "deviceproxy.h"
#include "devicesirros.h"
#include "devicesirrosL.h"
#include "devicesirrosS.h"
#include "devicescoro.h"
#include "genLamp.h"


using namespace QMLInterface;
using namespace Framework;

static const QHash<int,int> defaultHash;
//                 key, value
static const QHash<int, int> sirosMapping(
        std::initializer_list<std::pair<int, int> >({
                                                        //            {DeviceSiros::Ids::DEVICE_DESC,    DeviceProxy::eRoles::DEVICE_DESC},
                                                        {DeviceSiros::Ids::STUDIO_ADDRESS,   DeviceProxy::eRoles::STUDIO_ADDRESS},
                                                        //            {DeviceSiros::Ids::NUM_OF_LAMPS,   DeviceProxy::eRoles::NUM_OF_LAMPS},
                                                        {DeviceSiros::Ids::DEVICE_NAME,      DeviceProxy::eRoles::NAME},
                                                        {DeviceSiros::Ids::SOFTWARE_VERSION, DeviceProxy::eRoles::SW_VERSION},
                                                        {DeviceSiros::Ids::LAMP_ADDRESS,     DeviceProxy::eRoles::LAMP_ADDRESS},
                                                        {DeviceSiros::Ids::ALARM,            DeviceProxy::eRoles::ALARM},
                                                        {DeviceSiros::Ids::MAX_ENERGY,       DeviceProxy::eRoles::MAX_APERTURE},
                                                        {DeviceSiros::Ids::F_STOP_RANGE,     DeviceProxy::eRoles::F_STOP_RANGE},
                                                        {DeviceSiros::Ids::STANDBY,          DeviceProxy::eRoles::STANDBY},
                                                        {DeviceSiros::Ids::SPEED,            DeviceProxy::eRoles::SPEED},
                                                        {DeviceSiros::Ids::AUDIO,            DeviceProxy::eRoles::AUDIO},
                                                        {DeviceSiros::Ids::SEQUENCE,         DeviceProxy::eRoles::SEQUENCE},
                                                        {DeviceSiros::Ids::SYNCHRONIZED_SEQUENCE, DeviceProxy::eRoles::SYN_SEQUENCE},
                                                        {DeviceSiros::Ids::PAUSE_TIME,       DeviceProxy::eRoles::PAUSE_TIME},
                                                        {DeviceSiros::Ids::FLASH_DURATION,   DeviceProxy::eRoles::FLASH_DURATION},
                                                        {DeviceSiros::Ids::DELAY_TIME,       DeviceProxy::eRoles::DELAY},
                                                        {DeviceSiros::Ids::COG_LIGHT_ENA,    DeviceProxy::eRoles::COGNI_COL_ENA},
                                                        {DeviceSiros::Ids::COG_LIGHT_ACT,    DeviceProxy::eRoles::COGNI_COL_ACT},
                                                        {DeviceSiros::Ids::COG_LIGHT_COL,    DeviceProxy::eRoles::COGNI_COL},
//                                                        {DeviceSiros::Ids::SLOW_CHARGE,      DeviceProxy::eRoles::SLOW_CHARGE},
                                                        {DeviceSiros::Ids::FLASH,            DeviceProxy::eRoles::TEST},
                                                        {DeviceSiros::Ids::MODEL_LIGHT_ENA,  DeviceProxy::eRoles::MODLIGHT},
                                                        {DeviceSiros::Ids::MODEL_LIGHT_MODE, DeviceProxy::eRoles::MODLIGHTMODE},
                                                        {DeviceSiros::Ids::ENERGY_TOTAL_APPERTURE, DeviceProxy::eRoles::ACT_APERTURE},
                                                        {DeviceSiros::Ids::APPLICATION, DeviceProxy::eRoles::APPLICATION},
                                                        {DeviceSiros::Ids::MASKGROUP,        DeviceProxy::eRoles::MASKGROUP},
                                                        {DeviceSiros::Ids::RF_SELECT,        DeviceProxy::eRoles::REMOTE_SWITCH},
                                                        {DeviceSiros::Ids::JOIN_NETWORK,     DeviceProxy::eRoles::JOIN_NETWORK},
                                                        {DeviceSiros::Ids::WLAN_LOGIN1_SSID, DeviceProxy::eRoles::WLAN_SSID},
                                                        {DeviceSiros::Ids::FLASH_COUNT,  DeviceProxy::eRoles::FLASH_COUNT},
                                                        {DeviceSiros::Ids::WLAN_LOGIN1_PWD,  DeviceProxy::eRoles::WLAN_PWD},
                                                        {DeviceSiros::Ids::WLAN_ENC_MODE,    DeviceProxy::eRoles::WLAN_ENC}}));

static const QHash<int, int> sirosLMapping(
        std::initializer_list<std::pair<int, int> >({
                                                        //            {DeviceSiros::Ids::DEVICE_DESC,    DeviceProxy::eRoles::DEVICE_DESC},
                                                        {DeviceSiros::Ids::STUDIO_ADDRESS,   DeviceProxy::eRoles::STUDIO_ADDRESS},
                                                        //            {DeviceSiros::Ids::NUM_OF_LAMPS,   DeviceProxy::eRoles::NUM_OF_LAMPS},
                                                        {DeviceSiros::Ids::DEVICE_NAME,      DeviceProxy::eRoles::NAME},
                                                        {DeviceSiros::Ids::SOFTWARE_VERSION, DeviceProxy::eRoles::SW_VERSION},
                                                        {DeviceSiros::Ids::LAMP_ADDRESS,     DeviceProxy::eRoles::LAMP_ADDRESS},
                                                        {DeviceSiros::Ids::ALARM,            DeviceProxy::eRoles::ALARM},
                                                        {DeviceSiros::Ids::MAX_ENERGY,       DeviceProxy::eRoles::MAX_APERTURE},
                                                        {DeviceSiros::Ids::F_STOP_RANGE,     DeviceProxy::eRoles::F_STOP_RANGE},
                                                        {DeviceSiros::Ids::STANDBY,          DeviceProxy::eRoles::STANDBY},
                                                        {DeviceSiros::Ids::SPEED,            DeviceProxy::eRoles::SPEED},
                                                        {DeviceSiros::Ids::AUDIO,            DeviceProxy::eRoles::AUDIO},
                                                        {DeviceSiros::Ids::SEQUENCE,         DeviceProxy::eRoles::SEQUENCE},
                                                        {DeviceSiros::Ids::SYNCHRONIZED_SEQUENCE, DeviceProxy::eRoles::SYN_SEQUENCE},
                                                        {DeviceSiros::Ids::PAUSE_TIME,       DeviceProxy::eRoles::PAUSE_TIME},
                                                        {DeviceSiros::Ids::FLASH_DURATION,   DeviceProxy::eRoles::FLASH_DURATION},
                                                        {DeviceSiros::Ids::DELAY_TIME,       DeviceProxy::eRoles::DELAY},
                                                        {DeviceSiros::Ids::COG_LIGHT_ENA,    DeviceProxy::eRoles::COGNI_COL_ENA},
                                                        {DeviceSiros::Ids::COG_LIGHT_ACT,    DeviceProxy::eRoles::COGNI_COL_ACT},
                                                        {DeviceSiros::Ids::COG_LIGHT_COL,    DeviceProxy::eRoles::COGNI_COL},
                                                        {DeviceSiros::Ids::SLOW_CHARGE,      DeviceProxy::eRoles::SLOW_CHARGE},
                                                        {DeviceSiros::Ids::FLASH,            DeviceProxy::eRoles::TEST},
                                                        {DeviceSiros::Ids::MODEL_LIGHT_ENA,  DeviceProxy::eRoles::MODLIGHT},
                                                        //                    {DeviceSiros::Ids::MODEL_LIGHT_MODE, DeviceProxy::eRoles::MODLIGHTMODE},
                                                        {DeviceSiros::Ids::ENERGY_TOTAL_APPERTURE, DeviceProxy::eRoles::ACT_APERTURE},
                                                        {DeviceSiros::Ids::APPLICATION, DeviceProxy::eRoles::APPLICATION},
                                                        {DeviceSiros::Ids::MASKGROUP,        DeviceProxy::eRoles::MASKGROUP},
                                                        {DeviceSiros::Ids::RF_SELECT,        DeviceProxy::eRoles::REMOTE_SWITCH},
                                                        {DeviceSiros::Ids::JOIN_NETWORK,     DeviceProxy::eRoles::JOIN_NETWORK},
                                                        {DeviceSiros::Ids::WLAN_LOGIN1_SSID, DeviceProxy::eRoles::WLAN_SSID},
                                                        {DeviceSiros::Ids::FLASH_COUNT,  DeviceProxy::eRoles::FLASH_COUNT},
                                                        {DeviceSiros::Ids::WLAN_LOGIN1_PWD,  DeviceProxy::eRoles::WLAN_PWD},
                                                        {DeviceSiros::Ids::WLAN_ENC_MODE,    DeviceProxy::eRoles::WLAN_ENC}}));

static const QHash<int, int> sirosSMapping(
        std::initializer_list<std::pair<int, int> >({
                                                        //            {DeviceSiros::Ids::DEVICE_DESC,    DeviceProxy::eRoles::DEVICE_DESC},
                                                        {DeviceSiros::Ids::STUDIO_ADDRESS,   DeviceProxy::eRoles::STUDIO_ADDRESS},
                                                        //            {DeviceSiros::Ids::NUM_OF_LAMPS,   DeviceProxy::eRoles::NUM_OF_LAMPS},
                                                        {DeviceSiros::Ids::DEVICE_NAME,      DeviceProxy::eRoles::NAME},
                                                        {DeviceSiros::Ids::SOFTWARE_VERSION, DeviceProxy::eRoles::SW_VERSION},
                                                        {DeviceSiros::Ids::LAMP_ADDRESS,     DeviceProxy::eRoles::LAMP_ADDRESS},
                                                        {DeviceSiros::Ids::ALARM,            DeviceProxy::eRoles::ALARM},
                                                        {DeviceSiros::Ids::MAX_ENERGY,       DeviceProxy::eRoles::MAX_APERTURE},
                                                        {DeviceSiros::Ids::F_STOP_RANGE,     DeviceProxy::eRoles::F_STOP_RANGE},
                                                        {DeviceSiros::Ids::STANDBY,          DeviceProxy::eRoles::STANDBY},
                                                        {DeviceSiros::Ids::SPEED,            DeviceProxy::eRoles::SPEED},
                                                        {DeviceSiros::Ids::AUDIO,            DeviceProxy::eRoles::AUDIO},
                                                        {DeviceSiros::Ids::SEQUENCE,         DeviceProxy::eRoles::SEQUENCE},
                                                        {DeviceSiros::Ids::SYNCHRONIZED_SEQUENCE, DeviceProxy::eRoles::SYN_SEQUENCE},
                                                        {DeviceSiros::Ids::PAUSE_TIME,       DeviceProxy::eRoles::PAUSE_TIME},
                                                        {DeviceSiros::Ids::FLASH_DURATION,   DeviceProxy::eRoles::FLASH_DURATION},
                                                        {DeviceSiros::Ids::DELAY_TIME,       DeviceProxy::eRoles::DELAY},
                                                        {DeviceSiros::Ids::COG_LIGHT_ENA,    DeviceProxy::eRoles::COGNI_COL_ENA},
                                                        {DeviceSiros::Ids::COG_LIGHT_ACT,    DeviceProxy::eRoles::COGNI_COL_ACT},
                                                        {DeviceSiros::Ids::COG_LIGHT_COL,    DeviceProxy::eRoles::COGNI_COL},
//                                                        {DeviceSiros::Ids::SLOW_CHARGE,      DeviceProxy::eRoles::SLOW_CHARGE},
                                                        {DeviceSiros::Ids::FLASH,            DeviceProxy::eRoles::TEST},
                                                        {DeviceSiros::Ids::MODEL_LIGHT_ENA,  DeviceProxy::eRoles::MODLIGHT},
                                                        {DeviceSiros::Ids::MODEL_LIGHT_MODE, DeviceProxy::eRoles::MODLIGHTMODE},
                                                        {DeviceSiros::Ids::ENERGY_TOTAL_APPERTURE, DeviceProxy::eRoles::ACT_APERTURE},
                                                        {DeviceSiros::Ids::APPLICATION, DeviceProxy::eRoles::APPLICATION},
                                                        {DeviceSiros::Ids::MASKGROUP,        DeviceProxy::eRoles::MASKGROUP},
                                                        {DeviceSiros::Ids::RF_SELECT,        DeviceProxy::eRoles::REMOTE_SWITCH},
                                                        {DeviceSiros::Ids::JOIN_NETWORK,     DeviceProxy::eRoles::JOIN_NETWORK},
                                                        {DeviceSiros::Ids::WLAN_LOGIN1_SSID, DeviceProxy::eRoles::WLAN_SSID},
                                                        {DeviceSiros::Ids::WLAN_LOGIN1_PWD,  DeviceProxy::eRoles::WLAN_PWD},
                                                        {DeviceSiros::Ids::FLASH_COUNT,  DeviceProxy::eRoles::FLASH_COUNT},
                                                        {DeviceSiros::Ids::WLAN_ENC_MODE,    DeviceProxy::eRoles::WLAN_ENC}}));
static const QHash<int, int> scoroMapping(
        std::initializer_list<std::pair<int, int> >({
                                                        //            {DeviceScoro::Ids::DEVICE_DESC,   DeviceProxy::eRoles::DEVICE_DESC},
                                                        {DeviceScoro::Ids::STUDIO_ADDR, DeviceProxy::eRoles::STUDIO_ADDRESS},
                                                        //            {DeviceScoro::Ids::NUM_LAMP_SLOTS, DeviceProxy::eRoles::NUM_OF_LAMPS},
                                                        {DeviceScoro::Ids::DEVICE_NAME, DeviceProxy::eRoles::NAME},
                                                        {DeviceScoro::Ids::AB_SW_VER, DeviceProxy::eRoles::SW_VERSION},  // use adapterboard software
                                                        {DeviceScoro::Ids::ALARM_REASON, DeviceProxy::eRoles::ALARM},
                                                        {DeviceScoro::Ids::DISPLAY_SPEED_APERTURE, DeviceProxy::eRoles::MAX_APERTURE}, // has speed automatic in background
                                                        {DeviceScoro::Ids::F_STOP_RANGE, DeviceProxy::eRoles::F_STOP_RANGE},
                                                        {DeviceScoro::Ids::F_STOP_MAX, DeviceProxy::eRoles::F_STOP_MAX},
                                                        {DeviceScoro::Ids::STANDBY_SW, DeviceProxy::eRoles::STANDBY},
                                                        {DeviceScoro::Ids::SPEED_SW, DeviceProxy::eRoles::SPEED},
                                                        {DeviceScoro::Ids::RDY_SOUND_VOLUME, DeviceProxy::eRoles::AUDIO},
                                                        {DeviceScoro::Ids::SEQUENCE, DeviceProxy::eRoles::SEQUENCE},
//                                                        {DeviceScoro::Ids::INTERVAL, DeviceProxy::eRoles::SYN_SEQUENCE},        // take care about this
                                                        {DeviceScoro::Ids::INTERVAL, DeviceProxy::eRoles::PAUSE_TIME},
                                                        {DeviceScoro::Ids::T01, DeviceProxy::eRoles::FLASH_TIME_SETTING},
                                                        {DeviceScoro::Ids::T01_VALUE, DeviceProxy::eRoles::FLASH_DURATION},
                                                        {DeviceScoro::Ids::DELAY, DeviceProxy::eRoles::DELAY},
                                                        {DeviceScoro::Ids::SLOW_CHARGING, DeviceProxy::eRoles::SLOW_CHARGE},
                                                        {DeviceScoro::Ids::FLASH_TEST, DeviceProxy::eRoles::TEST},
                                                        {DeviceScoro::Ids::PILOTLIGHT_SW, DeviceProxy::eRoles::MODLIGHT},
                                                        {DeviceScoro::Ids::PILOTLIGHT_MODE, DeviceProxy::eRoles::MODLIGHTMODE},
                                                        {DeviceScoro::Ids::GEN_APERTURE, DeviceProxy::eRoles::ACT_APERTURE},
                                                        {DeviceScoro::Ids::ALTERNATE_NUMBER_DEVICES, DeviceProxy::eRoles::APPLICATION},
                                                        {DeviceScoro::Ids::ALTERNATE_DEVICE_DELAY, DeviceProxy::eRoles::MASKGROUP},
                                                        {DeviceScoro::Ids::NW_JOIN, DeviceProxy::eRoles::JOIN_NETWORK},
                                                        {DeviceScoro::Ids::NW_SSID, DeviceProxy::eRoles::WLAN_SSID},
                                                        {DeviceScoro::Ids::NW_PWD, DeviceProxy::eRoles::WLAN_PWD},
                                                        {DeviceScoro::Ids::COLOR_TEMP, DeviceProxy::eRoles::COLOR_TEMP},
                                                        {DeviceScoro::Ids::REDUCED_PILOTLIGHT, DeviceProxy::eRoles::DIMMER},
                                                        {DeviceScoro::Ids::SENSITIVITY_PHOTO_CELL, DeviceProxy::eRoles::CELL_SENSITIVITY},
                                                        {DeviceScoro::Ids::CELL_SW, DeviceProxy::eRoles::CELL},
                                                        {DeviceScoro::Ids::IR_RFS_SW, DeviceProxy::eRoles::IR_RFS},
//                                                        {DeviceScoro::Ids::IR_SW, DeviceProxy::eRoles::IR},
                                                        {DeviceScoro::Ids::TEST_KEY_INTENSITY, DeviceProxy::eRoles::BRIGHT_TEST},
                                                        {DeviceScoro::Ids::RDY_SOUND_MODE, DeviceProxy::eRoles::AUDIO_MODE},
                                                        {DeviceScoro::Ids::DISPLAY_INTENSITY, DeviceProxy::eRoles::DISP_INTENSITY},
                                                        {DeviceScoro::Ids::RESET_FLASH_COUNTER, DeviceProxy::eRoles::RESET_FLASH_COUNT},
                                                        {DeviceScoro::Ids::DAILY_FLASH_COUNT, DeviceProxy::eRoles::DAILY_COUNTER},
                                                        {DeviceScoro::Ids::TOTAL_FLASH_COUNT, DeviceProxy::eRoles::OVERALL_COUNTER}}));


static const QHash<int, int> genLampMapping(
        std::initializer_list<std::pair<int, int> >({
                                                        {GenLamp::Ids::L1_ADDR,         DeviceProxy::eRoles::LAMP_ADDRESS},
                                                        {GenLamp::Ids::L2_ADDR,         DeviceProxy::eRoles::LAMP_ADDRESS},
                                                        {GenLamp::Ids::L3_ADDR,         DeviceProxy::eRoles::LAMP_ADDRESS},
                                                        {GenLamp::Ids::L1_SW,           DeviceProxy::eRoles::STANDBY},
                                                        {GenLamp::Ids::L2_SW,           DeviceProxy::eRoles::STANDBY},
                                                        {GenLamp::Ids::L3_SW,           DeviceProxy::eRoles::STANDBY},
                                                        {GenLamp::Ids::L1_APERTURE,     DeviceProxy::eRoles::ACT_APERTURE},
                                                        {GenLamp::Ids::L2_APERTURE,     DeviceProxy::eRoles::ACT_APERTURE},
                                                        {GenLamp::Ids::L3_APERTURE,     DeviceProxy::eRoles::ACT_APERTURE}
                                                    }));

ParaMappings::ParaMappings(QObject *parent) : QObject(parent){}

const QHash<int, int> &ParaMappings::getDeviceMappings(const std::weak_ptr<Device> d){
    if(auto dev = d.lock()){
        QString name = dev->className();
        if(name == "Framework::DeviceSiros"){
            return sirosMapping;
        }
        else if(name == "Framework::DeviceSirosL"){
            return sirosLMapping;
        }
        else if(name == "Framework::DeviceSirosS"){
            return sirosSMapping;
        }
        else if(name == "Framework::DeviceScoro"){
            return scoroMapping;
        }
        else if(name == "Framework::GenLamp"){
            return genLampMapping;
        }
        else{
            return defaultHash;
        }
    }
    else{
        return defaultHash;
    }
}
