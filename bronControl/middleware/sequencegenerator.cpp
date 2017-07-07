#include "sequencegenerator.h"

SequenceGenerator::SequenceGenerator(){
}

uint32_t SequenceGenerator::incSequ(){
    uint32_t ret;
    m_mutex.lock();
    m_sequence++;
    ret = m_sequence;
    m_mutex.unlock();
    return ret;
}

//SequenceGenerator& SequenceGenerator::operator++(){
//    QMutexLocker lock(&m_mutex);
//    m_sequence++;
//    return *this;
//}

//SequenceGenerator::operator uint32_t() const{
//    QMutexLocker lock(&m_mutex);
//    return m_sequence;
//}
