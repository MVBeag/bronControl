#include <QMetaEnum>
#include "devicepara.h"
#include "device.h"
#include "request.h"


//const QString DevicePara::className    = "DevicePara";
//const quint32 DevicePara::classVersion = 1;

using namespace Framework;

bool Framework::DevicePara::setData(const QVariant &val, Errors::Id error, bool doNotCheckValue){
    // signal rxResponse in either case, needed if sequences are running
    emit rxResponse(shared_from_this(), static_cast<quint32>(error));


//        qDebug() << "rx unconditional: "
//                 << GetElder().getHostAddress()
//                 << " " << GetElder().getName()
//                 << "::" << m_elder.IdString(m_id)
//                 << "::" << m_data.toString()
//                 << " val " << val.toString();

    // first timer handling
    if(error == Errors::Id::E_NO_ERROR){
        m_triggerResponse.getRuntimeMs();

        if(m_preReadFct(m_elder, val)){
            QVariant v = val;
            if(this->m_type == Types::eInt){
                v = QString::number(val.toInt()/m_scaling, 'f', 3).toFloat();
            }

            if((m_data == v) && (m_state == State::eDefined) && (doNotCheckValue == false)){
                return false;
            }

            m_data = v;
            m_memory = m_data.toString().toUtf8();
            m_triggerResponse.setResponseData(triggerReason::eNewVal, m_data);

            qDebug() << "rx: "
                     << GetElder().getHostAddress()
                     << " " << GetElder().getName()
                     << "::" << m_elder.IdString(m_id)
                     << "::" << m_data.toString()
                     << " val " << val.toString();

            // handle dependencies
            m_depFct(m_elder);
            emit deviceParaDataChanged(shared_from_this());
            if(m_state == State::eUndefined){
                m_state = State::eDefined;
            }
            return true;
        }
    }
//    else{
//        qDebug() << m_elder.getName()
//                 << " setData with error "
//                 << m_elder.IdString(m_id)
//                 << " "
//                 << Errors(error).Name();
//    }
    return false;
}



QVariant Framework::DevicePara::scaledDataForRemote() const{
    return m_triggerResponse.getResponseData().toFloat() * m_scaling;
}

const char *Framework::DevicePara::stringBufferForRemote() const{
    return m_triggerResponse.getResponseString();
}

int Framework::DevicePara::DevicePara::stringBufferSizeForRemote() const{
    return m_triggerResponse.getResponseBufferSize();
}

int Framework::DevicePara::Id(){
    return m_id;
}


Device &Framework::DevicePara::GetElder() const{
    return m_elder;
}

DevicePara::Types Framework::DevicePara::Type() const{
    return m_type;
}

DevicePara::Settings Framework::DevicePara::GetSettings() const{
    return m_settings;
}

bool Framework::DevicePara::operator ==(const DevicePara &r) const{
    if(m_elder == r.m_elder){
        if(m_id == r.m_id){
            return true;
        }
    }
    return false;
}



DevicePara &Framework::DevicePara::operator=(const DevicePara &arg){
    // assign only items, belonging to data, all other things are like given in constructor
    m_data = arg.m_data;
    m_triggerResponse = arg.m_triggerResponse;
    m_memory = arg.m_memory;
    m_state = arg.m_state;
    return *this;
}

// remote accessors
Framework::DevicePara::DevicePara(Device* parent, int id, const QVariant value, DevicePara::Types type, float scale, DevicePara::Settings s, preReadFct fct, depFunction dfct)
    : QObject()
    , m_settings(s)
    , m_id(id)
    , m_data(value)
    , m_memory()
    , m_type(type)
    , m_elder(*parent)
    , m_scaling(scale)
    , m_preReadFct(fct)
    , m_depFct(dfct)
    , m_state(DevicePara::State::eUndefined)
    , m_triggerResponse(*this){
    connect(this, SIGNAL(deviceParaDataChanged(std::weak_ptr<DevicePara>)), &m_elder, SLOT(onDeviceParaDataChanged(std::weak_ptr<DevicePara>)));
}

Framework::DevicePara::DevicePara(const DevicePara &p)
    : QObject()
    , m_settings(p.m_settings)
    , m_data(p.m_data)
    , m_memory(p.m_memory)
    , m_type(p.m_type)
    , m_elder(p.m_elder)
    , m_scaling(p.m_scaling)
    , m_preReadFct(p.m_preReadFct)
    , m_depFct(p.m_depFct)
    , m_triggerResponse(*this){
    connect(this, SIGNAL(deviceParaDataChanged(std::weak_ptr<DevicePara>)), &m_elder, SLOT(onDeviceParaDataChanged(std::weak_ptr<DevicePara>)));
}




