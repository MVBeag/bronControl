#include <QMetaEnum>
#include <random>
#include <QtAlgorithms>
#include <QAtomicInteger>
#include "sequencegenerator.h"
#include "device.h"
#include "errors.h"
#include "Message.h"
#include "ethaddress.h"
#include "networkapi_impl.h"
#include "studiomgr.h"
#include "remoteaddressinfo.h"


using namespace Framework;
static SequenceGenerator gSequence;
static int gHiddenId = 0;                       // internal instance counter, used to compare loaded devices or for debugging purposes,
                                                // is not suitable in case of copying a studio (use ethernet address or tag if applicable)
const float Device::MAX_APERTURE = 10.0f;       // max possible aperture, per definition 10 up to now (jan 2017)


/*--- defines ----------------------------------------------------------------*/
#define REQUEST_TIMEOUT_IN_MS (3000)
#define OBSERVING_TIMER_IN_MS (2000)

#define MAX_TIMEOUT_COUNTER   (4)

/*--- types ------------------------------------------------------------------*/

typedef enum _ParamValueType
{
  ParamValueType_Int,
  ParamValueType_String
} ParamValueType;

static ssize_t sEncodeParamGetMessage(void* pBuffer,
                                      unsigned int nBufferSize,
                                      uint32_t nId,
                                      uint32_t nSequence,
                                      ParamValueType paramValType)
{
  Message_t myMessage;
  asn_enc_rval_t encodeResult;
  ssize_t nRetVal = 0;

  (void)memset(&myMessage, 0, sizeof(Message_t));
  if (0 == asn_ulong2INTEGER(&(myMessage.id), (unsigned long)nId))
  {
    if (0 == asn_ulong2INTEGER(&(myMessage.sequence), (unsigned long)nSequence))
    {
      switch (paramValType)
      {
      case ParamValueType_Int:
        myMessage.data.present = MessageData_PR_mgi;
        break;
      case ParamValueType_String:
      default:
        myMessage.data.present = MessageData_PR_mgs;
      } /* switch */
      encodeResult = der_encode_to_buffer(&asn_DEF_Message,
                                          &myMessage,
                                          pBuffer,
                                          nBufferSize);
      if(encodeResult.encoded > 0)
      { /* send message */
        nRetVal = encodeResult.encoded;
      }
    }
  }

  asn_DEF_Message.free_struct(&asn_DEF_Message, &myMessage, 1);
  return(nRetVal);
} /* sEncodeParamGetMessage */

/*----------------------------------------------------------------------------*/

static ssize_t sEncodeParamSetIntMessage(void* pBuffer,
                                         unsigned int nBufferSize,
                                         uint32_t nId,
                                         uint32_t nSequence,
                                         uint32_t nValue)
{
  Message_t myMessage;
  asn_enc_rval_t encodeResult;
  ssize_t nRetVal = 0;

  (void)memset(&myMessage, 0, sizeof(Message_t));
  if (0 == asn_ulong2INTEGER(&(myMessage.id), (unsigned long)nId))
  {
    if (0 == asn_ulong2INTEGER(&(myMessage.sequence), (unsigned long)nSequence))
    {
      myMessage.data.present = MessageData_PR_ms;
      myMessage.data.choice.ms.value.present = ParamValue_PR_int;

      if (0 == asn_ulong2INTEGER(&(myMessage.data.choice.ms.value.choice.Int),
                                 (unsigned long)nValue))
      {
        encodeResult = der_encode_to_buffer(&asn_DEF_Message,
                                            &myMessage,
                                            pBuffer,
                                            nBufferSize);
        if(encodeResult.encoded > 0)
        { /* send message */
          nRetVal = encodeResult.encoded;
        }
      }
    }
  }

  asn_DEF_Message.free_struct(&asn_DEF_Message, &myMessage, 1);
  return(nRetVal);
} /* sEncodeParamSetIntMessage */

/*----------------------------------------------------------------------------*/

static ssize_t sEncodeParamSetStringMessage(void* pBuffer,
                                            unsigned int nBufferSize,
                                            uint32_t nId,
                                            uint32_t nSequence,
                                            const char* cpData,
                                            int nDataSize)
{
  Message_t myMessage;
  asn_enc_rval_t encodeResult;
  ssize_t nRetVal = 0;

  (void)memset(&myMessage, 0, sizeof(Message_t));
  if (0 == asn_ulong2INTEGER(&(myMessage.id), (unsigned long)nId))
  {
    if (0 == asn_ulong2INTEGER(&(myMessage.sequence), (unsigned long)nSequence))
    {
      myMessage.data.present = MessageData_PR_ms;
      myMessage.data.choice.ms.value.present = ParamValue_PR_data;

      if (0 == OCTET_STRING_fromBuf(
                                 &(myMessage.data.choice.ms.value.choice.data),
                                 cpData,
                                 nDataSize))
      {
        encodeResult = der_encode_to_buffer(&asn_DEF_Message,
                                            &myMessage,
                                            pBuffer,
                                            nBufferSize);
        if(encodeResult.encoded > 0)
        { /* send message */
          nRetVal = encodeResult.encoded;
        }
      }
    }
  }

  asn_DEF_Message.free_struct(&asn_DEF_Message, &myMessage, 1);
  return(nRetVal);
} /* sEncodeParamSetStringMessage */

/*----------------------------------------------------------------------------*/

static ssize_t sEncodeResultMessageInt(void* pBuffer,
                                       unsigned int nBufferSize,
                                       uint32_t nId,
                                       uint32_t nSequence,
                                       uint32_t nResult)
{
  Message_t myMessage;
  asn_enc_rval_t encodeResult;
  ssize_t nRetVal = 0;

  (void)memset(&myMessage, 0, sizeof(Message_t));
  if (0 == asn_ulong2INTEGER(&(myMessage.id), (unsigned long)nId))
  {
    if (0 == asn_ulong2INTEGER(&(myMessage.sequence), (unsigned long)nSequence))
    {
      myMessage.data.present = MessageData_PR_mr;
      myMessage.data.choice.mr.result.present = ParamValue_PR_int;
      if (0 == asn_ulong2INTEGER(&(myMessage.data.choice.mr.result.choice.Int),
                                 (unsigned long)nResult))
      {
        encodeResult = der_encode_to_buffer(&asn_DEF_Message,
                                            &myMessage,
                                            pBuffer,
                                            nBufferSize);
        if(encodeResult.encoded > 0)
        { /* send message */
          nRetVal = encodeResult.encoded;
        }
      }
    }
  }

  asn_DEF_Message.free_struct(&asn_DEF_Message, &myMessage, 1);
  return(nRetVal);
} /* sEncodeResultMessageInt */

/*----------------------------------------------------------------------------*/

static ssize_t sEncodeResultMessageString(void* pBuffer,
                                          unsigned int nBufferSize,
                                          uint32_t nId,
                                          uint32_t nSequence,
                                          const char* cpData,
                                          int nDataSize)
{
  Message_t myMessage;
  asn_enc_rval_t encodeResult;
  ssize_t nRetVal = 0;

  (void)memset(&myMessage, 0, sizeof(Message_t));
  if (0 == asn_ulong2INTEGER(&(myMessage.id), (unsigned long)nId))
  {
    if (0 == asn_ulong2INTEGER(&(myMessage.sequence), (unsigned long)nSequence))
    {
      myMessage.data.present = MessageData_PR_mr;
      myMessage.data.choice.mr.result.present = ParamValue_PR_data;

      if (0 == OCTET_STRING_fromBuf(
                          &(myMessage.data.choice.mr.result.choice.data),
                          cpData,
                          nDataSize))
      {
        encodeResult = der_encode_to_buffer(&asn_DEF_Message,
                                            &myMessage,
                                            pBuffer,
                                            nBufferSize);
        if(encodeResult.encoded > 0)
        { /* send message */
          nRetVal = encodeResult.encoded;
        }
      }
    }
  }

  asn_DEF_Message.free_struct(&asn_DEF_Message, &myMessage, 1);
  return(nRetVal);
} /* sEncodeResultMessageString */

