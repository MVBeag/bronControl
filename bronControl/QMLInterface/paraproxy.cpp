#include "paraproxy.h"
#include <QQmlEngine>
#include <QDebug>

using namespace QMLInterface;

void QMLInterface::ParaProxy::makeConnections(){
    if(auto p = m_p.lock()){
        connect(p.get(), SIGNAL(deviceParaDataChanged(std::weak_ptr<DevicePara>)), this, SLOT(onDeviceParaDataChanged(std::weak_ptr<DevicePara>)));
    }
}


QMLInterface::ParaProxy::ParaProxy(std::weak_ptr<DevicePara> para)
    : BronItemAPI()
    , m_p(para)
    , m_role(-1){
    makeConnections();
}

QMLInterface::ParaProxy::~ParaProxy(){
}

QMLInterface::ParaProxy::ParaProxy(const ParaProxy &p)
    : BronItemAPI()
    , m_p(p.m_p)
    , m_role(p.m_role){
    makeConnections();
}

ParaProxy &QMLInterface::ParaProxy::operator=(const ParaProxy &p){
    m_p = p.m_p;
    makeConnections();
    return *this;
}

QVariant QMLInterface::ParaProxy::getData(int role) const{
    if(auto p = m_p.lock()){
        switch(role){
        case VALUE: return p->data();
        case IDENTIFIER: return QVariant::fromValue(p->Id());
        case DEVICE_ROLE: return m_role;
        default: return QVariant();
        }
    }else{
        return QVariant();
    }
}

bool QMLInterface::ParaProxy::setData(int role, const QVariant &v){
    if(auto p = m_p.lock()){
        switch(role){
        case VALUE: return p->setRemote(v);
        case IDENTIFIER:
        case DEVICE_ROLE:
        default: return false;
        }
    }
    return false;
}

QHash<int, QByteArray> QMLInterface::ParaProxy::roleNames() const{
    static const QHash<int, QByteArray> roles(std::initializer_list<std::pair<int, QByteArray> >({
                                            {VALUE, "value"},
                                            {IDENTIFIER, "id"},
                                            {DEVICE_ROLE, "devRole"}}));
    return roles;
}

