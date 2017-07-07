#ifndef REMOTEADDRESSINFO_H
#define REMOTEADDRESSINFO_H

#include <stdint.h>
#include <QObject>

struct DeviceAddress {
    uint32_t   address;
    uint16_t    port;
};


class RemoteAddressInfo{
public:
    RemoteAddressInfo()
        : m_remoteAddress{0L,0}{}
    RemoteAddressInfo(uint32_t addr, uint16_t port)
        : m_remoteAddress{addr, port}{}
    RemoteAddressInfo(const RemoteAddressInfo &ra)
        : m_remoteAddress(ra.m_remoteAddress){}

    DeviceAddress remoteAddress() const{
        return m_remoteAddress;
    }

private:
    DeviceAddress m_remoteAddress;
};

Q_DECLARE_METATYPE(RemoteAddressInfo);

#endif // REMOTEADDRESSINFO_H