/*----------------------------------------------------------------------------*/

static ssize_t sEncodeErrorMessage(void* pBuffer,
                                   unsigned int nBufferSize,
                                   uint32_t nId,
                                   uint32_t nSequence,
                                   Errors::Id errCode)
{
  Message_t myMessage;
  asn_enc_rval_t encodeResult;
  ssize_t nRetVal = 0;

  (void)memset(&myMessage, 0, sizeof(Message_t));
  if (0 == asn_ulong2INTEGER(&(myMessage.id), (unsigned long)nId))
  {
    if (0 == asn_ulong2INTEGER(&(myMessage.sequence), (unsigned long)nSequence))
    {
      myMessage.data.present = MessageData_PR_me;
      if (0 == asn_ulong2INTEGER(&(myMessage.data.choice.me.error), (unsigned long)errCode))
      {
        encodeResult = der_encode_to_buffer(&asn_DEF_Message,
                                            &myMessage,
                                            pBuffer,
                                            nBufferSize);
        if(encodeResult.encoded > 0)
        { /* send message */
          nRetVal = encodeResult.encoded;
        }
      }
    }
  }

  asn_DEF_Message.free_struct(&asn_DEF_Message, &myMessage, 1);
  return(nRetVal);
} /* sEncodeErrorMessage */





/**
 * @brief Framework::Device::getIsDeviceAwareness
 * @return check, if device is the same, (based on MAC Address), to get the MAC address,
 * an ARP Process is started. IOS doesn't allow starting new Processes
 */
bool Framework::Device::getIsDeviceAwareness() const{
#ifdef Q_OS_IOS
    return false;
#else
    return m_devAwareness;
#endif
}

void Framework::Device::setIsDeviceAwareness(bool isDeviceAwareness){
#ifdef Q_OS_IOS
    Q_UNUSED(isDeviceAwareness)
    m_devAwareness = false;
#else
    m_devAwareness = isDeviceAwareness;
#endif
}



Framework::Device::Device(bool demo)
    : QObject()
    , IPersistency()
    , m_isOnline(false)
    , m_demo(demo)
    , m_sm(*this)
    , m_workerThread(nullptr)
    , m_retries(eRetries)
    , m_requests(0)
    , m_reqLimit(5)
    , m_interfaceIx(0xffff)
    , m_autoDeleteTimer()
    , m_winkTimer()
    , m_rescanTimer(new QTimer())
    , m_appParaStoreage()
    , m_storeTempParas(false)
    , m_isScanDevice(false)
    , m_isPolling(true)
    , m_tPolling(true)
    , m_tRescanTimerActive(false)
    , m_nestedSequencesLevel(0)
    , m_hiddenId(gHiddenId++)
    , m_oldRemoteCtrl(ParaSelects::RemoteCtrl::RcOff)
    , m_winkActive(false)
    , m_winkCounter(9)
    , m_network(StudioMgr::getInstance()->getNetAccess())
    , m_ethAddress()
    , m_hostAddress()
    , m_hostPort(MW_DEVICE_DEFAULT_SERVICE_PORT)
    , m_devAwareness(false)
    , m_backup()
    , m_runningSequences()
    , m_runningRequests()
    , m_mutex(QMutex::Recursive){
    construct();
}

Framework::Device::Device()
    : QObject()
    , IPersistency()
    , m_isOnline(false)
    , m_demo(false)
    , m_sm(*this)
    , m_workerThread(nullptr)
    , m_retries(eRetries)
    , m_requests(0)
    , m_reqLimit(5)
    , m_interfaceIx(0xffff)
    , m_autoDeleteTimer()
    , m_winkTimer()
    , m_rescanTimer(new QTimer())
    , m_appParaStoreage()
    , m_storeTempParas(false)
    , m_isScanDevice(false)
    , m_isPolling(true)
    , m_tPolling(true)
    , m_tRescanTimerActive(false)
    , m_nestedSequencesLevel(0)
    , m_hiddenId(gHiddenId++)
    , m_oldRemoteCtrl(ParaSelects::RemoteCtrl::RcOff)
    , m_winkActive(false)
    , m_winkCounter(9)
    , m_network(StudioMgr::getInstance()->getNetAccess())
    , m_ethAddress()
    , m_hostAddress()
    , m_hostPort(MW_DEVICE_DEFAULT_SERVICE_PORT)
    , m_devAwareness(false)
    , m_backup()
    , m_runningSequences()
    , m_runningRequests()
    , m_mutex(QMutex::Recursive){
    construct();
}

Framework::Device::~Device(){
    m_autoDeleteTimer.stop();
    m_winkTimer.stop();
//    m_rescanTimer.stop();
    m_sm.abort();
    m_workerThread->quit();
    m_workerThread->wait();

    disconnect(m_sm.getTaskTimer(), &QTimer::timeout, this, &Device::Timeout);
    disconnect(&m_autoDeleteTimer, SIGNAL(timeout()), this, SLOT(autoDeleteTimeout()));
    disconnect(&m_winkTimer, SIGNAL(timeout()), this, SLOT(onWinkTimeout()));
    disconnect(this, SIGNAL(rxResponse()), this, SLOT(retriggerAutodeleteTimeout()));
    disconnect(this, &Device::enqueueRetryQueue, &m_sm, &RemoteClientSM::onEnqueueRetryQueue);
    disconnect(this, &Device::enqueueCommandQueue, &m_sm, &RemoteClientSM::onEnqueueCommandQueue);
    disconnect(this, &Device::enqueueSequenceQueue, &m_sm, &RemoteClientSM::onEnqueueSequenceQueue);
    disconnect(this, &Device::enqueuePollingQueue, &m_sm, &RemoteClientSM::onEnqueuePollingQueue);
    disconnect(this, &Device::enqueueInitQueue, &m_sm, &RemoteClientSM::onEnqueueInitQueue);
    disconnect(this, &Device::dequeueRunningRequest, &m_sm, &RemoteClientSM::onDequeueRunningRequest);
    disconnect(this, SIGNAL(udpReadData(RemoteAddressInfo, QByteArray)), &m_sm, SLOT(onHandleRx(RemoteAddressInfo, QByteArray)));
    disconnect(this,&Device::stopWork, &m_sm, &RemoteClientSM::stopWork);
    disconnect(this, &Device::startWork, &m_sm, &RemoteClientSM::doWork);
    m_workerThread->deleteLater();

    m_runningSequences.clear();
}


void Framework::Device::Timeout(){
    m_sm.setThreadWaitTime(RemoteClientSM::eSettings::eStdWaitTime);
    if(isInit()){
        emit startWork();
    }
}

