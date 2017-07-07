#include "bronitemapi.h"


QMLInterface::BronItemAPI::BronItemAPI()
    : QObject()
    , m_modelIx(-1)
    , m_lampIx(-1){}

QMLInterface::BronItemAPI::BronItemAPI(const QMLInterface::BronItemAPI &api)
    : QObject()
    , std::enable_shared_from_this<BronItemAPI>(api)
    , m_modelIx(api.m_modelIx)
    , m_lampIx(api.m_lampIx){}

int QMLInterface::BronItemAPI::changeIndex(int ix){
    int ret = m_modelIx;
    m_modelIx = ix;
    return ret;
}

int QMLInterface::BronItemAPI::changeLampIx(int ix){
    int ret = m_lampIx;
    m_lampIx = ix;
    return ret;
}

int QMLInterface::BronItemAPI::childNumber() const{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(std::const_pointer_cast<BronItemAPI>(shared_from_this()));

    return 0;
}

void QMLInterface::BronItemAPI::insertChild(int position, std::shared_ptr<BronItemAPI> child){
    m_childItems.insert(position, child);
}

int QMLInterface::BronItemAPI::row() const{
    if(m_parentItem){
        return m_parentItem->childPos(std::const_pointer_cast<BronItemAPI>(shared_from_this()));
    }
    else{
        return 0;
    }
}

int QMLInterface::BronItemAPI::childPos(const std::weak_ptr<QMLInterface::BronItemAPI> item){
    return m_childItems.indexOf(item.lock());
}
