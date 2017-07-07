#include "errors.h"
#include <QMetaEnum>
#include <QMetaObject>

using namespace Framework;

Framework::Errors::Errors()
    : QObject()
    , m_id(Errors::Id::E_NO_ERROR){
}

Framework::Errors::Errors(Errors::Id id)
    : QObject()
    , m_id(id){}

Framework::Errors::Errors(const Errors &i)
    : QObject()
    , m_id(i.m_id){}


Framework::Errors::Errors(uint32_t id)
    : QObject()
    , m_id(static_cast<Errors::Id>(id)){
}


Errors::Id Framework::Errors::GetId() const{
    return m_id;
}

QString Framework::Errors::Name(){
    QMetaEnum metaEnum = QMetaEnum::fromType<Id>();
    return QString(metaEnum.valueToKey((uint32_t)m_id));
}

Framework::Errors::operator uint32_t() const{
    return static_cast<uint32_t>(m_id);
}