void Framework::Device::read(const QJsonObject &device){
    // fill rollback data as backup
    write(m_backup);

    // 1. read back the device paras, there are obviously the most problems
    QJsonArray paras = device["Parameter"].toArray();
    for(int i = 0; i < paras.size(); i++){
        QJsonObject para = paras[i].toObject();
        QListIterator<QString> iter(para.keys());
        while(iter.hasNext()){
            QString sel = iter.next();
            if(sel == "Address"){
                setStudioAddress(para.value(sel).toInt());
            }
            else if(sel == "LampAddress"){
                setLampAddress(para.value(sel).toInt());
            }
            else if(sel == "Delay"){
                setDelay(para.value(sel).toDouble());
            }
            else if(sel == "MaxEnergy"){
                //setMaxEnergy()}});
            }
            else if(sel == "MinEnergy"){
                //setMinEnergy()}});
            }
            else if(sel == "ActEnergy"){
                //getActEnergy()}});
            }
            else if(sel == "ModLightEna"){
                setModLight(para.value(sel).toInt());
            }
            else if(sel == "ModLightMode"){
                setModLightMode(para.value(sel).toInt());
            }
            else if(sel == "CogniColEna"){
                setCogniEnable(para.value(sel).toInt());
            }
            else if(sel == "Sequence"){
                setSequence(para.value(sel).toInt());
            }
            else if(sel == "SequenceMode"){
                setSynSequence(para.value(sel).toInt());
            }
            else if(sel == "Pause"){
                setPauseTime(para.value(sel).toDouble());
            }
            else if(sel == "FlashTimeControl"){
                setFlashTimeSetting(para.value(sel).toInt());
            }
            else if(sel == "SlowCharge"){
                    setSlowCharge(para.value(sel).toInt());
            }
            else if(sel == "FreemaskAlternate"){
                setApplication(para.value(sel).toInt());
            }
            else if(sel == "AlternateNrOfDevices"){
                setMaskGroup(para.value(sel).toInt());
            }
            else if(sel == "Speed"){
                setSpeed(static_cast<ParaSelects::SpeedMode>(para.value(sel).toInt()));
            }
            else if(sel == "ColorTempCtrl"){
                setColorTemp(para.value(sel).toInt());
            }
            else if(sel == "PilotLightReduction"){
                setDimmer(para.value(sel).toInt());
            }
            else if(sel == "SensitivityPhotoCell"){
                setCellSensitivity(para.value(sel).toInt());
            }
            else if(sel == "PhotoCellCtrl"){
                setCell(para.value(sel).toInt());
            }
            else if(sel == "IrRfsComCtrl"){
                setIR_RFS(para.value(sel).toInt());
            }
            else if(sel == "BightnessTestCtrl"){
                setBrightnessTest(para.value(sel).toInt());
            }
            else if(sel == "ReadySoundCtrl"){
                setRdySoundMode(para.value(sel).toInt());
            }
            else if(sel == "AudioVolume"){
                setAudio(para.value(sel).toInt());
            }
            else if(sel == "DisplayIntensityCtrl"){
                setDisplayIntensity(para.value(sel).toInt());
            }
            else if(sel == "DailyFlashCounter"){
                //setDailyCounter(para.value(sel))}});
            }
            else if(sel == "OverallFlashCounter"){
                //, getOverallCounter()}});
            }
            else if(sel == "CountLowEnergyFlash"){
                getCountLowEnergyFlash();
            }
            else if(sel == "CountMiddleEnergyFlash"){
                getCountMiddleEnergyFlash();
            }
            else if(sel == "CountHighEnergyFlash"){
                getCountHighEnergyFlash();
            }
        }
    }
}

void Framework::Device::write(QJsonObject &device) const{
    device["Name"]    = getName();
    device["DriverVersion"] = getDriverVersion();
    device["FirmwareVersion"] = getSWVersion();
    device["Tag"]  = getTag();
    device["NumLambs"] = getNumLamps();
    device["Type"] = getDeviceType();
    QJsonObject network;
    network["SSID"] = getSSID();
    network["Password"] = getPWD();
    network["EncryptionMode"] = getEncryptionMode();
    network["IPV4"] = getHostAddress().toString();
    network["Port"] = getHostPort();
    network["Ethernet"] = getEthernetAddress();
    network["Retries"] = getRetries();
    network["Polling"] = isPolling();
    device["Network"] = network;
    QJsonArray paras;

    paras.append(QJsonObject({{"Address", getStudioAddress()}}));
    paras.append(QJsonObject({{"LampAddress", getLampAddress()}}));
    paras.append(QJsonObject({{"Delay", getDelay()}}));
    paras.append(QJsonObject({{"MaxEnergy", getMaxEnergy()}}));
    paras.append(QJsonObject({{"MinEnergy", getMinEnergy()}}));
    paras.append(QJsonObject({{"ActEnergy", getActEnergy()}}));
    paras.append(QJsonObject({{"ModLightEna", getModLight()}}));
    paras.append(QJsonObject({{"ModLightMode", getModLightMode()}}));
    paras.append(QJsonObject({{"CogniColEna", getCogniEnable()}}));
    paras.append(QJsonObject({{"Sequence", getSequence()}}));
    paras.append(QJsonObject({{"SequenceMode", getSynSequence()}}));
    paras.append(QJsonObject({{"Pause", getPauseTime()}}));
    paras.append(QJsonObject({{"FlashTimeControl", getFlashTimeSetting()}}));
    paras.append(QJsonObject({{"SlowCharge", getSlowCharge()}}));
    paras.append(QJsonObject({{"FreemaskAlternate", getApplication()}}));
    paras.append(QJsonObject({{"AlternateNrOfDevices", getMaskGroup()}}));
    paras.append(QJsonObject({{"Speed", static_cast<int>(getSpeed())}}));
    paras.append(QJsonObject({{"ColorTempCtrl", getColorTemp()}}));
    paras.append(QJsonObject({{"PilotLightReduction", getDimmer()}}));
    paras.append(QJsonObject({{"SensitivityPhotoCell", getCellSensitivity()}}));
    paras.append(QJsonObject({{"PhotoCellCtrl", getCell()}}));
    paras.append(QJsonObject({{"IrRfsComCtrl", getIR_RFS()}}));
    paras.append(QJsonObject({{"BightnessTestCtrl", getBrightnessTest()}}));
    paras.append(QJsonObject({{"ReadySoundCtrl", getRdySoundMode()}}));
    paras.append(QJsonObject({{"AudioVolume", getAudio()}}));
    paras.append(QJsonObject({{"DisplayIntensityCtrl", getDisplayIntensity()}}));
    paras.append(QJsonObject({{"DailyFlashCounter", getDailyCounter()}}));
    paras.append(QJsonObject({{"OverallFlashCounter", getOverallCounter()}}));
    device["Parameter"] = paras;
}

