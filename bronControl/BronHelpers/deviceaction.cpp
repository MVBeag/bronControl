#include "deviceaction.h"

Framework::DeviceAction::DeviceAction(uint32_t seq, std::function<bool()> fct)
    : m_sequence(seq)
    , m_fct(fct){}
