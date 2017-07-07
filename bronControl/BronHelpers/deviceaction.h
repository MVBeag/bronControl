#ifndef DEVICEACTION_H
#define DEVICEACTION_H

#include <stdint.h>
#include <QVariant>
#include <functional>
#include "devicepara.h"

namespace Framework{

class DeviceAction{
    friend class CommandSequence;
public:
    DeviceAction(uint32_t seq, std::function<bool()> fct);
    bool triggerAction();

private:
    uint32_t m_sequence;
    std::function<bool()> m_fct;
};

inline bool DeviceAction::triggerAction(){
    if(m_fct){
        m_fct();
        return true;
    }
    return false;
}


}
#endif // DEVICEACTION_H