void Framework::Device::response(Errors::Id errorCode, uint32_t nId, QVariant data, Request *req){

//    qDebug()  << "Device::response " << Errors(errorCode).Name() << " " << nId << " " << data.toString();

    emit dequeueRunningRequest(nId);
    bool doDeleteParameter = false;

    Errors err(errorCode);
    switch(err.GetId()){
    case Errors::Id::LOCAL_INIT_ERROR: return; // do nothing, this is a local error code, impossible in response
    case Errors::Id::LOCAL_PTR_INVALID: return; // do nothing, this is a local error code, impossible in response
    case Errors::Id::E_NO_ERROR:
        emit rxResponse();

        if(req->getQueueType() == Request::eQueueType::ePolling){
            decRequests();
        }
        if(auto p = req->getPara().lock()){
            // call the final function
            p->setData(data, errorCode);
        }
        //        sr->checkInitRequests();
        if(req->getQueueType()!= Request::eQueueType::ePolling){
            req->deleteLater(); // we are in a slot
        }
        return;
    case Errors::Id::PARAM_INEXISTENT:  doDeleteParameter = true;
    case Errors::Id::PARAM_WRONG_TYPE:
    case Errors::Id::PARAM_OUT_OF_RANGE:
    case Errors::Id::BUFFER_TOO_SMALL:
    case Errors::Id::OUT_OF_MEMORY:
    case Errors::Id::MANY_REQUESTS:
    case Errors::Id::ENCODING:
    case Errors::Id::PARAM_READ_ONLY:
    case Errors::Id::OSAPI_PORT_IN_USE:
    case Errors::Id::OSAPI_TIMER_FAIL:
    case Errors::Id::OSAPI_DID_NOT_CONNECT:
    case Errors::Id::OSAPI_CONNECT_FAILED:
    case Errors::Id::BCOM_VERSIONSMISMATCH:
    case Errors::Id::BCOM_WRONG_SERVICEID:
    case Errors::Id::BCOM_PROTOCOL_ERROR:
    case Errors::Id::CODE_OSAPI_UNKNOWN:
    case Errors::Id::CODE_UNKNOWN:
    case Errors::Id::RESOURCE_NOT_AVAIL:
        emit rxResponse();

        // inform device about error
        if(auto r = req->getPara().lock()){
            r->setData(data, errorCode);
            if(doDeleteParameter){
                r->GetElder().removeRemotePara(r->Id());
            }
        }

        switch(req->getQueueType()){
        case Request::eQueueType::eUnknown:
        case Request::eQueueType::eInit:
        case Request::eQueueType::eCommand:
        case Request::eQueueType::eSequence:
        case Request::eQueueType::eRetry:
            req->deleteLater();
            break;
        case Request::eQueueType::ePolling:
            decRequests();
            if(doDeleteParameter){
                emit removeParaFromQueue(Request::eQueueType::ePolling, req->getId());
            }
            break;
        }
        return;
    case Errors::Id::PARAM_TIMEOUT:
        // for parameters in the command queue, we don't do retries
        if(req->getRetries() < getRetries()){
            switch(req->getQueueType()){
            case Request::eQueueType::eUnknown: req->deleteLater(); return;
            case Request::eQueueType::eInit:
            case Request::eQueueType::eCommand:
            case Request::eQueueType::eRetry:   break;
            case Request::eQueueType::ePolling: decRequests();return; // no retries
            case Request::eQueueType::eSequence:
                req->incRetries();
                // use also sequence queue for retries!!!
                emit enqueueSequenceQueue(req);
                return;
            }
            // no retries on update service command
            if(auto p = req->getPara().lock()){
                if(p->GetElder().isUpdateService(req->m_Id) == false){
                    req->incRetries();
                    // enqueue to retries queue
                    retryRequest(req);
                }
            }
        }
        else{
            switch(req->getQueueType()){
            case Request::eQueueType::eUnknown:
            case Request::eQueueType::eInit:
            case Request::eQueueType::eCommand:
            case Request::eQueueType::eSequence:
            case Request::eQueueType::eRetry:   break;
            case Request::eQueueType::ePolling: decRequests();return; // no retries
            }

            if(getName() != "ScanDevice"){
                if(auto p = req->getPara().lock()){
                    p->setData(data, errorCode);
                }
//                qDebug() << "discard parameter without valid response!! "
//                         << err.Name()
//                         << IdString(static_cast<int>(nId))
//                         << getHostAddress().toString()
//                         << getName();
            }

            req->deleteLater();
        }
    }

}

Errors::Id Framework::Device::comCaller(Request *r){
    Errors::Id ret = Errors::Id::CODE_UNKNOWN;

    Request::eTypes type = r->Type();
    if(auto p = r->getPara().lock()){

        if(!p->isDemo()){
            p->startComTimer();
            switch(type){
            case Request::eGetInt:      ret = GetRemoteParamInt(r);                                                                   break;
            case Request::eGetString:   ret = GetRemoteParamString(r);                                                                break;
            case Request::eSetInt:      ret = SetRemoteParamInt(p->scaledDataForRemote().toInt(), r);                                 break;
            case Request::eSetString:   ret = SetRemoteParamString(p->stringBufferForRemote(),p->stringBufferSizeForRemote(),r);      break;
            }
        }
        else{
            ret = Errors::Id::E_NO_ERROR;
        }
    }

    return ret;

}

Request *Framework::Device::findOutstandingRequest(uint32_t nSequence){
    QMutexLocker lock(&m_mutex);
    if(m_runningRequests.contains(nSequence)){
        Request *r = m_runningRequests.value(nSequence);
        m_runningRequests.remove(nSequence);
        return r;
    }
    else{
      return(nullptr);
    }
}



Errors::Id Framework::Device::LocalGetParamInt(uint32_t nId, uint32_t* pRetValue){
  Errors::Id nErrCode = Errors::Id::E_NO_ERROR;
  uint32_t nRetVal = 0;

  switch (nId){
  case PARAM_MASTER_UNIQUE_ID:
    nRetVal = 0xbaadbeef;
    break;
  default:
    nErrCode = Errors::Id::PARAM_INEXISTENT;
  } /* switch */

  if (nullptr != pRetValue){
    *pRetValue = nRetVal;
  }

  return(nErrCode);
} /* sLocalGetParamInt */

Errors::Id Framework::Device::LocalSetParamInt(uint32_t nId, uint32_t nSetValue){
  Errors::Id nErrCode = Errors::Id::E_NO_ERROR;

  switch (nId){
  case PARAM_MASTER_UNIQUE_ID:
//    m_masterID = nSetValue;
    qDebug() << "PARAM MASTER ID set " << nSetValue;
    break;
  default:
    nErrCode = Errors::Id::PARAM_INEXISTENT;
  } /* switch */

  return(nErrCode);
}


