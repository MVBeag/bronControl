#ifndef SEQUENCEGENERATOR_H
#define SEQUENCEGENERATOR_H

#include <QMutex>
#include <QMutexLocker>
#include <stdint.h>


class SequenceGenerator
{
public:
    SequenceGenerator();
//    SequenceGenerator& operator++();
    uint32_t incSequ();
//    operator uint32_t() const;
private:
    mutable QMutex m_mutex{};
    uint32_t m_sequence{0L};
//    QAtomicInteger<uint32_t> m_sequence{0L};
};

#endif // SEQUENCEGENERATOR_H
