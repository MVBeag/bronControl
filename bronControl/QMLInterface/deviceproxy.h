#ifndef DEVICEPROXY_H
#define DEVICEPROXY_H

#include <QObject>
#include "device.h"
#include "paraproxy.h"
#include "bronitemapi.h"
#include "bronlistmodel.h"


namespace QMLInterface{

using namespace Framework;


/**
 * @brief The DeviceProxy class is the proxa for a Device. The patameters are delivered as BronListModel. The
 * BronItemAPI of thid BronListModel is implemented in ParaProxa the class
 */
class DeviceProxy : public BronItemAPI {
    Q_OBJECT
    friend class Framework::Device;        // give access to device
public:

    enum eRoles{
        ONLINE =Qt::UserRole + 1, //257
        TYPE,
        PARAS,
        LOCAL_COGNI,
        MIN_APERTURE,
        MAX_APERTURE,                             /**< max possible output power in W */
        FULL_MODEL_NAME,
        UPDATING,
        INCPOWER,
        DECPOWER,
        LAMPS,
        LAMP_CHANNEL,
        ACT_ENERGY,
        STUDIO_ADDRESS,
        LAMP_ADDRESS,
        COGNI_COL_ENA,
        COGNI_COL_ACT,
        COGNI_COL,
        DELAY,
        MODLIGHT,
        MODLIGHTMODE,
//        DEVICE_DESC,
//        NUM_OF_LAMPS,
        NAME,
        SW_VERSION,
        ALARM,
        MAX_ENERGY,
        F_STOP_RANGE,
        F_STOP_MAX,
        STANDBY,
        SPEED,
        AUDIO,
        SEQUENCE,
        SYN_SEQUENCE,
        PAUSE_TIME,
        FLASH_TIME_SETTING,
        FLASH_DURATION,
        SLOW_CHARGE,
        TEST,
        ACT_APERTURE,
        APPLICATION,
        MASKGROUP,
        NUM_LAMPS,
        /// paramappings
        COLOR_TEMP,
        DIMMER,
        CELL_SENSITIVITY,
        CELL,
        IR_RFS,
        BRIGHT_TEST,
        AUDIO_MODE,
//        KEY_SOUND_VOL,
        DISP_INTENSITY,
        DAILY_COUNTER,
        OVERALL_COUNTER,
        RESET_FLASH_COUNT,
        /// paramappings
        JOIN_NETWORK,
        WLAN_SSID,
        WLAN_PWD,
        WLAN_ENC,
        REMOTE_SWITCH,
        WINK,
        ERROR
    };
    Q_ENUM(eRoles)


    explicit DeviceProxy(std::weak_ptr<Device> d);
    explicit DeviceProxy(const DeviceProxy &p);
    explicit DeviceProxy();
    virtual ~DeviceProxy();

    // implementation of BronItemAPI
    virtual QVariant getData(int role) const override;
    virtual bool setData(int role, const QVariant &v) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief getParameters return the underlying parameters
     * @return
     */
    BronListModel *getParameters();

//    /**
//     * @brief runService part of the serice interface of the used list model
//     * @param name name of the service
//     * @param data data for the service
//     */
//    virtual void runService(QString name, QVariant data) override;

    /**
     * @brief setOnline sets the device online
     * @param val
     */
    virtual void setOnline(bool val) override;

    /**
     * @brief HostAddress
     * @return host address of the underlying element or an QHostAddress()
     */
    QHostAddress HostAddress() const;

    /**
     * @brief getDevice returns the underlying element, the proxy is designed for
     * @return
     */
    std::weak_ptr<Device> getDevice() const;

    /**
     * @brief comparer
     * @return returns element for comparing function of this object
     */
    virtual QVariant comparer() override;

    /**
     * @brief getRoleForPara get the role in the model for the para
     * @param id of the parameter
     * @return role if there is one or -1 if there is no role assigned
     */
    int getRoleForPara(int id);

    BronListModel &getLampList();

public slots:
    void onParaProxyDataChanged(std::weak_ptr<ParaProxy> pp);
    void onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy> dp);
    void onDeviceProxyUpdateQML(int role, std::weak_ptr<DeviceProxy> dp);

signals:
    /**
     * @brief deviceProxyUpdateQML signaled on a parameter change, not driven
     * by registered para proxy
     * @param dp
     */
    void deviceProxyUpdateQML(std::weak_ptr<DeviceProxy> dp);

    /**
     * @brief deviceProxyUpdateQML signaled on a parameter change, driven by an
     * registered paraProxy
     * @param role role in the device proxy
     * @param dp
     */
    void deviceProxyUpdateQML(int role, std::weak_ptr<DeviceProxy> dp);

    /**
     * @brief deviceProxyDeviceProxyDataChanged emitted if a genLamp has new data
     * @param index
     * @param what
     */
    void deviceProxyDeviceProxyDataChanged(int index, QVector<int> what);

    /**
     * @brief deviceProxyUpdateGeniLampListQML signal a parameter change to a assigned geni list,
     * driven by an parameter (faster, only this hmi part will be updated)
     * @param role
     * @param dp
     */
    void deviceProxyUpdateGeniLampListQML(int index, QVector<int> what);

private:
    std::weak_ptr<Device> m_p;
    ParaProxy m_ItemAPIPrototype;
    BronListModel m_paraList;
    BronListModel m_lampList;
};

inline BronListModel *DeviceProxy::getParameters() {
    return &m_paraList;
}

inline void DeviceProxy::setOnline(bool val){
    if(auto d = m_p.lock())
        d->setOnline(val);
}

inline std::weak_ptr<Device> DeviceProxy::getDevice() const{
    return m_p;
}

inline QVariant DeviceProxy::comparer(){
    if(auto d = m_p.lock()){
        return d->getLampAddress();
    }
    return QVariant();
}

}

Q_DECLARE_METATYPE(std::shared_ptr<QMLInterface::DeviceProxy>)
Q_DECLARE_METATYPE(std::weak_ptr<QMLInterface::DeviceProxy>)
#endif // DEVICEPROXY_H