void Framework::Device::receiveData(RemoteAddressInfo remoteAddress, const char* data, uint16_t length){
  QMutexLocker lock(&m_mutex);
  asn_dec_rval_t decodeResult;
  void* pStructure = nullptr;
  asn_codec_ctx_t codecContext;

  /* decode the ASN message */
  codecContext.max_stack_size = 0; /* disable stack checking @@@ TODO: add a correct maximum stack size */
  decodeResult = ber_decode(&codecContext, &asn_DEF_Message, &pStructure, data, length);
  if(RC_OK != decodeResult.code){
    qDebug() << "   decode error";
    if (nullptr != pStructure){
      FREEMEM(pStructure);
    }
  }
  else{
    size_t nErrBuffLen = 0; /* we are not interested in the error message */

    if (nullptr != pStructure){
      Message_t* pMessage2 = (Message_t*)pStructure;

      if(asn_check_constraints(&asn_DEF_Message, pStructure, nullptr, &nErrBuffLen)){
            qDebug() <<"    ASN.1 constraint check failed";
      }
      else{
        unsigned long nParamId_ULONG;
        unsigned long nSequence_ULONG;
        bool bDecodeSuccess = true;

        if (0 != asn_INTEGER2ulong(&(pMessage2->id), &nParamId_ULONG)){
          bDecodeSuccess = false;
        }
        if (0 != asn_INTEGER2ulong(&(pMessage2->sequence), &nSequence_ULONG)){
          bDecodeSuccess = false;
        }
        if (bDecodeSuccess){
          uint32_t nParamId = (uint32_t)nParamId_ULONG;
          uint32_t nSequence = (uint32_t)nSequence_ULONG;
          switch (pMessage2->data.present){
          case MessageData_PR_mgi: /* get request integer */{
              Errors::Id errCode;
              uint32_t nRetVal = 0;
              char aryBuffer[256];
              ssize_t nUsedBufferSize;

              if (PARAM_LAST_INTERNAL_ID >= nParamId){
                errCode = LocalGetParamInt(nParamId, &nRetVal);
              }
              else{
                errCode = Errors::Id::PARAM_INEXISTENT;
              }
              if (Errors::Id::E_NO_ERROR == errCode){
                nUsedBufferSize = sEncodeResultMessageInt(aryBuffer, sizeof(aryBuffer), nParamId, nSequence, nRetVal);
              }
              else{
                nUsedBufferSize = sEncodeErrorMessage(aryBuffer, sizeof(aryBuffer), nParamId, nSequence, errCode);
              }

              if (0 != nUsedBufferSize){
                  nUsedBufferSize++;
                  QByteArray sendData(aryBuffer, nUsedBufferSize);
                  emit send(remoteAddress, sendData, (uint16_t)nUsedBufferSize);
              }
            }
            break;

          case MessageData_PR_mgs: /* get request string */{
              Errors::Id errCode;
              char aryBuffer[2048]; /* double use:
                                        1. value string;
                                        2. encoded data */
              ssize_t nUsedBufferSize;
              uint16_t nUsedValueSize;

              if (PARAM_LAST_INTERNAL_ID >= nParamId){
                errCode = LocalGetParamString(nParamId, aryBuffer, sizeof(aryBuffer), &nUsedValueSize);
              }
              else{
                errCode = Errors::Id::PARAM_INEXISTENT;
              }
              if (Errors::Id::E_NO_ERROR == errCode){
                nUsedBufferSize = sEncodeResultMessageString(aryBuffer, sizeof(aryBuffer), nParamId, nSequence, aryBuffer, nUsedValueSize);
              }
              else{
                nUsedBufferSize = sEncodeErrorMessage(aryBuffer, sizeof(aryBuffer), nParamId, nSequence, errCode);
              }

              if (0 != nUsedBufferSize){
                  nUsedBufferSize++;
                  QByteArray sendData(aryBuffer, nUsedBufferSize);
                  emit send(remoteAddress, sendData, (uint16_t)nUsedBufferSize);
              }
            }
            break;

          case MessageData_PR_ms: /* set request */{
              Errors::Id errCode = Errors::Id::E_NO_ERROR;
              char aryBuffer[256];
              ssize_t nUsedBufferSize = 0;

              switch (pMessage2->data.choice.ms.value.present){
              case ParamValue_PR_int:{
                  unsigned long nParamValue_ULONG = 0;

                  if (0 == asn_INTEGER2ulong(&(pMessage2->data.choice.ms.value.choice.Int), &nParamValue_ULONG)){
                    uint32_t nParamValue = (uint32_t)nParamValue_ULONG;

                    if (PARAM_LAST_INTERNAL_ID >= nParamId){
                      errCode = LocalSetParamInt(nParamId, nParamValue);
                    }
                    else {
                      errCode = Errors::Id::PARAM_INEXISTENT;
                    }
                    if (Errors::Id::E_NO_ERROR == errCode){
                      nUsedBufferSize = sEncodeResultMessageInt(aryBuffer, sizeof(aryBuffer), nParamId, nSequence, nParamValue);
                    }
                  }
                  else{
                    errCode = Errors::Id::ENCODING;
                  }



              }
              break;
              case ParamValue_PR_data:
                  if (PARAM_LAST_INTERNAL_ID >= nParamId){
                    errCode = LocalSetParamString(nParamId, (char*)pMessage2->data.choice.ms.value.choice.data.buf, pMessage2->data.choice.ms.value.choice.data.size);
                  }
                  else{
                    errCode = Errors::Id::PARAM_INEXISTENT;
                  }
                  if (Errors::Id::E_NO_ERROR == errCode){
                    nUsedBufferSize = sEncodeResultMessageString(aryBuffer, sizeof(aryBuffer), nParamId, nSequence, (char*)pMessage2->data.choice.ms.value.choice.data.buf, pMessage2->data.choice.ms.value.choice.data.size);
                  }
                break;
              default:
                errCode = Errors::Id::PARAM_WRONG_TYPE;
              } /* switch */

              if (Errors::Id::E_NO_ERROR != errCode){
                nUsedBufferSize = sEncodeErrorMessage(aryBuffer,
                                                      sizeof(aryBuffer),
                                                      nParamId,
                                                      nSequence,
                                                      errCode);
              }

              if (0 != nUsedBufferSize){
                  nUsedBufferSize++;
                  QByteArray sendData(aryBuffer, nUsedBufferSize);
                  emit send(remoteAddress, sendData, (uint16_t)nUsedBufferSize);
              }
            }
            break;

          case MessageData_PR_mr: /* result response */{
//              qDebug() << "response received id " << nParamId << " " << getName();
              Request* pRequest = findOutstandingRequest(nSequence);

              if (nullptr != pRequest){
                if(pRequest->m_timeoutTimer){
                    pRequest->m_timeoutTimer->stop();
                    disconnect(pRequest->m_timeoutTimer, SIGNAL(timeout()), pRequest, SLOT(onTimeout()));
                    disconnect(pRequest, SIGNAL(stopAndDelete()), pRequest->m_timeoutTimer, SLOT(deleteLater()));
                    delete pRequest->m_timeoutTimer;
                }
//                qDebug() << "response belongs to request";
                pRequest->m_timeoutTimer = nullptr;
                switch (pMessage2->data.choice.mr.result.present){
                case ParamValue_PR_int:{
                    unsigned long nInt_ULONG = 0;

                    if (0 == asn_INTEGER2ulong(&(pMessage2->data.choice.mr.result.choice.Int), &nInt_ULONG)){
                      uint32_t nInt = (uint32_t)nInt_ULONG;
                      response(Errors::Id::E_NO_ERROR,
                                   nParamId,
                                   nInt,
                                   pRequest);
                    }
                    else{
                        qDebug() << "       decode error!!!";
                    }
                  }
                  break;

                case ParamValue_PR_data:
                    response(Errors::Id::E_NO_ERROR,
                                 nParamId,
                                 (char*)pMessage2->data.choice.mr.result.choice.data.buf,
//                                 pMessage2->data.choice.mr.result.choice.data.size,
                                 pRequest);
                break;

                default:
                  ;
                }
              }
            }
            break;

          case MessageData_PR_me: /* error response */{
//              qDebug() << "error response";
              Request* pRequest = findOutstandingRequest(nSequence);
              if (nullptr != pRequest){
                unsigned long nErrorCode_ULONG = static_cast<unsigned long>(Errors::Id::CODE_UNKNOWN);
                if(pRequest->m_timeoutTimer){
                    pRequest->m_timeoutTimer->stop();
                    disconnect(pRequest->m_timeoutTimer, SIGNAL(timeout()), pRequest, SLOT(onTimeout()));
                    disconnect(pRequest, SIGNAL(stopAndDelete()), pRequest->m_timeoutTimer, SLOT(deleteLater()));
                    delete pRequest->m_timeoutTimer;
                }
                pRequest->m_timeoutTimer = nullptr;
//                m_network->StopAndReleaseTimer(pRequest->m_timeoutTimer);
                if (0 == asn_INTEGER2ulong(&(pMessage2->data.choice.me.error), &nErrorCode_ULONG)){
                  Errors::Id nErrorCode = (Errors::Id)nErrorCode_ULONG;
                  response(nErrorCode,nParamId, QVariant(), pRequest);
                }
                else{
                    qDebug() << "error code decode error";
                }
//                delete pRequest;
              }
            }
            break;

          default:
            ;
          } /* switch */
        }
        else{
            qDebug() << "decode error";
        }
      }
      asn_DEF_Message.free_struct(&asn_DEF_Message, pStructure, 0);
    }
  }
} /* sReceiveCallback */


void Framework::Device::CreateServerConnection(){
      m_network.reset();
      m_network = StudioMgr::getInstance()->CreateConnection(m_hostPort, shared_from_this());
      if(auto net = m_network.lock()){
          connect(this, SIGNAL(send(RemoteAddressInfo, QByteArray, uint16_t)), net.get(), SLOT(onSend(RemoteAddressInfo, QByteArray, uint16_t)), Qt::QueuedConnection);
      }
}

/*----------------------------------------------------------------------------*/

