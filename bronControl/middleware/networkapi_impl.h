/**
  * @brief qtnetworkintegration holds the real communication socket and list
  * of all hosts to communicate with
  */

#ifndef NETWORKAPI_IMPL_H
#define NETWORKAPI_IMPL_H

#include <stdint.h>
#include <QObject>
#include <QUdpSocket>
#include <QMultiMap>
#include <QMutex>
#include <QThread>
#include <QMutexLocker>
#include "remoteaddressinfo.h"
#include "networkapi.h"


using namespace Framework;

class NetworkAPIImpl : public NetworkAPI{

    public:
        explicit NetworkAPIImpl(QObject *parent, uint16_t localPort);
        virtual ~NetworkAPIImpl();

    // functions implemented from NetworkAPI interface

    virtual Error Init() override;

    /**
     * @brief StartTimer gets a timer for timeout checks on the communication
     * @param timeoutInMs desired timeout in ms
     * @param timerCB function called at timeout
     * @return pointer to the timer
     */
    virtual TimerAPI * StartTimer(uint32_t timeoutInMs, TimerAPI::timerCB timerCB, Request* ctx) override;

    /**
     * @brief StopAndReleaseTimer stops and releases the timer
     * @param timer
     */
    virtual void StopAndReleaseTimer(TimerAPI *timer) override;

//    /**
//     * @brief CreateConnection creates and open an udp connection on given port
//     * @param localPort port to open
//     * @return pointer of connection or nullptr in case of failure
//     */
//    static QSharedPointer<NetworkAPI> CreateConnection(int localPort,  std::shared_ptr<Device> dev);



    /**
     * @brief ReleaseConnection releases the connection
     * @param network, the connection to realease
     */
    virtual void ReleaseConnection(std::weak_ptr<Device> dev) override;

    /**
     * @brief append insert a new known host (reply was received) to the system
     * @param host address of the host to communicate with
     * @param udp wrapper for the client to get access to the host
     */
    virtual void append(QHostAddress host, std::weak_ptr<Device> d) override;

    /**
     * @brief remove removes a known host from the system
     * @param udp wrapper which was
     * @return
     */
    virtual bool remove(std::weak_ptr<Device> d) override;


        /**
         * @brief setLocalPort sets the local port, normally not needed for udp communication, due to an
         * bug in Siros devices, there is a need to use a special port
         * @param port to use
         */
        void setLocalPort(uint16_t port){
            m_localPort = port;
        }

        /**
         * @brief ReadData from the udp socket
         */
        virtual void ReadData() override;
        /**
         * @brief Send writes data to the udp socket, in addition to this, the checksum is added at the last byte, calculated with
         * crc8_messagecalc.
         * @param remoteAddress address to send
         * @param data without calculated checksum
         * @param length of the data (will be expanded by one because of the checksum)
         */
        virtual void onSend(RemoteAddressInfo remoteAddress, QByteArray sndData, uint16_t length) override;

    private:
        QMultiMap<QHostAddress, std::weak_ptr<Device> >m_hostList;
        QUdpSocket m_socket;
        uint16_t m_localPort;
        QMutex m_mutex;
};

/**
 * @brief operator < compares two QHostAddresses (as uint32_t), used in QMap to sort addresses
 * @param a1 QHostAddress 1
 * @param a2 QHostAddress 2
 * @return true if a1 < a2
 */
bool operator<(const QHostAddress &a1, const QHostAddress &a2);





#endif // NETWORKAPI_IMPL_H
