#include "devicesirrosL.h"
#include "paraSelectables.h"
#include "demodevicepara.h"

using namespace Framework;

Framework::DeviceSirosL::DeviceSirosL()
    : DeviceSiros(){
    removeRemotePara(DeviceSiros::Ids::MODEL_LIGHT_MODE);
}

Framework::DeviceSirosL::DeviceSirosL(int addr, bool demo)
    : DeviceSiros(addr,demo){
    removeRemotePara(DeviceSiros::Ids::MODEL_LIGHT_MODE);
    if(this->getDemo()){
        insertRemotePara(SPEED,                   std::make_shared<DemoDevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        insertRemotePara(SLOW_CHARGE,             std::make_shared<DemoDevicePara>(this, (SLOW_CHARGE),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
    }
    else{
        insertRemotePara(SPEED,                   std::make_shared<DevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        insertRemotePara(SLOW_CHARGE,             std::make_shared<DevicePara>(this, (SLOW_CHARGE),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
    }
}

Framework::DeviceSirosL::DeviceSirosL(std::weak_ptr<CreatorDevice> creator)
    : DeviceSiros(creator){
    if(auto cd = creator.lock()){
//    qDebug()<< "create SirosL from CreatorDevice "  << getHiddenId();
        removeRemotePara(DeviceSiros::Ids::MODEL_LIGHT_MODE);
        if(this->getDemo()){
            insertRemotePara(SPEED,                   std::make_shared<DemoDevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
            insertRemotePara(SLOW_CHARGE,             std::make_shared<DemoDevicePara>(this, (SLOW_CHARGE),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        }
        else{
            insertRemotePara(SPEED,                   std::make_shared<DevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
            insertRemotePara(SLOW_CHARGE,             std::make_shared<DevicePara>(this, (SLOW_CHARGE),0,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
        }
    }
}

Framework::DeviceSirosL::~DeviceSirosL(){
}

const QString Framework::DeviceSirosL::IdString(int id) const{
    const QMetaEnum &me = QMetaEnum::fromType<DeviceSirosL::Ids>();
    const char *key = me.valueToKey(id);
    if(key){
        return QString(key);
    }
    else{
        return QString("DeviceSirosL unknown Id: " + QString::number(id));
    }
}  
