#include "devicescoro.h"
#include "paraSelectables.h"
#include "genLamp.h"
#include "demodevicepara.h"
#include "studiomgr.h"
#include <math.h>


using namespace Framework;

/**
  * validity functions
  */

static void handleLamp1(Device &dev){
    DeviceScoro *scoro = dynamic_cast<DeviceScoro *>(&dev);
    if(dev.getPara(DeviceScoro::L1_INSERTED)->data().toBool() == true){
        if(scoro){
            if(!scoro->hasLamp(0)){
                if(scoro->getDemo()){
                    auto l = std::make_shared<GenLamp>(0, scoro, scoro->getDemo());
                    l->setParameter(GenLamp::L1_ADDR, 1);
                    l->setParameter(GenLamp::L1_APERTURE, 4.6);
                    l->setParameter(GenLamp::L1_SW, true);
                    l->init();
                    scoro->addLamp(l);
                    // add this device also to ths studio, the visualisation is done via filterss
                    StudioMgr::getInstance()->newIdentifiedDevice(l);
                    emit l->initParasRead(l);
                }
                else{
                    emit scoro->createLamp(scoro, 0);
                }
            }
        }
    }
    else{
        if(scoro->hasLamp(0)){
            if(scoro->getDemo()){
                auto l = scoro->removeLamp(0).lock();
                emit l->deviceLost(l);

            }
            else{
                emit scoro->destroyLamp(scoro, 0);
            }
        }
    }
}

static void handleLamp2(Device &dev){
    DeviceScoro *scoro = dynamic_cast<DeviceScoro *>(&dev);
    if(dev.getPara(DeviceScoro::L2_INSERTED)->data().toBool() == true){
        if(scoro){
            if(!scoro->hasLamp(1)){
                if(scoro->getDemo()){
                    auto l = std::make_shared<GenLamp>(1, scoro, scoro->getDemo());
                    l->setParameter(GenLamp::L2_ADDR, 1);
                    l->setParameter(GenLamp::L2_APERTURE, 4.6);
                    l->setParameter(GenLamp::L2_SW, true);
                    l->init();
                    scoro->addLamp(l);
                    // add this device also to ths studio, the visualisation is done via filterss
                    StudioMgr::getInstance()->newIdentifiedDevice(l);
                    emit l->initParasRead(l);
                }
                else{
                    emit scoro->createLamp(scoro, 1);
                }
            }
        }
    }
    else{
        if(scoro->hasLamp(1)){
            if(scoro->getDemo()){
                auto l = scoro->removeLamp(1).lock();
                emit l->deviceLost(l);
            }
            else{
                emit scoro->destroyLamp(scoro, 1);
            }
        }
    }
}

static void handleLamp3(Device &dev){
    DeviceScoro *scoro = dynamic_cast<DeviceScoro *>(&dev);
    if(dev.getPara(DeviceScoro::L3_INSERTED)->data().toBool() == true){
        if(scoro){
            if(!scoro->hasLamp(2)){
                if(scoro->getDemo()){
                    auto l = std::make_shared<GenLamp>(2, scoro, scoro->getDemo());
                    l->setParameter(GenLamp::L3_ADDR, 1);
                    l->setParameter(GenLamp::L3_APERTURE, 4.6);
                    l->setParameter(GenLamp::L3_SW, true);
                    l->init();
                    // add this device also to ths studio, the visualisation is done via filterss
                    scoro->addLamp(l);
                    StudioMgr::getInstance()->newIdentifiedDevice(l);
                    emit l->initParasRead(l);
                }
                else{
                    emit scoro->createLamp(scoro, 2);
                }
            }
        }
    }
    else{
        if(scoro->hasLamp(2)){
            if(scoro->getDemo()){
                auto l =scoro->removeLamp(2).lock();
                emit l->deviceLost(l);
            }
            else{
                emit scoro->destroyLamp(scoro, 2);
            }
        }
    }
}


static void checkReadoutParas(Device &d){
    QString s = d.getPara(DeviceScoro::UPDATE_SERVICE)->data().toString();
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

static void l1SwChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l1SwChanged(dev.getPara(DeviceScoro::L1_SW)->scaledData());
}

static void l1ApertureChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l1ApertureChanged(dev.getPara(DeviceScoro::L1_APERTURE)->scaledData());
}

static void l1AddressChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l1AddrChanged(dev.getPara(DeviceScoro::L1_ADDR)->scaledData());
}
static void l2SwChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l2SwChanged(dev.getPara(DeviceScoro::L2_SW)->scaledData());
}

static void l2ApertureChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l2ApertureChanged(dev.getPara(DeviceScoro::L2_APERTURE)->scaledData());
}

static void l2AddressChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l2AddrChanged(dev.getPara(DeviceScoro::L2_ADDR)->scaledData());
}
static void l3SwChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l3SwChanged(dev.getPara(DeviceScoro::L3_SW)->scaledData());
}

static void l3ApertureChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l3ApertureChanged(dev.getPara(DeviceScoro::L3_APERTURE)->scaledData());
}

