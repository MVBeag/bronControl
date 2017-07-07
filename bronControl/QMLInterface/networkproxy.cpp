#include "networkproxy.h"
#include <QDebug>

using namespace QMLInterface;


QMLInterface::NetworkProxy::NetworkProxy(std::weak_ptr<QNetworkInterface> interface)
    : BronItemAPI()
    , m_interface(interface){
}

QMLInterface::NetworkProxy::~NetworkProxy(){
}

QMLInterface::NetworkProxy::NetworkProxy(const NetworkProxy &p)
    : BronItemAPI()
    , m_interface(p.m_interface){
}

NetworkProxy &QMLInterface::NetworkProxy::operator=(const NetworkProxy &p){
    m_interface = p.m_interface;
    return *this;
}

QVariant QMLInterface::NetworkProxy::getData(int role) const{
    if(m_interface){
        switch(role){
        case Qt::DisplayRole:
        case NAME: return m_interface->humanReadableName();
        case IDENTIFIER: return m_interface->index();
        default: return QVariant();
        }
    }else{
        return QVariant();
    }
}

bool QMLInterface::NetworkProxy::setData(int role, const QVariant &v){
    Q_UNUSED(v)
    Q_UNUSED(role)
    return false;
}

QHash<int, QByteArray> QMLInterface::NetworkProxy::roleNames() const{
    static const QHash<int, QByteArray> roles(std::initializer_list<std::pair<int, QByteArray> >({
                                            {NAME, "name"},
                                            {IDENTIFIER, "id"}}));
    return roles;
}