void Framework::Device::initCommunication(){
  CreateServerConnection();
}

/*----------------------------------------------------------------------------*/

void Framework::Device::Shutdown(){
  if( auto net = m_network.lock()){
      disconnect(this, SIGNAL(send(RemoteAddressInfo, QByteArray, uint16_t)), net.get(), SLOT(onSend(RemoteAddressInfo, QByteArray, uint16_t)));
      net->ReleaseConnection(shared_from_this());
  }
  QMutexLocker lock(&m_mutex);
  // delete running request timeouts
  QMapIterator<uint32_t, Request *> iter(m_runningRequests);
  while(iter.hasNext()){
      iter.next();
      Request *r = iter.value();
      if(r->m_timeoutTimer){
          emit r->stopAndDelete();
          r->m_timeoutTimer = nullptr;
      }
  }
} /* mwShutdown */

void Framework::Device::outstandingRequestTimeout(Request* req){
    Request * r = findOutstandingRequest(req->m_nSequence);
    if(r != nullptr){
        response(Errors::Id::PARAM_TIMEOUT,req->getId(),QVariant(), req);
    }
}

void Framework::Device::storePara(DevicePara *para){
    if(m_storeTempParas){
        m_appParaStoreage.insert(para->Id(), para->data());
    }
}

void Framework::Device::restoreTempParas(){
    if(m_appParaStoreage.size() > 0){
        QExplicitlySharedDataPointer<CommandSequence> s = startCommandSequence("Restoring Parameter");
        for(auto it = m_appParaStoreage.begin(); it != m_appParaStoreage.end(); ){
            getPara(it.key())->setRemote(it.value());
            it = m_appParaStoreage.erase(it);
        }
        endCommandSequence(s);
    }
}

void Framework::Device::stopBackgroundUpdate(){
    QMutexLocker lock(&m_mutex);
    if(m_nestedSequencesLevel == 0){
        m_tPolling = m_isPolling;
        m_tRescanTimerActive = m_rescanTimer->isActive();
        emit stopRescanTimer();
        m_isPolling = false;
    }
    m_nestedSequencesLevel++;
}

void Framework::Device::startBackgroundUpdate(){
//    QMutexLocker lock(&m_mutex);

    if(m_nestedSequencesLevel > 0){
        m_nestedSequencesLevel--;
    }
    if(m_nestedSequencesLevel == 0){
        m_isPolling = m_tPolling;
        m_tPolling = false;
        if(m_tRescanTimerActive){
            emit startRescanTimer();
            m_tRescanTimerActive = false;
        }
    }
}

bool Framework::Device::areOpenSequences() const{
    bool ret = false;
    if(!m_runningSequences.isEmpty()){
        QMapIterator<uint32_t, QExplicitlySharedDataPointer<CommandSequence> > it(m_runningSequences);
        while(it.hasNext()){
            QExplicitlySharedDataPointer<CommandSequence>seq = it.next().value();
            if(seq->isOpen()){
                ret = true;
                break;
            }
        }
    }
    return ret;
}

/*----------------------------------------------------------------------------*/
Errors::Id Framework::Device::SendRequest(Request *pRequest, RemoteAddressInfo deviceAddress, const char* cpBuffer, int nUsedBufferSize){
    Errors::Id nErrCode = Errors::Id::CODE_UNKNOWN;
    bool isTest = false;

    if (0 != nUsedBufferSize){
        pRequest->m_timeoutTimer = nullptr;
        // no timeout in case of test flash or update service
        if(auto para = pRequest->getPara().lock()){
            isTest = para->GetElder().isTestFlash(pRequest->m_Id);
            // don't start retry timer if command should not use retries
            if(para->GetElder().checkCommandwoResponse(pRequest->m_Id) == false){
                pRequest->m_timeoutTimer = new QTimer();
                pRequest->m_timeoutTimer->setSingleShot(true);
                connect(pRequest->m_timeoutTimer, SIGNAL(timeout()), pRequest, SLOT(onTimeout()));
                connect(pRequest, SIGNAL(stopAndDelete()), pRequest->m_timeoutTimer, SLOT(deleteLater()));
                pRequest->m_timeoutTimer->start(REQUEST_TIMEOUT_IN_MS);
            }
            nUsedBufferSize++;
            QByteArray sendData(cpBuffer, nUsedBufferSize);

            emit send(deviceAddress,
                      sendData,
                      (uint16_t)nUsedBufferSize);
            nErrCode = Errors::Id::E_NO_ERROR;
        }
    }
    else{
        nErrCode = Errors::Id::ENCODING;
    }


    if (Errors::Id::E_NO_ERROR == nErrCode) {
        if(!isTest){
            m_runningRequests.insert(pRequest->m_nSequence, pRequest);
        }
    }
    return nErrCode;
} /* sSendRequest */

Errors::Id Framework::Device::GetRemoteParamInt(RemoteAddressInfo deviceAddress, Request* req){
    if(nullptr == req){
        return Errors::Id::LOCAL_INIT_ERROR;
    }
    else{
        if(auto p = req->getPara().lock()){
            if (PARAM_LAST_INTERNAL_ID >= p->Id()){
                return Errors::Id::PARAM_INEXISTENT;
            }
            else if (m_runningRequests.size() >= (MW_MAX_OUTSTANDING_REQUESTS - 1)){
                // better way, so we can use a standard queueing with reading and dequeue in case of success
                return Errors::Id::MANY_REQUESTS;
            }
            else{
                QMutexLocker lock(&m_mutex);
                char aryBuffer[256];
                int nUsedBufferSize;
                req->m_nSequence = gSequence.incSequ();
                req->m_Id = p->Id();
                nUsedBufferSize = sEncodeParamGetMessage(aryBuffer,
                                                         sizeof(aryBuffer),
                                                         p->Id(),
                                                         req->m_nSequence,
                                                         ParamValueType_Int);
                Errors::Id ret = SendRequest(req, deviceAddress, aryBuffer, nUsedBufferSize);
                if(ret == Errors::Id::E_NO_ERROR){
                    QListIterator<std::function<bool ()> > iter(req->getPostSendActions());
                    while(iter.hasNext()){
                        auto f = iter.next();
                        f();
                    }
                }
                return ret;
            }
        }
        return Errors::Id::LOCAL_PTR_INVALID;
    }
}

Errors::Id Framework::Device::SetRemoteParamInt(RemoteAddressInfo deviceAddress,
                                             uint32_t nSetValue,
                                             Request* req){
    if(nullptr == req){
        return Errors::Id::LOCAL_INIT_ERROR;
    }
    else{
        if(auto p = req->getPara().lock()){


            if (PARAM_LAST_INTERNAL_ID >= p->Id()){
                return Errors::Id::PARAM_INEXISTENT;
            }
            else if (m_runningRequests.size() >= (MW_MAX_OUTSTANDING_REQUESTS - 1)){
                return Errors::Id::MANY_REQUESTS;
            }
            else{
                QMutexLocker lock(&m_mutex);
                char aryBuffer[256];
                int nUsedBufferSize;
                req->m_nSequence = gSequence.incSequ();
                req->m_Id = p->Id();
                nUsedBufferSize = sEncodeParamSetIntMessage(aryBuffer,
                                                            sizeof(aryBuffer),
                                                            p->Id(),
                                                            req->m_nSequence,
                                                            nSetValue);
                Errors::Id ret = SendRequest(req, deviceAddress, aryBuffer, nUsedBufferSize);
                if(ret == Errors::Id::E_NO_ERROR){
                    QListIterator<std::function<bool ()> > iter(req->getPostSendActions());
                    while(iter.hasNext()){
                        auto f = iter.next();
                        f();
                    }
                }
                return ret;
            }
        }
        return Errors::Id::LOCAL_PTR_INVALID;
    }
}
/*----------------------------------------------------------------------------*/

