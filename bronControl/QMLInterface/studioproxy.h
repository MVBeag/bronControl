#ifndef STUDIOPROXY_H
#define STUDIOPROXY_H

#include <QObject>
//#include <QMutex>
//#include <QMutexLocker>
#include "deviceproxy.h"
#include "brongroupproxy.h"
#include "bronlistmodel.h"
#include "bronitemapi.h"
#include "bronstudio.h"
#include "bronconinterface.h"


namespace QMLInterface{

/**
 * @brief The StudioProxy class exposes the devices and groups to QML. This is done based on QAbstractListModel
 */

class StudioProxy : public BronItemAPI{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)// WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString ssid READ getSsid WRITE setSsid NOTIFY ssidChanged)
    Q_PROPERTY(BronListModel* deviceList READ deviceList NOTIFY deviceListChanged)
    friend class Framework::BronStudio;
public:
    StudioProxy(BronStudio *std = nullptr);
    StudioProxy(const StudioProxy &p);
    virtual ~StudioProxy();

    /**
     * @brief The eRoles enum used for
     */
    enum eRoles{
        NAME = Qt::UserRole + 1,
        NETWORK,
        SSID,
        PASSWORD,
        NETWORKSTATE,
        STUDIO,
        ADDRESS,
        GROUPS,
        DEVICES,
        ONLINE,
        AWARENESS,
        INCPOWER,
        DECPOWER,
        SWITHMODLIGHT,
        SWITCHSTANDBY,
        SWITCHCOGNILIGHT,
        TEST,
        MINPOWER,
        MAXPOWER,
        MAXMINPOSSIBLEENERGY,
        MINMAXPOSSIBLEENERGY,
        SPEED,
        AUDIO,
        APPLICATION,
        COGNITION,
        COGNITION_ENABLE,
        SWITCH_REMOTE
    };
    Q_ENUM(eRoles)

    enum class eNetworkState{
        Unknown = static_cast<int>(Framework::BronStudio::eNetworkState::eUnknown),
        KnownNotVisible = static_cast<int>(Framework::BronStudio::eNetworkState::eKnownNotVisible),
        KnownAccessable = static_cast<int>(Framework::BronStudio::eNetworkState::eKnownAccessable),
        Active = static_cast<int>(Framework::BronStudio::eNetworkState::eActive)
    };
    Q_ENUM(eNetworkState);


    void clearElements();

    // overrides from BronItemAPI
    virtual QVariant getData(int role) const override;
    virtual bool setData(int role, const QVariant &v) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    // property interfaces
    QString getName() const;
    QString getSsid()const;
    void setSsid(const QString s);
    QString getPw() const;
    void setPw(const QString pw);
    void setEncr(int mode);
    int getAddress()const;

    BronListModel* deviceList();

    virtual QVariant comparer() override;

    virtual void setOnline(bool val) override;

    void checkPowerLimits();
    void checkPowerLimitsWOSignals();

    std::shared_ptr<DeviceProxy> getProxyOfDevice(std::weak_ptr<Device> p) const;

    QVariantMap incubateDevices(QVariantMap val);

    BronStudio *getStudio() const;

public slots:
    void onBronStudioNewDevice(std::weak_ptr<Device> d );
    void onBronStudioGoneDevice(std::shared_ptr<Device> d);

    void onBronStudioGroupAdded(BronGroup *group);
    void onBronStudioGroupRemoved(BronGroup *group);

    void onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy> dp);
    void onDeviceProxyUpdateQML(int role, std::weak_ptr<DeviceProxy> dp);
    void onNetConfigChanged();
    void onDataChanged(BronStudio::eData data);

    void onError(QString errorText);
    void onShowCogni();

signals:
    void nameChanged(const QString &newName);
    void ssidChanged(const QString &newSsid);
    void deviceListChanged();
    void studioProxyDeviceProxyDataChanged(int index, QVector<int> what);
    void studioProxyDataChanged(int index, QVector<int> what);
    void error(QString errorText);

protected:
    BronStudio *m_studio;
    DeviceProxy m_deviceAPIPrototype;  /**< prototype for device access */
    BronGroupProxy m_groupAPIPrototype; /**< prototype for group access */
    BronListModel m_deviceList;  /**< holds the devicelist for the proxy */
    BronListModel m_groupList;   /**< holds the grouplist for the proxy */
    double m_minPower;  /**< holds the min power (Total energy appertrue) of devices in this studio */
    double m_maxPower;  /**< holds the max power (Total energy apperture) of devices in this studio */
    double m_maxMinPower;
    double m_minMaxPower;
//    BronConInterface *m_remote;
//    QMutex m_mutex;
};

inline QString StudioProxy::getName() const{
    if(m_studio) return m_studio->Name();
    return QString();
}

inline QString StudioProxy::getSsid() const{
    if(m_studio) return m_studio->Ssid();
    return QString();
}

inline QString StudioProxy::getPw() const{
    if(m_studio) return m_studio->Password();
    return QString();
}

inline void StudioProxy::setPw(const QString pw){
    if(m_studio) m_studio->setPassword(pw);
}

inline void StudioProxy::setEncr(int mode){
    if(m_studio) m_studio->setEncrypionMode(mode);
}

inline int StudioProxy::getAddress() const{
    if(m_studio){
        return m_studio->getAddress();
    }
    return -1;
}

inline void StudioProxy::setSsid(const QString s){
    if(m_studio) m_studio->setSsid(s);
}

inline BronListModel *StudioProxy::deviceList() {
    return &m_deviceList;
}

inline QVariant StudioProxy::comparer(){
    return getData(ADDRESS);
}

inline void StudioProxy::setOnline(bool val){
    if(m_studio) m_studio->setOnline(val);
}

inline BronStudio *StudioProxy::getStudio() const{
    return m_studio;
}

inline void StudioProxy::onError(QString errorText){
    emit error(errorText);
}

}

Q_DECLARE_METATYPE(std::shared_ptr<QMLInterface::StudioProxy>)
Q_DECLARE_METATYPE(std::weak_ptr<QMLInterface::StudioProxy>)

#endif // STUDIOPROXY_H
