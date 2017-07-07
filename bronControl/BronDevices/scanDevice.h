#ifndef SCANDEVICE_H
#define SCANDEVICE_H

#include <QObject>
#include <QNetworkAddressEntry>
#include "device.h"
#include <QQueue>
#include <QByteArray>

namespace Framework{

class BronStudio;

class ScanDevice : public Device{
    Q_OBJECT
public:

    enum Ids{
         STUDIO_ADDRESS = PARAM_STUDIO_ADDRESS
    };
    Q_ENUM(Ids)

    enum eSettings{
        eRetries = 0,
        eRescanTime = 5000,             // ms
        eSelfDeletionTime = 30000
    };


    ScanDevice();
    virtual ~ScanDevice();
    virtual eDeviceTypes getType() const override;
    /**
     * @brief GetAllParas get a list with all parameters
     * @return
     */
    virtual QList<std::shared_ptr<DevicePara> > GetAllParas() const override;

    /**
     * @brief GetInitParas Parameters, need to read out during device init
     * @return
     */
    virtual QList<std::shared_ptr<DevicePara> > GetInitParas()const override;

    /**
     * @brief GetPollParas Parameters, needed to be called cyclic
     * @return
     */
    virtual QList<std::shared_ptr<DevicePara> > GetPollParas()const override;

    /**
     * @brief isInit returns the init state
     * @return
     */
    virtual bool isInit() const override;

    /**
     * @brief setInit sets the init state
     * @param val
     */
    virtual void setInit(bool val) override;

    virtual QString getName() const override;

    /**
     * @brief GetParaLocal
     * @param id
     * @return local parameter without communication
     */
    virtual std::shared_ptr<DevicePara> getPara(int id )const override;

    virtual int getStudioAddress() const override;

    void setNetworkEntry(const QNetworkAddressEntry e);

    QNetworkAddressEntry getNetworkEntry() const;

    void setStudio(BronStudio *studio);

    BronStudio *getStudio() const;
    /**
     * @brief isForStudio check if this scan device belongs to a studio
     * @param studio if studio == nullptr, returns in any case true
     * @return
     */
    bool isForStudio(BronStudio *studio = nullptr);

    virtual int iDStudioAddress() const override;
    virtual const QString IdString(int id) const override;

public slots:
    void rescanTimeout();
    void disableSelfDeletion(){
        m_selfDeleteTimer.stop();
    }

    void selfDeletionTimeot();

signals:

    /**
     * @brief deleteDevice called if no response was received within 30 seconds
     */
    void deleteDevice(std::weak_ptr<Device>);


private:
    bool m_isInitialized; /**< init indicator */
    QMap<int, std::shared_ptr<DevicePara> > m_remoteParas; /**< Vector with deviceParameters */
    std::shared_ptr<DefaultPara> m_defaultPara;
//    DevicePara *m_pDefaultPara;
    QTimer m_selfDeleteTimer;
    QNetworkAddressEntry m_networkEntry;
    QList<QHostAddress> m_respondingDevices;
    QQueue<QList<QHostAddress> > m_fir; // used to detect device changes
    BronStudio *m_studio;
    void initParas();
    void makeConnections();
};

inline Device::eDeviceTypes ScanDevice::getType() const{
    return Device::eDeviceTypes::eScan;
}

inline bool ScanDevice::isInit() const{
    return m_isInitialized;
}

inline void ScanDevice::setInit(bool val){
    m_isInitialized = val;
}

inline std::shared_ptr<DevicePara> ScanDevice::getPara(int id) const{
    return m_remoteParas.value(id, m_defaultPara);      // dont give nullptr in case of error otherwise crash will haplen, create a dummy device that logs the invalid id
}

inline int ScanDevice::getStudioAddress() const{
    return getPara(STUDIO_ADDRESS)->data().toInt();
}

inline void ScanDevice::setNetworkEntry(const QNetworkAddressEntry e){
    m_networkEntry = e;
}

inline QNetworkAddressEntry ScanDevice::getNetworkEntry() const{
    return m_networkEntry;
}

inline void ScanDevice::setStudio(BronStudio *studio){
    m_studio = studio;
}

inline BronStudio *ScanDevice::getStudio() const{
    return m_studio;
}

inline void ScanDevice::selfDeletionTimeot(){
    emit deleteDevice(shared_from_this());
//    emit deleteDevice(std::weak_ptr<ScanDevice>(this));
}

inline int ScanDevice::iDStudioAddress() const{
    return STUDIO_ADDRESS;
}

}

#endif // SCANDEVICE_H
