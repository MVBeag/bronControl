#include "devicesirrosS.h"
#include "paraSelectables.h"
#include "demodevicepara.h"


using namespace Framework;

Framework::DeviceSirosS::DeviceSirosS()
    : DeviceSiros(){
}

Framework::DeviceSirosS::DeviceSirosS(int addr, bool demo)
    : DeviceSiros(addr, demo){
    if(this->getDemo()){
        insertRemotePara(SPEED,                   std::make_shared<DemoDevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
    }
    else{
        insertRemotePara(SPEED,                   std::make_shared<DevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
    }

}

DeviceSirosS::DeviceSirosS(std::weak_ptr<CreatorDevice> creator)
    : DeviceSiros(creator){
    if(this->getDemo()){
        insertRemotePara(SPEED,                   std::make_shared<DemoDevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
    }
    else{
        insertRemotePara(SPEED,                   std::make_shared<DevicePara>(this, (SPEED), (int)ParaSelects::SpeedMode::Normal,DevicePara::Types::eInt, 1.0f, DevicePara::eInit));
    }
}

Framework::DeviceSirosS::~DeviceSirosS(){}

const QString Framework::DeviceSirosS::IdString(int id) const{
    const QMetaEnum &me = QMetaEnum::fromType<DeviceSirosS::Ids>();
    const char *key = me.valueToKey(id);
    if(key){
        return QString(key);
    }
    else{
        return QString("DeviceSirosS unknown Id: " + QString::number(id));
    }
}
