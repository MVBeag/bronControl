/**
 * @author      Jörg Roth; Bron Elektronik AG
 * @date        12.07.2016
 * @version     0.0.90
 * @abstract base class for all bron devices, this class handles the communication with the remote device.
 * A device have parameters class DevicePara and Serivce (registered runtime functions). A DeviceParameter
 * normally has no dependencies. If functions use more than one parameter, a service is created. We use services
 * instead of impelemnted base class functions, because we want a small API.
 *
 * The communication is driven from paramters, if a setRemote/getRemote Para was successful, the value is stored
 * in the parameter, this causes a valuesChanged signal of the parameter. Within these mechanism, you can
 * use automatic data binding in qt/QML applications
 */


#ifndef GENLAMP_H
#define GENLAMP_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QString>
#include <QPointer>
#include "device.h"
#include "devicepara.h"
#include "devicescoro.h"
#include "errors.h"
#include "request.h"
#include "paraSelectables.h"

namespace Framework{
/**
 * @brief The Device class is the abstract base class for accessing remote device functions. It acts as a kind of proxy. Beside
 * this, it holds all the parameters, the device has. A Parameter (DevicePara has no dependency to other parameters. If there are
 * parameters, the device has a function (here called service) to do the work.
 */
class GenLamp: public Device{
    Q_OBJECT

public:

    enum Ids{
        L1_APERTURE              = scoroParam::SP_L1_APERTURE,
        L2_APERTURE              = scoroParam::SP_L2_APERTURE,
        L3_APERTURE              = scoroParam::SP_L3_APERTURE,
        L1_SW                    = scoroParam::SP_L1_SW, 				  /* -p get/set; -c tells whether the lamp is switched on (1) or off (0) */
        L2_SW                    = scoroParam::SP_L2_SW,
        L3_SW                    = scoroParam::SP_L3_SW,
        L1_ADDR                  = scoroParam::SP_L1_ADDR,				  /* -p get/set */
        L2_ADDR                  = scoroParam::SP_L2_ADDR,
        L3_ADDR                  = scoroParam::SP_L3_ADDR,
    };
    Q_ENUM(Ids)

    explicit GenLamp(int lampNr, QPointer<DeviceScoro> elder, bool demo = false);
    explicit GenLamp(const GenLamp &l);
    explicit GenLamp();
    virtual ~GenLamp();


    virtual eDeviceTypes getType() const override;
    virtual int getLampAddress() const override;
    virtual bool setLampAddress(int val) override;
    virtual float getApertureAct() const override;
    virtual bool setActApertureEnergy(float val) override;
    virtual int getStudioAddress() const override;
    virtual float getFStopRange() const override;
    virtual float getApertureMax() const override;
    virtual QString getSSID() const override;
    virtual QString getPWD() const override;
    virtual void activateWink() override;
    virtual void deactivateWink() override;
    virtual void changeWink(bool ena) override;

    /**
     * @brief setName setter for device Name emits NameChanged event
     * @param name
     */
    virtual bool setName(const QString &name) override;

    /**
     * @brief Name getter
     * @return
     */
    virtual QString getName() const override;
    virtual QString getRemoteName() override;

    /**
     * @brief Para returns a registered Para by id, if id not found, nullptr is returnd
     * @param id of the parameter you want
     * @return address of parameter in case of success, nullptr else
     */
    virtual std::shared_ptr<DevicePara> getPara(int id) const override;

    /**
     * @brief GetAllParas
     * @return a vecor containing all paras
     */
    virtual QList<std::shared_ptr<DevicePara> > GetAllParas() const override;

    /**
     * @brief GetInitParas
     * @return a vector containing the init paras, read first
     */
    virtual QList<std::shared_ptr<DevicePara> > GetInitParas()const override;

    /**
     * @brief GetPollParas
     * @return a vector containing the polling paras
     */
    virtual QList<std::shared_ptr<DevicePara> > GetPollParas() const override;

    /**
     * @brief isInit checks if device was initialized
     * @return
     */
    virtual bool isInit() const override;

    /**
     * @brief setInit mark device as initialized
     * @param val
     */
    virtual void setInit(bool val) override;

    /**
     * @brief isInStandby check if remote device is in standby, we interpret switch off as standby
     * @return
     */
    virtual int getStandby() const override;
    virtual bool setStandby(int val) override;

