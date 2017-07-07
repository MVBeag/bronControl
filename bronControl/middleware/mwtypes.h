#ifndef MWTYPES_H
#define MWTYPES_H

#include <stdint.h>
#include "errors.h"
#include "timerapi.h"

class MiddlewareType;

using namespace  Framework;

using MWParamIntCB = void (*)(Errors::Id errorCode, uint32_t nId, uint32_t nValue, void* context);
using MWParamStringCB = void (*)(Errors::Id errorCode, uint32_t nId, const char* cpBuffer, uint16_t nBufferSize, void* context);

/* functions, which should be registered and will be called by middleware */
using MWGetParamIntCB = Errors::Id (*)(void* context, uint32_t nId, uint32_t* pRetValue);
using MWGetParamStringCB = Errors::Id (*)(void* context, uint32_t nId, char* pBuffer, uint16_t nBufferSize, uint16_t* pUsedBufferSize);

using MWSetParamIntCB =  Errors::Id (*)(void* context, uint32_t nId, uint32_t nSetValue);
using MWSetParamStringCB = Errors::Id (*)(void* context, uint32_t nId, const char* cpBuffer, uint16_t nBufferSize);

/**** middleware init/shutdown ****/
//typedef void (MWAsyncResultCB)(void* context, MWErrorCode errorCode);
//typedef MWAsyncResultCB MWInitCB;

struct MWParamCallbacks {
    MWGetParamIntCB pGetParamInt;
    MWGetParamStringCB pGetParamString;
    MWSetParamIntCB pSetParamInt;
    MWSetParamStringCB pSetParamString;
};

//struct OutStandingRequest{
//  bool m_bUsed{false};
//  uint32_t m_nSequence{0};
//  uint32_t m_Id{0};
//  ICmdResponse *m_rsp{nullptr};
////  MWParamIntCB m_pIntCallback;
////  MWParamStringCB m_pStringCallback;
//  void* m_pArg{nullptr};
//  TimerAPI * m_timeoutTimer{nullptr};
//  MiddlewareType *m_mw{nullptr};
//};



#endif // MWTYPES_H
