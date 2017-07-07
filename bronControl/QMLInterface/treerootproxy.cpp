#include "treerootproxy.h"
#include <QDebug>

using namespace QMLInterface;


QMLInterface::TreeRootProxy::TreeRootProxy()
    : BronItemAPI(){
}

QMLInterface::TreeRootProxy::~TreeRootProxy(){
}

QMLInterface::TreeRootProxy::TreeRootProxy(const TreeRootProxy &p)
    : BronItemAPI(){
    Q_UNUSED(p)
}

TreeRootProxy &QMLInterface::TreeRootProxy::operator=(const TreeRootProxy &p){
    Q_UNUSED(p)
    return *this;
}

QVariant QMLInterface::TreeRootProxy::getData(int role) const{
        switch(role){
        case NAME: return "root";
        default: return QVariant();
        }
}

bool QMLInterface::TreeRootProxy::setData(int role, const QVariant &v){
    Q_UNUSED(v)
    Q_UNUSED(role)
    return false;
}

QHash<int, QByteArray> QMLInterface::TreeRootProxy::roleNames() const{
    static const QHash<int, QByteArray> roles(std::initializer_list<std::pair<int, QByteArray> >({
                                            {NAME, "name"}}));
    return roles;
}