    /**
     * @brief getEthernetAddress
     * @return return the remote ethernet address, if not read (getIsDeviceAwareness() = false)
     * returns an empty string
     */
    QString getEthernetAddress() const;

    bool isScoroInStandby() const;

    virtual QString className() const override;

    int getNr() const;

    virtual int iDLampAddress() const override;
    virtual int getDeviceType() const override;
    virtual const QString IdString(int id) const override;

    virtual bool increasePower(float val) override;
    virtual bool decreasePower(float val) override;

    virtual void setDefaultParaId(int id) override;
    virtual int getChannel() const override;
    virtual float getMaxEnergy() const override;
    virtual void setPolling(bool val) override;
    virtual bool isPolling() const override;

    QPointer<DeviceScoro> getScoroDevice();

public slots:
    void onAddrChanged(QVariant val);
    void onApertureChanged(QVariant val);
    void onSwChanged(QVariant val);


private:
    bool m_isInit;
    int m_lampNr;
    QPointer<DeviceScoro> m_scoro;
    QMap<int, std::shared_ptr<DevicePara> > m_remoteParas; /**< Vector with deviceParameters */
    std::shared_ptr<DefaultPara> m_defaultPara;
    QString m_name;
    void initParas(bool demo);
};


inline Device::eDeviceTypes GenLamp::getType() const{
    return Device::eDeviceTypes::eGenLamp;
}

inline int GenLamp::getLampAddress() const{
    return getPara(L1_ADDR + m_lampNr)->data().toInt();
}

inline bool GenLamp::setLampAddress(int val){
    return getPara(L1_ADDR + m_lampNr)->setRemote(val);
}

inline float GenLamp::getApertureAct() const{
    return getPara(L1_APERTURE + m_lampNr)->data().toFloat();
}

inline bool GenLamp::setActApertureEnergy(float val){
    return getPara(L1_APERTURE + m_lampNr)->setRemote(val);
}

inline int GenLamp::getStudioAddress() const{
    if(m_scoro){
        return m_scoro->getStudioAddress();
    }
    return -1;
}

inline QString GenLamp::getSSID() const{
    if(m_scoro){
        return m_scoro->getSSID();
    }
    return "";
}

inline QString GenLamp::getPWD() const{
    if(m_scoro){
        return m_scoro->getPWD();
    }
    return "";
}

inline void GenLamp::activateWink(){
    if(m_scoro) m_scoro->activateWink();
}

inline void GenLamp::deactivateWink(){
    if(m_scoro) m_scoro->deactivateWink();
}

inline void GenLamp::changeWink(bool ena){
    if(m_scoro) m_scoro->changeWink(ena);
}

inline QString GenLamp::getRemoteName(){
    return getName();
}

inline std::shared_ptr<DevicePara> GenLamp::getPara(int id) const{
    return m_remoteParas.value(id, m_defaultPara);      // dont give nullptr in case of error otherwise crash will haplen, create a dummy device that logs the invalid id
}

inline QList<std::shared_ptr<DevicePara> > GenLamp::GetAllParas() const{
    return m_remoteParas.values();
}

inline bool GenLamp::isInit() const{
    return m_isInit;
}

inline void GenLamp::setInit(bool val){
    m_isInit = val;
}





inline QString GenLamp::className() const{
    return GenLamp::staticMetaObject.className();
}

inline int GenLamp::getNr() const{
    return m_lampNr;
}

inline int GenLamp::iDLampAddress() const{
    return L1_ADDR + m_lampNr;
}

inline int GenLamp::getDeviceType() const{
    return Device::eTypes::eGeniLamp;
}

inline void GenLamp::setDefaultParaId(int id){
    m_defaultPara->setId(id);
}

inline int GenLamp::getChannel() const{
    return m_lampNr;
}

inline void GenLamp::setPolling(bool){
    Device::setPolling(false);
    // do nothing, scoros are not polled
}

inline bool GenLamp::isPolling() const{
    return false;
}

inline void GenLamp::onAddrChanged(QVariant val){
    getPara(L1_ADDR + m_lampNr)->setData(val);

}

inline void GenLamp::onSwChanged(QVariant val){
    getPara(L1_SW + m_lampNr)->setData(val);
}

}

Q_DECLARE_METATYPE(Framework::GenLamp)

#endif // GENLAMP_H
