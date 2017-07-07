/**
  * @abstract this device identifies the real device on the other end of the
  * communication an initiates a correct instantiation
  */
#ifndef CREATORDEVICE_H
#define CREATORDEVICE_H

#include <QObject>
#include <QTimer>
#include <QNetworkAddressEntry>
#include "device.h"
#include "scanDevice.h"

namespace Framework{

class CreatorDevice : public Device{
    Q_OBJECT
public:

    enum Ids{
        DEVICE_DESC               = PARAM_DEVICE_DESC,
        STUDIO_ADDRESS            = PARAM_STUDIO_ADDRESS,
        DEVICE_NAME               = PARAM_DEVICE_NAME,
        MAX_ENERGY                = PARAM_WLAN_MAX_ENGERGY,
        F_STOP_RANGE              = PARAM_WLAN_F_STOP_RANGE,
        SSID_COMPACT              = PARAM_WLAN_LOGIN1_SSID,
        SSID_GENI                 = scoroParam::SP_NW_SSID,
        PWD_COMPACT               = PARAM_WLAN_LOGIN1_PWD,
        PWD_GENI                  = scoroParam::SP_NW_PWD,
        ENCR_COMPACT              = PARAM_WLAN_LOGIN1_ENC_TYPE,
    };
    Q_ENUM(Ids)

    CreatorDevice(uint interfaceIx);

    virtual ~CreatorDevice();

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
    virtual int iDStudioAddress() const override;

    virtual const QString IdString(int id) const override;

    virtual QString getRemoteName() override;


private:
    bool m_isInitialized; /**< init indicator */
    QMap<int, std::shared_ptr<DevicePara>> m_remoteParas; /**< Vector with deviceParameters */
    std::shared_ptr<DefaultPara> m_defaultPara;
//    DevicePara *m_pDefaultPara;
    void initParas();
    void makeConnections();
};

inline Device::eDeviceTypes CreatorDevice::getType() const{
    return Device::eDeviceTypes::eCreator;
}

inline bool CreatorDevice::isInit() const{
    return m_isInitialized;
}

inline void CreatorDevice::setInit(bool val){
    m_isInitialized = val;
}

inline std::shared_ptr<DevicePara> CreatorDevice::getPara(int id) const{
    return m_remoteParas.value(id, m_defaultPara);      // dont give nullptr in case of error otherwise crash will haplen, create a dummy device that logs the invalid id
}

inline int CreatorDevice::getStudioAddress() const{
    return getPara(STUDIO_ADDRESS)->data().toInt();
}

inline int CreatorDevice::iDStudioAddress() const{
    return STUDIO_ADDRESS;
}

inline QString CreatorDevice::getRemoteName(){
    return getPara(DEVICE_NAME)->data().toString();
}

}

#endif // CREATORDEVICE_H
