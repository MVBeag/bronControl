/** @abstract NetworkAPI is a virtual interface to the real interface
 * just implement this on the target to make it work
 */
#ifndef NETWORKAPI_H
#define NETWORKAPI_H

#include <QObject>
#include <QHostAddress>
#include <stdint.h>
#include <memory>
#include "timerapi.h"
#include "remoteaddressinfo.h"

namespace Framework{
    class Device;
    class Request;
}


using namespace Framework;


class NetworkAPI : public QObject{
    Q_OBJECT
public:
    enum class Error{
        eNoError,
        eError
    };


    NetworkAPI(QObject *parent)
        : QObject(parent){}
    virtual ~NetworkAPI(){}

    /**
     * @brief Init initializes the network api
     * @return eNoError or eError depending on internal state
     */
    virtual Error Init() = 0;


    /**
     * @brief StartTimer gets a timer for timeout checks on the communication
     * @param timeoutInMs desired timeout in ms
     * @param timerCB function called at timeout
     * @return pointer to the timer
     */
    virtual TimerAPI * StartTimer(uint32_t timeoutInMs, TimerAPI::timerCB timerCB, Request* ctx) = 0;

    /**
     * @brief StopAndReleaseTimer stops and releases the timer
     * @param timer
     */
    virtual void StopAndReleaseTimer(TimerAPI *timer) = 0;

    /**
     * @brief Send writes data to the udp socket, in addition to this, the checksum is added at the last byte, calculated with
     * crc8_messagecalc.
     * @param remoteAddress address to send
     * @param data without calculated checksum
     * @param length of the data (will be expanded by one because of the checksum)
     */
//    virtual void Send(RemoteAddressInfo remoteAddress, const char* data, uint16_t length) = 0;


    /**
     * @brief ReleaseConnection releases the connection
     * @param network, the connection to realease
     */
    virtual void ReleaseConnection(std::weak_ptr<Device> dev) = 0;

    /**
     * @brief append insert a new known host (reply was received) to the system
     * @param host address of the host to communicate with
     * @param udp wrapper for the client to get access to the host
     */
    virtual void append(QHostAddress host, std::weak_ptr<Device> d) = 0;

    /**
     * @brief remove removes a known host from the system
     * @param udp wrapper which was
     * @return
     */
    virtual bool remove(std::weak_ptr<Device> d) = 0;


public slots:

    /**
     * @brief ReadData from the udp socket
     */
    virtual void ReadData() = 0;
    /**
     * @brief Send writes data to the udp socket, in addition to this, the checksum is added at the last byte, calculated with
     * crc8_messagecalc.
     * @param remoteAddress address to send
     * @param data without calculated checksum
     * @param length of the data (will be expanded by one because of the checksum)
     */
    virtual void onSend(RemoteAddressInfo remoteAddress, QByteArray sndData, uint16_t length)  = 0;

signals:
        void stop();
};

#endif // NETWORKAPI_H
