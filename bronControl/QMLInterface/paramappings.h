#ifndef PARAMAPPINGS_H
#define PARAMAPPINGS_H

#include <QObject>
#include <QHash>
#include "device.h"

using namespace Framework;

class ParaMappings : public QObject
{
    Q_OBJECT
public:
    explicit ParaMappings(QObject *parent = 0);
    static const QHash<int,int> &getDeviceMappings(const std::weak_ptr<Device> );
private:
};

#endif // PARAMAPPINGS_H