Errors::Id Framework::Device::GetRemoteParamString(RemoteAddressInfo deviceAddress,
                                                Request* req){

    if(nullptr == req){
        return Errors::Id::LOCAL_INIT_ERROR;
    }
    else{
        if(auto p = req->getPara().lock()){
            if (PARAM_LAST_INTERNAL_ID >= p->Id()){
                return Errors::Id::PARAM_INEXISTENT;
            }
            else if (m_runningRequests.size() >= (MW_MAX_OUTSTANDING_REQUESTS - 1)){
                return Errors::Id::MANY_REQUESTS;
            }
            else{
                QMutexLocker lock(&m_mutex);
                char aryBuffer[256];
                int nUsedBufferSize;
                req->m_nSequence = gSequence.incSequ();
                req->m_Id = p->Id();
                nUsedBufferSize = sEncodeParamGetMessage(aryBuffer,
                                                         sizeof(aryBuffer),
                                                         p->Id(),
                                                         req->m_nSequence,
                                                         ParamValueType_String);
                Errors::Id ret = SendRequest(req, deviceAddress, aryBuffer, nUsedBufferSize);
                if(ret == Errors::Id::E_NO_ERROR){
                    QListIterator<std::function<bool ()> > iter(req->getPostSendActions());
                    while(iter.hasNext()){
                        auto f = iter.next();
                        f();
                    }
                }
                return ret;
            }
        }
    }
    return Errors::Id::LOCAL_PTR_INVALID;
}


Errors::Id Framework::Device::SetRemoteParamString(RemoteAddressInfo deviceAddress,
                                                 const char* cpBuffer,
                                                 uint16_t nBufferSize,
                                                 Request* req)
{

    if(nullptr == req){
        return Errors::Id::LOCAL_INIT_ERROR;
    }
    else{
        if(auto p = req->getPara().lock()){
            if (PARAM_LAST_INTERNAL_ID >= p->Id()){
                return Errors::Id::PARAM_INEXISTENT;
            }
            else if (m_runningRequests.size() >= (MW_MAX_OUTSTANDING_REQUESTS - 1)){
                return Errors::Id::MANY_REQUESTS;
            }
            else{
                QMutexLocker lock(&m_mutex);
                char aryBuffer[256];
                int nUsedBufferSize;
                req->m_nSequence = gSequence.incSequ();
                req->m_Id = p->Id();
                nUsedBufferSize = sEncodeParamSetStringMessage(aryBuffer,
                                                               sizeof(aryBuffer),
                                                               p->Id(),
                                                               req->m_nSequence,
                                                               cpBuffer,
                                                               nBufferSize);
                Errors::Id ret = SendRequest(req, deviceAddress, aryBuffer, nUsedBufferSize);
                if(ret == Errors::Id::E_NO_ERROR){
                    QListIterator<std::function<bool ()> > iter(req->getPostSendActions());
                    while(iter.hasNext()){
                        auto f = iter.next();
                        f();
                    }
                }
                return ret;
            }
        }
        return Errors::Id::LOCAL_PTR_INVALID;
    }
}


//void Framework::Device::setConnectionCBs(const MWConnectionCallbacks &connectionCBs){
//    m_connectionCBs = connectionCBs;
//}

Errors::Id Framework::Device::SetRemoteParamInt(uint32_t nSetValue, Request *req){
    RemoteAddressInfo addr = {m_hostAddress.toIPv4Address(),m_hostPort};
    return SetRemoteParamInt(addr, nSetValue, req);
}

Errors::Id Device::GetRemoteParamInt(Request *req){
    // start timing measurement for com
    RemoteAddressInfo addr = {m_hostAddress.toIPv4Address(),m_hostPort};
    return GetRemoteParamInt(addr, req);
}

Errors::Id Device::GetRemoteParamString(Request *req){
    RemoteAddressInfo addr = {m_hostAddress.toIPv4Address(),m_hostPort};
    return GetRemoteParamString(addr, req);
}


Errors::Id Device::SetRemoteParamString(const char* cpBuffer, uint16_t nBufferSize, Request *req){
    RemoteAddressInfo addr = {m_hostAddress.toIPv4Address(),m_hostPort};
    return SetRemoteParamString(addr,cpBuffer,nBufferSize,req);
}


void Framework::Device::initMW(){
    if(m_hostAddress.isNull()){
       return;
    }
    else{
        initCommunication();
    }
}

QExplicitlySharedDataPointer<CommandSequence> Framework::Device::startCommandSequence(QString name){
    uint32_t seqNr = m_sequence++;
    stopBackgroundUpdate();
    QExplicitlySharedDataPointer<CommandSequence> sequence(new CommandSequence(seqNr, name));
    connect(sequence.data(), SIGNAL(sequenceDone(uint32_t, Errors::Id)), this, SLOT(onSequenceDone(uint32_t, Errors::Id)));
    m_runningSequences.insert(seqNr, sequence);
    return sequence;
}

void Framework::Device::endCommandSequence(QExplicitlySharedDataPointer<CommandSequence>seq){
    if(m_runningSequences.contains(seq->number)){
        seq->registeringFinished();
        // maybe, there are no paras in, remove sequence and start update
        if(seq->isEmpty()){
            m_runningSequences.remove(seq->number);
            disconnect(seq.data(), SIGNAL(sequenceDone(uint32_t, Errors::Id)), this, SLOT(onSequenceDone(uint32_t, Errors::Id)));
            startBackgroundUpdate();
        }
    }
}



void Framework::Device::setHostAddress(const QHostAddress &ha, const uint16_t port, bool resolveMac){
    if(m_hostAddress == ha) return;

    m_hostAddress = ha;
    if(m_hostAddress != QHostAddress::Null){
        if(m_ethAddress.isEmpty()){
            if(resolveMac){
#if defined Q_OS_IOS
                ;
#elif defined Q_OS_ANDROID
                ;
#else
                ethAddress *eth = new ethAddress(m_hostAddress,this);
                connect(eth, &ethAddress::ethResolved, this, &Device::onEthAddressResolved);//, Qt::QueuedConnection);
#endif
            }
        }
    }

    m_hostPort = port;
}

const QHostAddress Framework::Device::getHostAddress() const  {
    return m_hostAddress;
}

uint16_t Framework::Device::getHostPort() const{
    return m_hostPort;
}

bool Framework::Device::setParameter(int id, const QVariant &val){
    return getPara(id)->setRemote(val);
}

bool Framework::Device::SetPara(std::weak_ptr<DevicePara> para, DevicePara::Types type,  QList<std::function<bool()> > postCmd){
    if(auto p = para.lock()){
        Request *r = new Request(type == DevicePara::Types::eInt ? Request::eTypes::eSetInt :
                                                                   Request::eTypes::eSetString,p,postCmd);
        if(r){
            if(areOpenSequences()){
//          check for open sequences, if there, insert para in sequence
                if(!m_runningSequences.isEmpty()){
                    QMapIterator<uint32_t, QExplicitlySharedDataPointer<CommandSequence> > it(m_runningSequences);
                    while(it.hasNext()){
                        QExplicitlySharedDataPointer<CommandSequence>seq = it.next().value();
                        if(seq->isOpen()){
                            connect(p.get(), SIGNAL(rxResponse(std::weak_ptr<DevicePara>, quint32)), seq.data(), SLOT(onRxResponse(std::weak_ptr<DevicePara>, quint32)));
                            seq->insertPara(para);
                        }
                    }
                    emit enqueueSequenceQueue(r);
                }
                else{
                  emit enqueueCommandQueue(r);
                }
            }
            else{
                emit enqueueCommandQueue(r);
            }
            return true;
        }
    }
    return false;
}

