/**
  * @abstract Device Finder is used to search and instantiate the devices found on any
  * interfaces (IPV4 of the executing host)
  * Therefore, a name request ist sent by a so called bcast device (only name request) with a local broadcast. If there is any response
  * to this request, in the receiving function (qtNetworkIntegration::ReadData()), is any
  * response to a unkown device routet back to the device finder (be careful, the broadcast request causes
  * also an valid data package) for the executing maching, this machine is by default also in the broadcast
  * range. The response telegrams are used to build the new devices.
  * For every unknown response, a bcast device is build. These bcast devices try to identify the device
  * type (read fStopRange, ...). If a device is identified, a new specialized Device is created with this
  * data. These devices are added to appropriate studio. With these method, we found all studios
  * on all network interfaces
  */

#ifndef DEVICEFINDER_H
#define DEVICEFINDER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include "device.h"
#include "scanDevice.h"
#include "devicescoro.h"
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QList>
#include "creatorDevice.h"

namespace Framework{

class DeviceFinder : public QObject{
    Q_OBJECT
public:

    explicit DeviceFinder();
    virtual ~DeviceFinder();

//    /**
//     * @brief gotResponse check if there was a response to a broadcasting device. If there is no response after
//     * a couple of seconds, the broadcasting (bcast device) deletes itself
//     * @param sender address of host
//     * @param senderPort port of host
//     * @return true if there was a response, false if not
//     */
//    bool gotResponse(QHostAddress sender, quint16 senderPort);

    /**
     * @brief checkResponse in case of an not known device responds to a request, this function
     * creates the scanningDevices
     * @param sender
     * @param senderPort
     * @param interfaceIx interface used
     */
    void checkResponse(QHostAddress sender, quint16 senderPort, uint interfaceIx);

//    /**
//     * @brief appendCreatorDevice after a response to a broadcast is received, an so called creater device is built.
//     * these device is added with this function to the system.
//     * @param dev
//     */
//    void appendCreatorDevice(std::weak_ptr<CreatorDevice> dev);

    /**
     * @brief removeCreatorDevice if the creator device has read all needed information, a specialized device is
     * built and the bcast device is removed.
     * @param dev
     */
    void removeCreatorDevice(std::weak_ptr<CreatorDevice> dev);

    /**
     * @brief startDiscoverDevices starts a device search on all system inferfaces
     * @param toFind
     */
    void startDiscoverDevices(BronStudio *studio, QList<QNetworkAddressEntry> scanAddresses);

    /**
     * @brief stopDiscoverDevices stop the discovery process
     * @param studio studio on which the discovery should stop
     * @param stopAll if stopAll is set, stop independent of the studio
     */
    void stopDiscoverDevices(BronStudio *studio, bool stopAll = true);

public slots:

    /**
     * @brief removeAnonymusDevice removes a "scan device" (maybe no response)
     * @param dev
     */
    void removeAnonymusDevice(std::weak_ptr<Device> dev);

    /**
     * @brief deviceIdentified this function removes an creator device an build the specialized one. If a
     * specialized is build, it is registered in the StudioMgr "StudioMgr::newIdentifiedDevice(std::weak_ptr<Device> )"
     */
    void deviceIdentified(std::weak_ptr<Device> d);

    void onCreateLamp(DeviceScoro *d, int lampNr);
    void onDestroyLamp(DeviceScoro *d, int lampNr);

private:
    QMutex m_mutex;
    // m_broadcastDevices and m_creating devices are living here
    QList<std::shared_ptr<ScanDevice> > m_broadcastDevices; /**< list of scanning (broadcasting) devices */
    QList<std::shared_ptr<CreatorDevice> >m_creatingDevices; /**< list of creating devices(used to identify a special type */
};


}

bool operator ==(const QNetworkInterface &a, const QNetworkInterface &b);


#endif // DEVICESIRROS_H
