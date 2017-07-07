#ifndef GLOBALINIT_H
#define GLOBALINIT_H

#include "NetWorkScanner.h"
#include "deviceFinder.h"
#include "studiomgr.h"
#include "networkapi_impl.h"

/**
  * @abstract used to bind the singeltons to the application for automatic deletion
 */
using namespace Framework;

void stopCommunication(){
    QExplicitlySharedDataPointer<StudioMgr> studioMgr = StudioMgr::getInstance();
    if(studioMgr){
        studioMgr->setOffline(true);
    }
}

#endif // GLOBALINIT_H