static void l3AddressChanged(Device &dev){
    DeviceScoro *s = dynamic_cast<DeviceScoro *>(&dev);
    if(s) emit s->l3AddrChanged(dev.getPara(DeviceScoro::L3_ADDR)->scaledData());
}


#define _I   DevicePara::eInit

void DeviceScoro::initParas(int, bool demo){
    m_hasLamp[0] =
    m_hasLamp[1] =
    m_hasLamp[2] = false;

    if(demo){
        m_remoteParas.insert(DEVICE_DESC,          std::make_shared<DemoDevicePara>(this, (DEVICE_DESC),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(STANDBY_SW,           std::make_shared<DemoDevicePara>(this, (STANDBY_SW), false, DevicePara::Types::eInt, 1.0f, DevicePara::eInit ));
        m_remoteParas.insert(DEVICE_NAME,          std::make_shared<DemoDevicePara>(this, (DEVICE_NAME), "",DevicePara::Types::eString, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(AB_SW_VER,            std::make_shared<DemoDevicePara>(this, (AB_SW_VER), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(NUM_LAMP_SLOTS,       std::make_shared<DemoDevicePara>(this, (NUM_LAMP_SLOTS), 3, DevicePara::Types::eInt, 1.0f, DevicePara::eInit ));
        m_remoteParas.insert(STUDIO_ADDR,          std::make_shared<DemoDevicePara>(this, (STUDIO_ADDR), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit ));
        m_remoteParas.insert(L1_INSERTED,          std::make_shared<DemoDevicePara>(this, (L1_INSERTED), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit , &everValid, &handleLamp1));
        m_remoteParas.insert(L2_INSERTED,          std::make_shared<DemoDevicePara>(this, (L2_INSERTED), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit , &everValid, &handleLamp2));
        m_remoteParas.insert(L3_INSERTED,          std::make_shared<DemoDevicePara>(this, (L3_INSERTED), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit , &everValid, &handleLamp3));
        //    m_remoteParas.insert(Paras::Id::MAX_ENERGY,              std::make_shared<DemoDevicePara>(this, (Paras::Id::MAX_ENERGY), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(F_STOP_RANGE,         std::make_shared<DemoDevicePara>(this, (F_STOP_RANGE), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit ));
        m_remoteParas.insert(F_STOP_MAX,           std::make_shared<DemoDevicePara>(this, (F_STOP_MAX), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit ));
        m_remoteParas.insert(UPDATE_SERVICE,       std::make_shared<DemoDevicePara>(this, (UPDATE_SERVICE), "",DevicePara::Types::eString, 1.0f, DevicePara::noOptions, everValid, checkReadoutParas));
        m_remoteParas.insert(DISPLAY_APERTURE_9,   std::make_shared<DemoDevicePara>(this, (DISPLAY_APERTURE_9), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit));
        m_remoteParas.insert(PILOTLIGHT_MODE,      std::make_shared<DemoDevicePara>(this, (PILOTLIGHT_MODE), (int)ParaSelects::CognitionLightColor::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(PILOTLIGHT_SW,        std::make_shared<DemoDevicePara>(this, (PILOTLIGHT_SW), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(REDUCED_PILOTLIGHT,   std::make_shared<DemoDevicePara>(this, (REDUCED_PILOTLIGHT), false, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        //    m_remoteParas.insert(Paras::Id::ENERGY_TOTAL_APPERTURE,  std::make_shared<DemoDevicePara>(this, (Paras::Id::ENERGY_TOTAL_APPERTURE), QVariant(0),DevicePara::Types::eInt, 10.0f, _IP, &eneryValidity));
        m_remoteParas.insert(ALARM_REASON,         std::make_shared<DemoDevicePara>(this, (ALARM_REASON), (int)ParaSelects::AlarmType::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(RDY_SOUND_MODE,       std::make_shared<DemoDevicePara>(this, (RDY_SOUND_MODE), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(RDY_SOUND_VOLUME,     std::make_shared<DemoDevicePara>(this, (RDY_SOUND_VOLUME), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(COLOR_TEMP,           std::make_shared<DemoDevicePara>(this, (COLOR_TEMP), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SPEED_SW,             std::make_shared<DemoDevicePara>(this, (SPEED_SW), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(T01_VALUE,            std::make_shared<DemoDevicePara>(this, (T01_VALUE), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(T01,                  std::make_shared<DemoDevicePara>(this, (T01), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DAILY_FLASH_COUNT,    std::make_shared<DemoDevicePara>(this, (DAILY_FLASH_COUNT), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(TOTAL_FLASH_COUNT,    std::make_shared<DemoDevicePara>(this, (TOTAL_FLASH_COUNT), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(STROBO_SW,            std::make_shared<DemoDevicePara>(this, (STROBO_SW), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SEQUENCE,             std::make_shared<DemoDevicePara>(this, (SEQUENCE), 1,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(INTERVAL,             std::make_shared<DemoDevicePara>(this, (INTERVAL), 0, DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(ALTERNATE_NUMBER_DEVICES,std::make_shared<DemoDevicePara>(this, (ALTERNATE_NUMBER_DEVICES), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(ALTERNATE_DEVICE_DELAY,std::make_shared<DemoDevicePara>(this, (ALTERNATE_DEVICE_DELAY), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DELAY,                std::make_shared<DemoDevicePara>(this, (DELAY), 0,DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(FLASH_TEST,           std::make_shared<DemoDevicePara>(this, (FLASH_TEST),0,DevicePara::Types::eInt, DevicePara::eInit));
        m_remoteParas.insert(SLOW_CHARGING,        std::make_shared<DemoDevicePara>(this, (SLOW_CHARGING),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SCORO_ENERGY_TYPE,    std::make_shared<DemoDevicePara>(this, (SCORO_ENERGY_TYPE),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(GEN_APERTURE,         std::make_shared<DemoDevicePara>(this, (GEN_APERTURE),0,DevicePara::Types::eInt, 10.0f, DevicePara::eInit));

        m_remoteParas.insert(IR_RFS_SW,             std::make_shared<DemoDevicePara>(this, (IR_RFS_SW),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(CELL_SW,               std::make_shared<DemoDevicePara>(this, (CELL_SW),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
//        m_remoteParas.insert(RFS_SW,                std::make_shared<DemoDevicePara>(this, (RFS_SW),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SENSITIVITY_PHOTO_CELL,std::make_shared<DemoDevicePara>(this, (SENSITIVITY_PHOTO_CELL),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(RESET_FLASH_COUNTER,   std::make_shared<DemoDevicePara>(this, (RESET_FLASH_COUNTER),0,DevicePara::Types::eInt, 1.0f));
        m_remoteParas.insert(TEST_KEY_INTENSITY,    std::make_shared<DemoDevicePara>(this, (TEST_KEY_INTENSITY),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DISPLAY_INTENSITY,    std::make_shared<DemoDevicePara>(this, (DISPLAY_INTENSITY),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(NW_JOIN,              std::make_shared<DemoDevicePara>(this, (NW_JOIN),0,DevicePara::Types::eInt,1.0f, DevicePara::eInit));
        m_remoteParas.insert(NW_SSID,              std::make_shared<DemoDevicePara>(this, (NW_SSID),"Bron-Studio",DevicePara::Types::eString, DevicePara::eInit));
        m_remoteParas.insert(NW_PWD,               std::make_shared<DemoDevicePara>(this, (NW_PWD),"bronControl",DevicePara::Types::eString, DevicePara::eInit));
    }
    else{
        m_remoteParas.insert(DEVICE_DESC,          std::make_shared<DevicePara>(this, (DEVICE_DESC),0,DevicePara::Types::eInt,1.0f,DevicePara::eInit));
        m_remoteParas.insert(STANDBY_SW,           std::make_shared<DevicePara>(this, (STANDBY_SW), false, DevicePara::Types::eInt,1.0f, DevicePara::eInit ));
        m_remoteParas.insert(DEVICE_NAME,          std::make_shared<DevicePara>(this, (DEVICE_NAME), "",DevicePara::Types::eString,1.0f, DevicePara::eInit));
        m_remoteParas.insert(AB_SW_VER,            std::make_shared<DevicePara>(this, (AB_SW_VER), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(NUM_LAMP_SLOTS,       std::make_shared<DevicePara>(this, (NUM_LAMP_SLOTS), 3, DevicePara::Types::eInt, 1.0f, DevicePara::eInit ));
        m_remoteParas.insert(STUDIO_ADDR,          std::make_shared<DevicePara>(this, (STUDIO_ADDR), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit ));
        m_remoteParas.insert(L1_INSERTED,          std::make_shared<DevicePara>(this, (L1_INSERTED), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit , &everValid, &handleLamp1));
        m_remoteParas.insert(L1_ADDR,              std::make_shared<DevicePara>(this, (L1_ADDR),1,DevicePara::Types::eInt,1.0f,DevicePara::eInit, everValid, &l1AddressChanged));
        m_remoteParas.insert(L1_APERTURE,          std::make_shared<DevicePara>(this, (L1_APERTURE),0.0,DevicePara::Types::eInt,10.0f,DevicePara::eInit, everValid, &::l1ApertureChanged));
        m_remoteParas.insert(L1_SW,                std::make_shared<DevicePara>(this, (L1_SW),false,DevicePara::Types::eInt,1.0f,DevicePara::eInit, everValid, &::l1SwChanged));
        m_remoteParas.insert(L2_INSERTED,          std::make_shared<DevicePara>(this, (L2_INSERTED), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit , &everValid, &handleLamp2));
        m_remoteParas.insert(L2_ADDR,              std::make_shared<DevicePara>(this, (L2_ADDR),1,DevicePara::Types::eInt,1.0f,DevicePara::eInit, everValid, &l2AddressChanged));
        m_remoteParas.insert(L2_APERTURE,          std::make_shared<DevicePara>(this, (L2_APERTURE),0.0,DevicePara::Types::eInt,10.0f,DevicePara::eInit, everValid, &::l2ApertureChanged));
        m_remoteParas.insert(L2_SW,                std::make_shared<DevicePara>(this, (L2_SW),false,DevicePara::Types::eInt,1.0f,DevicePara::eInit, everValid, &::l2SwChanged));
        m_remoteParas.insert(L3_INSERTED,          std::make_shared<DevicePara>(this, (L3_INSERTED), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit , &everValid, &handleLamp3));
        m_remoteParas.insert(L3_ADDR,              std::make_shared<DevicePara>(this, (L3_ADDR),1,DevicePara::Types::eInt,1.0f,DevicePara::eInit, everValid, &l3AddressChanged));
        m_remoteParas.insert(L3_APERTURE,          std::make_shared<DevicePara>(this, (L3_APERTURE),0.0,DevicePara::Types::eInt,10.0f,DevicePara::eInit, everValid, &::l3ApertureChanged));
        m_remoteParas.insert(L3_SW,                std::make_shared<DevicePara>(this, (L3_SW),false,DevicePara::Types::eInt,1.0f,DevicePara::eInit, everValid, &::l3SwChanged));

        //    m_remoteParas.insert(Paras::Id::MAX_ENERGY,              std::make_shared<DevicePara>(this, (Paras::Id::MAX_ENERGY), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(F_STOP_RANGE,         std::make_shared<DevicePara>(this, (F_STOP_RANGE), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit ));
        m_remoteParas.insert(F_STOP_MAX,           std::make_shared<DevicePara>(this, (F_STOP_MAX), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit ));
        m_remoteParas.insert(UPDATE_SERVICE,       std::make_shared<DevicePara>(this, (UPDATE_SERVICE), "",DevicePara::Types::eString, 1.0f, DevicePara::noOptions , everValid, checkReadoutParas));
        m_remoteParas.insert(DISPLAY_APERTURE_9,   std::make_shared<DevicePara>(this, (DISPLAY_APERTURE_9), 0, DevicePara::Types::eInt, 10.0f, DevicePara::eInit ));
        m_remoteParas.insert(PILOTLIGHT_MODE,      std::make_shared<DevicePara>(this, (PILOTLIGHT_MODE), (int)ParaSelects::CognitionLightColor::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(PILOTLIGHT_SW,        std::make_shared<DevicePara>(this, (PILOTLIGHT_SW), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(REDUCED_PILOTLIGHT,   std::make_shared<DevicePara>(this, (REDUCED_PILOTLIGHT), false, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        //    m_remoteParas.insert(Paras::Id::ENERGY_TOTAL_APPERTURE,  std::make_shared<DevicePara>(this, (Paras::Id::ENERGY_TOTAL_APPERTURE), QVariant(0),DevicePara::Types::eInt, 10.0f, _IP, &eneryValidity));
        m_remoteParas.insert(ALARM_REASON,         std::make_shared<DevicePara>(this, (ALARM_REASON), (int)ParaSelects::AlarmType::NONE,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(RDY_SOUND_MODE,       std::make_shared<DevicePara>(this, (RDY_SOUND_MODE), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(RDY_SOUND_VOLUME,     std::make_shared<DevicePara>(this, (RDY_SOUND_VOLUME), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(COLOR_TEMP,           std::make_shared<DevicePara>(this, (COLOR_TEMP), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SPEED_SW,             std::make_shared<DevicePara>(this, (SPEED_SW), false,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(T01_VALUE,            std::make_shared<DevicePara>(this, (T01_VALUE), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(T01,                  std::make_shared<DevicePara>(this, (T01), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DAILY_FLASH_COUNT,    std::make_shared<DevicePara>(this, (DAILY_FLASH_COUNT), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(TOTAL_FLASH_COUNT,    std::make_shared<DevicePara>(this, (TOTAL_FLASH_COUNT), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(STROBO_SW,            std::make_shared<DevicePara>(this, (STROBO_SW), 0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SEQUENCE,             std::make_shared<DevicePara>(this, (SEQUENCE), 1,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(INTERVAL,             std::make_shared<DevicePara>(this, (INTERVAL), 0, DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(ALTERNATE_NUMBER_DEVICES,std::make_shared<DevicePara>(this, (ALTERNATE_NUMBER_DEVICES), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(ALTERNATE_DEVICE_DELAY,std::make_shared<DevicePara>(this, (ALTERNATE_DEVICE_DELAY), 0, DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DELAY,                std::make_shared<DevicePara>(this, (DELAY), 0,DevicePara::Types::eInt, 1000.0f, DevicePara::eInit));
        m_remoteParas.insert(FLASH_TEST,           std::make_shared<DevicePara>(this, (FLASH_TEST),0,DevicePara::Types::eInt));
        m_remoteParas.insert(SLOW_CHARGING,        std::make_shared<DevicePara>(this, (SLOW_CHARGING),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SCORO_ENERGY_TYPE,    std::make_shared<DevicePara>(this, (SCORO_ENERGY_TYPE),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(GEN_APERTURE,         std::make_shared<DevicePara>(this, (GEN_APERTURE),0,DevicePara::Types::eInt, 10.0f, DevicePara::eInit));
        m_remoteParas.insert(IR_RFS_SW,            std::make_shared<DevicePara>(this, (IR_RFS_SW),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(CELL_SW,               std::make_shared<DevicePara>(this, (CELL_SW),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
//        m_remoteParas.insert(RFS_SW,                std::make_shared<DevicePara>(this, (RFS_SW),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(SENSITIVITY_PHOTO_CELL,std::make_shared<DevicePara>(this, (SENSITIVITY_PHOTO_CELL),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(RESET_FLASH_COUNTER,   std::make_shared<DevicePara>(this, (RESET_FLASH_COUNTER),0,DevicePara::Types::eInt, 1.0f));
        m_remoteParas.insert(TEST_KEY_INTENSITY,    std::make_shared<DevicePara>(this, (TEST_KEY_INTENSITY),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(DISPLAY_INTENSITY,    std::make_shared<DevicePara>(this, (DISPLAY_INTENSITY),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(NW_JOIN,              std::make_shared<DevicePara>(this, (NW_JOIN),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        m_remoteParas.insert(NW_SSID,              std::make_shared<DevicePara>(this, (NW_SSID),"Bron-Studio",DevicePara::Types::eString, DevicePara::eInit));
        m_remoteParas.insert(NW_PWD,               std::make_shared<DevicePara>(this, (NW_PWD),"bronControl",DevicePara::Types::eString, DevicePara::eInit));
    }
}

Framework::DeviceScoro::DeviceScoro()
    : Device(false)
    , m_isInitialized(false)
    , m_isInDeleter(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
    initParas();
}

Framework::DeviceScoro::DeviceScoro(int lampAddr, bool demo)
    : Device(demo)
    , m_isInitialized(false)
    , m_isInDeleter(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this)){
    initParas(lampAddr, demo);
}

DeviceScoro::DeviceScoro(std::weak_ptr<CreatorDevice> creator)
    : Device(false)
    , m_isInitialized(false)
    , m_isInDeleter(false)
    , m_remoteParas()
    , m_defaultPara(std::make_shared<DefaultPara>(this))
    , m_oldWinkIndicator(0){
    if(auto cd = creator.lock()){
        setInterfaceIx(cd->getInterfaceIx());
//    setBcastDevice(cd->getBcastDevice());
        initParas();
        setHostAddress(cd->getHostAddress(), cd->getHostPort());
        *(getPara(DEVICE_DESC)) = *(cd->getPara(CreatorDevice::DEVICE_DESC));
        *(getPara(DEVICE_NAME)) = *(cd->getPara(CreatorDevice::DEVICE_NAME));
        *(getPara(STUDIO_ADDR)) = *(cd->getPara(CreatorDevice::STUDIO_ADDRESS));
        *(getPara(NW_SSID)) = *(cd->getPara(CreatorDevice::SSID_GENI));
        *(getPara(NW_PWD)) = *(cd->getPara(CreatorDevice::PWD_GENI));
        connect(m_rescanTimer, SIGNAL(timeout()), this, SLOT(doRescan()));
        m_rescanTimer->setInterval(eRescanTime);
        m_rescanTimer->setSingleShot(false);
//      rescanTimer is started on initDone
//        emit startRescanTimer();
    }
}

Framework::DeviceScoro::DeviceScoro(const DeviceScoro &sc)
    : Device(sc.getDemo())
    , m_isInitialized(sc.m_isInitialized)
    , m_isInDeleter(sc.m_isInDeleter)
    , m_remoteParas(sc.m_remoteParas)
    , m_defaultPara(std::make_shared<DefaultPara>(this))
    , m_oldWinkIndicator(0){
//    qDebug() << "Scoro copy contructor";
}

//protected boolean ecoEnabled = false; sirrosL
//protected float energyCur = 0f;

Framework::DeviceScoro::~DeviceScoro(){
//    qDebug() << "destroy scoro"  << getHiddenId();
    for(unsigned long i=0; i < (sizeof(m_hasLamp)/sizeof(int)); i++){
        if(hasLamp(i)){
            if(auto gl = removeLamp(i).lock()){
                if(!m_isInDeleter){
                    emit gl->deviceLost(gl);
                }
            }
        }
    }
    m_remoteParas.clear();
}


QList<std::shared_ptr<DevicePara> > Framework::DeviceScoro::GetAllParas() const{
    return m_remoteParas.values();
}

QList<std::shared_ptr<DevicePara> > Framework::DeviceScoro::GetPollParas()const{
    QList<std::shared_ptr<DevicePara> > poll;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::ePoll)){
            poll.append(p);
        }
    }
    return poll;
}



QString Framework::DeviceScoro::getName() const{
    int type = getPara(DEVICE_DESC)->data().toInt();
    QString name = "?";
    switch(type){
    case 1: name = "Scoro 1600 E"; break;
    case 2: name = "Scoro 3200 E"; break;
    case 3: name = "Scoro 1600 S"; break;
    case 4: name = "Scoro 3200 S"; break;
    }

    return name;
}



ParaSelects::AlarmType Framework::DeviceScoro::getAlarm() const{
// to do check how to get the alarm state
    return ParaSelects::AlarmType::NONE;
}

bool Framework::DeviceScoro::hasLamp(std::weak_ptr<GenLamp> d){
    if(auto dev = d.lock()){
        QListIterator<std::weak_ptr<GenLamp> > i(m_lamps);
        while (i.hasNext()) {
            if(auto id = i.next().lock()){
                if(id->getHiddenId() == dev->getHiddenId()){
                    return true;
                }
            }
        }
    }
    return false;
}

void Framework::DeviceScoro::addLamp(std::weak_ptr<GenLamp> la){
    if(auto lamp = la.lock()){
        int nr = lamp->getNr();
        m_hasLamp[nr] = true;
        if(m_lamps.size() == 0){
            m_lamps.insert(0,lamp);
            setRequestLimit(1);
            lamp->setRequestLimit(1);
        }
        else{
            bool insert = false;
            QMutableListIterator<std::weak_ptr<GenLamp> >iter(m_lamps);
            while(iter.hasNext()){
                if(auto d = iter.next().lock()){
                    if(d->getNr() > nr){
                        iter.insert(lamp);
                        setRequestLimit(1);
                        lamp->setRequestLimit(1);
                        insert = true;
                        break;
                    }
                }
            }
            if(false == insert){
                m_lamps.insert(3, lamp);
                setRequestLimit(1);
                lamp->setRequestLimit(1);
            }
        }
    }
}


std::weak_ptr<GenLamp> Framework::DeviceScoro::removeLamp(int nr){
    m_hasLamp[nr] = false;
    QMutableListIterator<std::weak_ptr<GenLamp> >iter(m_lamps);
    while(iter.hasNext()){
        if(auto d = iter.next().lock()){
            if(d->getNr() == nr){
                iter.remove();
                return d;
            }
        }
    }
    if(m_lamps.size() == 0){
        setRequestLimit(5);
    }
    return std::weak_ptr<GenLamp>();
}



const QString Framework::DeviceScoro::IdString(int id) const{
    const QMetaEnum &me = QMetaEnum::fromType<DeviceScoro::Ids>();
    const char *key = me.valueToKey(static_cast<DeviceScoro::Ids>(id));
    if(key){
        return QString(key);
    }
    else{
        return QString("DeviceScoro unknown Id: " + QString::number(id));
    }
}

QList<std::shared_ptr<DevicePara> > Framework::DeviceScoro::GetInitParas()const{
    QList<std::shared_ptr<DevicePara> > init;
    foreach(auto p, m_remoteParas){
        if(p->GetSettings().testFlag(DevicePara::eInit)){
            init.append(p);
        }
    }
    return init;
}

bool Framework::DeviceScoro::increasePower(float val){
    float currEnergy = getApertureAct();
    float increment = val;

//    qDebug() << "DeviceScoro::increasePower(" << val << ")"
//             << " currEnergy " << currEnergy
//             << " getApertureMax() " << getApertureMax();
    currEnergy = ((currEnergy * 10.0f) + (increment * 10.0f))/ 10.0f;
    if(currEnergy > getApertureMax()){
        currEnergy = getApertureMax();
    }

    getPara(GEN_APERTURE)->setRemote(QVariant::fromValue(currEnergy));

    int numLamps = 0;
    float j = 0.0f;
    float act = 0.0f;
    if(getDemo()){
        numLamps = getNumConnectedLamps();
        if(numLamps > 0){
            j = getActEnergy();
            j /= numLamps;
            act = std::log(j/getMaxEnergy()* std::pow(2.0f, getApertureMax()))/std::log(2.0f);
        }

        QListIterator<std::weak_ptr<GenLamp> > iter(m_lamps);
        while(iter.hasNext()){
            if(auto d = iter.next().lock()){
                if(d->getName() == "L1"){
                    if(d->getDemo()){
                        d->getPara(GenLamp::L1_APERTURE)->setRemote(act);
                    }
                    d->getPara(GenLamp::L1_APERTURE)->getRemote();
                }
                else if(d->getName() == "L2"){
                    if(d->getDemo()){
                        d->getPara(GenLamp::L2_APERTURE)->setRemote(act);
                    }
                    d->getPara(GenLamp::L2_APERTURE)->getRemote();
                }
                else if(d->getName() == "L3"){
                    if(d->getDemo()){
                        d->getPara(GenLamp::L3_APERTURE)->setRemote(act);
                    }
                    d->getPara(GenLamp::L3_APERTURE)->getRemote();
                }
            }
        }
    }
 return true;
}

bool Framework::DeviceScoro::decreasePower(float val){
    float currEnergy = getApertureAct();
    float increment = val;
    currEnergy = ((currEnergy * 10.0f) - (increment * 10.0f))/ 10.0f;
    if(currEnergy < getApertureMin()){
        currEnergy = getApertureMin();
    }

    getPara(GEN_APERTURE)->setRemote(QVariant::fromValue(currEnergy));
//    getPara(T01_VALUE)->getRemote();

    int numLamps = 0;
    float j = 0.0f;
    float act = 0.0f;
    if(getDemo()){
        numLamps = getNumConnectedLamps();
        if(numLamps > 0){
            j = getActEnergy();
            j /= numLamps;
            act = std::log(j/getMaxEnergy()* std::pow(2.0f, getApertureMax()))/std::log(2.0f);
        }

        QListIterator<std::weak_ptr<GenLamp> > iter(m_lamps);
        while(iter.hasNext()){
            if (auto d = iter.next().lock()){
                if(d->getName() == "L1"){
                    if(d->getDemo()){
                        d->getPara(GenLamp::L1_APERTURE)->setRemote(act);
                    }
                    d->getPara(GenLamp::L1_APERTURE)->getRemote();
                }
                else if(d->getName() == "L2"){
                    if(d->getDemo()){
                        d->getPara(GenLamp::L2_APERTURE)->setRemote(act);
                    }
                    d->getPara(GenLamp::L2_APERTURE)->getRemote();
                }
                else if(d->getName() == "L3"){
                    if(d->getDemo()){
                        d->getPara(GenLamp::L3_APERTURE)->setRemote(act);
                    }
                    d->getPara(GenLamp::L3_APERTURE)->getRemote();
                }
            }
        }
    }
    return true;
}

bool Framework::DeviceScoro::setSpeed(ParaSelects::SpeedMode val){
    if(getName() == "Scoro 1600 E" || getName() == "Scoro 3200 E"){
        return false;
    }
    // scoro supports only normal and speed
    if(val == ParaSelects::SpeedMode::Normal || val == ParaSelects::SpeedMode::Speed){
        return getPara(SPEED_SW)->setRemote(static_cast<int>(val));
    }
    else{
        return false;
    }
}

//ALTERNATE_NUMBER_DEVICES ,/* -c 2<=n<=4, e.g. n=2 means that 2 devices take part with alternate and therefore, the device releases every second trigger */
//ALTERNATE_DEVICE_DELAY    /* -c n=0 off (renders the above inactive), n=1 active, n=2-4 wait[1-3]. E.g. n=1 means release on first trigger. Always set the above in advance */

int Framework::DeviceScoro::getApplication() const{
    int ret = getPara(ALTERNATE_NUMBER_DEVICES)->data().toInt();
    if(ret){
        ret--;
    }
    return ret;
}

bool Framework::DeviceScoro::setApplication(int val){
    switch(static_cast<ParaSelects::FreemaskMode>(val)){
    case ParaSelects::FreemaskMode::DISABLED:           return getPara(ALTERNATE_NUMBER_DEVICES)->setRemote(2);
    case ParaSelects::FreemaskMode::FREEMASK:           return getPara(ALTERNATE_NUMBER_DEVICES)->setRemote(2);
    case ParaSelects::FreemaskMode::ALTERNATE_3_MASKS:  return getPara(ALTERNATE_NUMBER_DEVICES)->setRemote(3);
    case ParaSelects::FreemaskMode::ALTERNATE_4_MASKS:  return getPara(ALTERNATE_NUMBER_DEVICES)->setRemote(4);
    }

    return false;
}

int Framework::DeviceScoro::getMaskGroup() const{
    return getPara(ALTERNATE_DEVICE_DELAY)->data().toInt();
}

bool Framework::DeviceScoro::setMaskGroup(int val){
    return getPara(ALTERNATE_DEVICE_DELAY)->setRemote(val);
}

bool Framework::DeviceScoro::setRemoteControl(ParaSelects::RemoteCtrl ctrl){
    switch(ctrl){
    case ParaSelects::RemoteCtrl::RcOff:
        getPara(CELL_SW)->setRemote(false);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::off));
        break;
    case ParaSelects::RemoteCtrl::RcRfs:
        getPara(CELL_SW)->setRemote(false);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::rfs));
        break;
    case ParaSelects::RemoteCtrl::RcIr:
        getPara(CELL_SW)->setRemote(false);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::ir));
        break;
    case ParaSelects::RemoteCtrl::RcIrRfs:
        getPara(CELL_SW)->setRemote(false);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::ir_rfs));
        break;
    case ParaSelects::RemoteCtrl::RcCell:
        getPara(CELL_SW)->setRemote(true);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::off));
        break;
    case ParaSelects::RemoteCtrl::RcCellRfs:
        getPara(CELL_SW)->setRemote(true);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::rfs));
        break;
    case ParaSelects::RemoteCtrl::RcCellIr:
        getPara(CELL_SW)->setRemote(true);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::ir));
        break;
    case ParaSelects::RemoteCtrl::RcCellIrRfs:
        getPara(CELL_SW)->setRemote(true);
        getPara(IR_RFS_SW)->setRemote(static_cast<int>(ParaSelects::RemoteCtrlSwitch::ir_rfs));
        break;
    }
    return true;
}

int DeviceScoro::getRemoteControl() const{
    if(getPara(CELL_SW)->data().toBool()){
        switch(static_cast<ParaSelects::RemoteCtrlSwitch>(getPara(IR_RFS_SW)->data().toInt())){
        case ParaSelects::RemoteCtrlSwitch::off: return static_cast<int>(ParaSelects::RemoteCtrl::RcCell);
        case ParaSelects::RemoteCtrlSwitch::ir:  return static_cast<int>(ParaSelects::RemoteCtrl::RcCellIr);
        case ParaSelects::RemoteCtrlSwitch::rfs: return static_cast<int>(ParaSelects::RemoteCtrl::RcCellRfs);
        case ParaSelects::RemoteCtrlSwitch::ir_rfs: return static_cast<int>(ParaSelects::RemoteCtrl::RcCellIrRfs);
        }
    }
    else{
        switch(static_cast<ParaSelects::RemoteCtrlSwitch>(getPara(IR_RFS_SW)->data().toInt())){
        case ParaSelects::RemoteCtrlSwitch::off: return static_cast<int>(ParaSelects::RemoteCtrl::RcOff);
        case ParaSelects::RemoteCtrlSwitch::ir:  return static_cast<int>(ParaSelects::RemoteCtrl::RcIr);
        case ParaSelects::RemoteCtrlSwitch::rfs: return static_cast<int>(ParaSelects::RemoteCtrl::RcRfs);
        case ParaSelects::RemoteCtrlSwitch::ir_rfs: return static_cast<int>(ParaSelects::RemoteCtrl::RcIrRfs);
        }
    }
    return -1; // should never happen, all pathes are covered with the above code
}

void Framework::DeviceScoro::makeApplicationTransparent(){
    int type = getPara(DEVICE_DESC)->data().toInt();
    // all Scoro E
    if(type == 1 || type == 2){
        QExplicitlySharedDataPointer<CommandSequence> seq = startCommandSequence("make transparent for application");
//        ParamLocker lock(*this);
        storeRemoteControl();
        setRemoteControl(ParaSelects::RemoteCtrl::RcOff);
        endCommandSequence(seq);
    }
}

void Framework::DeviceScoro::removeRemotePara(int id){
    if(m_remoteParas.contains(id)){
        m_remoteParas.remove(id);
    }
}


int Framework::DeviceScoro::migrateToNewNetwork(bool enterprize, QString ssid, QString pw, ParaSelects::EncryptionMode encr){
    Q_UNUSED(encr);
    if(enterprize){
        QExplicitlySharedDataPointer<CommandSequence> seq = startCommandSequence("migrate to network");
        QList<std::function<bool()> >postRemoteActions;
        postRemoteActions.append(std::bind(&DevicePara::setData, getPara(NW_JOIN), QVariant(enterprize), Errors::Id::E_NO_ERROR, false));
        postRemoteActions.append(std::bind(&Device::sendLost, shared_from_this()));
        bool (DevicePara::*fct)(QVariant, QList<std::function<bool()> >) = &DevicePara::setRemote;
        DeviceAction act(seq->number, std::bind(fct, getPara(NW_JOIN), QVariant(enterprize), postRemoteActions));
        seq->addAction(act);
        getPara(NW_SSID)->setRemote(ssid);
        getPara(NW_PWD)->setRemote(pw);
        endCommandSequence(seq);
    }
    else{
        QList<std::function<bool()> >postRemoteActions;
        postRemoteActions.append(std::bind(&DeviceScoro::sendCmdSequenceEnd, this));
        postRemoteActions.append(std::bind(&Device::sendLost, shared_from_this()));
        getPara(NW_JOIN)->setRemote(2, postRemoteActions);
    }
    return 1;
}

bool Framework::DeviceScoro::sendLost(){
    // if the scoro is lost, all connected lamps are lost too!
    foreach (auto lamp, m_lamps) {
       if(auto l = lamp.lock()){
            emit l->deviceLost(l);
       }
    }
    emit deviceLost(shared_from_this());
    return true;
}

bool Framework::DeviceScoro::setStandby(int val){
    QExplicitlySharedDataPointer<CommandSequence> seq = startCommandSequence("set standby");
    bool (DevicePara::*fct)(QVariant, QList<std::function<bool()> >) = &DevicePara::setRemote;
    DeviceAction act(seq->number, std::bind(fct, getPara(STANDBY_SW), QVariant(val), QList<std::function<bool()> >()));
    seq->addAction(act);
    getPara(L1_SW)->setRemote(val == 0 ? 1 : 0);
    getPara(L2_SW)->setRemote(val == 0 ? 1 : 0);
    getPara(L3_SW)->setRemote(val == 0 ? 1 : 0);

//    QListIterator<std::shared_ptr<Device> > iter(m_lamps);
//    while(iter.hasNext()){
//        GenLamp *lamp = dynamic_cast<GenLamp *>(iter.next());
//        if(lamp){
//            lamp->setStandby(val);
//        }
//    }
    endCommandSequence(seq);
    return true;
}