void Framework::DevicePara::getRemote(){
    setDataForRemote(triggerReason::eDoNothing);
    m_elder.GetPara(shared_from_this(),m_type);
}

bool Framework::DevicePara::setRemote(const QVariant value, QList<std::function<bool()> >postCmd){
    qDebug() << "setRemote: "
             << m_elder.getName()
             << " @"
             << m_elder.getHostAddress().toString()
             << " id: " << m_elder.IdString(m_id)
             << " " << value;
    m_elder.storePara(this);
    setDataForRemote(triggerReason::eNewTarget, value);
    return m_elder.SetPara(shared_from_this(), m_type, postCmd);
}

bool Framework::DevicePara::setRemote(){
    setDataForRemote(triggerReason::eDoNothing);
    return m_elder.SetPara(shared_from_this(), m_type);
}


Request *Framework::DevicePara::GetInitRequest() {
    if(m_settings.testFlag(eInit)){
        return new Request(m_type == DevicePara::Types::eInt ? Request::eTypes::eGetInt :
                                                               Request::eTypes::eGetString, shared_from_this());
    }
    else{
        return nullptr;
    }
}

Request *Framework::DevicePara::GetPollRequest(){
    if(m_settings.testFlag(ePoll)){
        return new Request(m_type == DevicePara::Types::eInt ? Request::eTypes::eGetInt :
                                                               Request::eTypes::eGetString, shared_from_this());
    }
    else{
        return nullptr;
    }
}


QVariant Framework::DefaultPara::data() const{
//    qDebug() << "devicePara Id or Role [" << m_dev->IdString(m_id) << "] undefined data --> check deviceParas";
    return QVariant();
}

bool Framework::DefaultPara::setRemote(const QVariant value, QList<std::function<bool()> > postCmd){
    Q_UNUSED(postCmd)
    qDebug() << "devicePara Id or Role [" << m_elder.IdString(m_id) << "] undefined setRemote -> check deviceParas" << value;
    return false;
}

int Framework::DefaultPara::id() const{
    return m_id;
}

void Framework::DefaultPara::setId(int id){
    m_id = id;
}

Framework::DevicePara::TriggerResponseData &Framework::DevicePara::TriggerResponseData::operator =(const DevicePara::TriggerResponseData &r){
    m_targetReached = r.m_targetReached;
    duration = r.duration;
    responseData = r.responseData;
    m_memory = r.m_memory;
    m_cmdTriggered = r.m_cmdTriggered;
    // elder stays the same
    return *this;
}

const char *Framework::DevicePara::TriggerResponseData::getResponseString() const{
    return m_memory.constData();
}

int Framework::DevicePara::TriggerResponseData::getResponseBufferSize() const{
    return m_memory.size();
}

void Framework::DevicePara::TriggerResponseData::setResponseData(triggerReason commandTriggered, const QVariant value){
    // if we got new targetval, write it in every case
    if(commandTriggered == triggerReason::eNewTarget){
        responseData = value;
        if(m_elder.Type() == Types::eString){
            m_memory = responseData.toString().toUtf8();
        }
        m_targetReached = false;
        m_cmdTriggered = commandTriggered;
    }
    // in every other case, it depends on the internal state of m_cmdTriggered
    else{
        if(commandTriggered == triggerReason::eDoNothing){
            // do nothing, because we want to read the data remotly
        }
        else if(commandTriggered == triggerReason::eNewVal){
            // check if target is reached, if not keep old target
            if(m_elder.Type() == Types::eInt){
                float edata = m_elder.data().toFloat();
                float deviation = edata * 0.001f;
                float val = value.toFloat();
                if((edata + deviation) > val && (edata - deviation) < val){
                    responseData = value;
                    m_cmdTriggered = triggerReason::eDoNothing;
                    m_targetReached = true;
                }
            }
            else if(m_elder.Type() == Types::eString){
                if(value == responseData){
                    m_cmdTriggered = triggerReason::eDoNothing;
                    m_targetReached = true;
                }
            }
        }
    }
}

bool Framework::operator ==(const std::weak_ptr<DevicePara> &a, const std::weak_ptr<DevicePara> &b){
    if(auto pa = a.lock()){
        if(auto pb = b.lock()){
            return *(pa.get()) == *(pb.get());
        }
    }
    return false;
}