void Framework::Device::GetPara(std::weak_ptr<DevicePara> para, DevicePara::Types type){
    if(auto p = para.lock()){
        Request *r = new Request(type == DevicePara::Types::eInt ? Request::eTypes::eGetInt :
                                                                   Request::eTypes::eGetString,p);
        if(r){
            if(areOpenSequences()){
                if(!m_runningSequences.isEmpty()){
                    QMapIterator<uint32_t, QExplicitlySharedDataPointer<CommandSequence> > it(m_runningSequences);
                    while(it.hasNext()){
                        QExplicitlySharedDataPointer<CommandSequence>seq = it.next().value();
                        connect(p.get(), SIGNAL(rxResponse(std::weak_ptr<DevicePara>, quint32)), seq.data(), SLOT(onRxResponse(std::weak_ptr<DevicePara>, quint32)));
                        seq->insertPara(para);
                    }
                    emit enqueueSequenceQueue(r);
                }
                else{
                    emit enqueueCommandQueue(r);
                }
            }
            else{
                emit enqueueCommandQueue(r);
            }
        }
    }
}

void Framework::Device::construct(){
    /**
         * @brief worker for m_workerThread, create it dynamically otherwise moveToThread will
         * cause an exeption
         */
    m_workerThread = new QThread(this);
    m_workerThread->setObjectName("Device_" + QString::number(m_hiddenId));
    m_sm.moveToThread(m_workerThread);
    m_sm.getTaskTimer()->moveToThread(m_workerThread);
    m_autoDeleteTimer.setSingleShot(false);
    m_autoDeleteTimer.setInterval(eAutodeleteTimeout);
    m_winkTimer.setSingleShot(false);
    m_winkTimer.setInterval(750);
    m_rescanTimer->moveToThread(m_workerThread);
    //    connect(m_workerThread, &QThread::finished, &m_com, &QObject::deleteLater);//, Qt::QueuedConnection);
    connect(this, &Device::startWork, &m_sm, &RemoteClientSM::doWork);//, Qt::QueuedConnection);
    connect(this,&Device::stopWork, &m_sm, &RemoteClientSM::stopWork);//, Qt::QueuedConnection);
    connect(this, SIGNAL(udpReadData(RemoteAddressInfo, QByteArray)), &m_sm, SLOT(onHandleRx(RemoteAddressInfo, QByteArray)));//, Qt::QueuedConnection);
    connect(this, &Device::enqueueInitQueue, &m_sm, &RemoteClientSM::onEnqueueInitQueue);//, Qt::QueuedConnection);
    connect(this, &Device::enqueuePollingQueue, &m_sm, &RemoteClientSM::onEnqueuePollingQueue);//, Qt::QueuedConnection);
    connect(this, &Device::enqueueCommandQueue, &m_sm, &RemoteClientSM::onEnqueueCommandQueue);//, Qt::QueuedConnection);
    connect(this, &Device::enqueueSequenceQueue, &m_sm, &RemoteClientSM::onEnqueueSequenceQueue);//, Qt::QueuedConnection);
    connect(this, &Device::enqueueRetryQueue, &m_sm, &RemoteClientSM::onEnqueueRetryQueue);//, Qt::QueuedConnection);
    connect(this, &Device::dequeueRunningRequest, &m_sm, &RemoteClientSM::onDequeueRunningRequest);//, Qt::QueuedConnection);
    connect(this, &Device::removeParaFromQueue, &m_sm, &RemoteClientSM::onRemoveParaFromQueue);
    connect(this, SIGNAL(rxResponse()), this, SLOT(retriggerAutodeleteTimeout()));//, Qt::QueuedConnection);
    connect(&m_autoDeleteTimer, SIGNAL(timeout()), this, SLOT(autoDeleteTimeout()));//, Qt::QueuedConnection);
    connect(&m_winkTimer, SIGNAL(timeout()), this, SLOT(onWinkTimeout()));
    connect(m_sm.getTaskTimer(), &QTimer::timeout, this, &Device::Timeout);//, Qt::QueuedConnection);
    connect(this, SIGNAL(startRescanTimer()), m_rescanTimer, SLOT(start()));
    connect(this, SIGNAL(stopRescanTimer()), m_rescanTimer, SLOT(stop()));
    m_workerThread->start();
}



bool Framework::Device::IsOnline() const{
    return m_isOnline;
}

void Framework::Device::setOnline(bool val){
    if(val == m_isOnline) return;

    m_isOnline = val;
    if(val){
        startFSM();
        emit onlineChanged(val);
        if(!m_isScanDevice){
            if(m_isPolling || m_rescanTimer->isActive()){
                m_autoDeleteTimer.start();
            }
        }
    }
    else{
        m_autoDeleteTimer.stop();
        m_sm.allowWork(false);
        emit stopWork();
        emit onlineChanged(val);
    }
}

void Framework::Device::init(){
    if(isInit() == false){
        auto initList = GetInitParas();
        foreach(auto p, initList){
            Request *r = p->GetInitRequest();
            if(r){
                emit enqueueInitQueue(r);
            }
        }
        // fill polling queue
        auto pollList = GetPollParas();
        foreach(auto p, pollList){
            Request *r = p->GetPollRequest();
            if(r){
                 emit enqueuePollingQueue(r);
            }
        }
        setInit(true);
    }
}

void Framework::Device::retryRequest(Request *r){
    emit enqueueRetryQueue(r);
}

void Framework::Device::writeData(RemoteAddressInfo remoteAddress, const char *data, uint16_t length){
    length++;
    QByteArray sendData(data, length);

    emit send(remoteAddress, sendData, length);
}


void Framework::Device::onEthAddressResolved(ethAddress *eth){
    if(eth){
        setEthernetAddress(eth->getEthernetAddress());
        eth->deleteLater();
    }
}

void Framework::Device::onSequenceDone(uint32_t seqNr, Errors::Id err){
    if(m_runningSequences.contains(seqNr)){
        QExplicitlySharedDataPointer<CommandSequence> seq =  m_runningSequences.value(seqNr);
        disconnect(seq.data(), SIGNAL(sequenceDone(uint32_t, Errors::Id)), this, SLOT(onSequenceDone(uint32_t, Errors::Id)));
        m_runningSequences.remove(seqNr);
        if(err != Errors::Id::E_NO_ERROR){
            emit error("ERROR on " + getName() + " " + seq->getName() + " " + Errors(err).Name());
        }
        emit cmdSequenceDone(shared_from_this(), err);
        // if seq was network migration, delete device
    }
    startBackgroundUpdate();
}

void Framework::Device::startFSM(){
    if(isInit()){
        m_sm.allowWork(true);
        emit startWork();
    }
}

float Framework::Device::getMinEnergy() const{
    // calculate the minimal possible energy with the following formula
    return getMaxEnergy()/(pow(2.0f,getApertureMax() - getFStopRange()));
}

float Framework::Device::getActEnergy() const{
    return getMaxEnergy()/(pow(2.0f,getApertureMax() - getApertureAct()));
}

bool Framework::Device::isAvailable() const {
    // Available if initialized, not in standby and no alarm present
    int standby = getStandby();
    if(-1 != standby){
        return (0 == getStandby()) && isInit() && getAlarm() == ParaSelects::AlarmType::NONE;
    }
    return false;
}

float Framework::Device::getApertureMax() const{
    return MAX_APERTURE;
}

void Framework::Device::freeze(){
    
}

/**
 * @brief unfreeze restarts everything behind (device threads, communication, ...)
 */
void Framework::Device::unfreeze(){
    
}

