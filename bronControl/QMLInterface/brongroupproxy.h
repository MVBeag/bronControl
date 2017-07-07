#ifndef BRONGROUPPROXY_H
#define BRONGROUPPROXY_H

#include <QObject>
#include "bronitemapi.h"
#include "bronlistmodel.h"
#include "deviceproxy.h"
#include "brongroup.h"



using namespace Framework;
namespace QMLInterface{

class StudioProxy;

class BronGroupProxy : public BronItemAPI{
    Q_OBJECT

public:
    explicit BronGroupProxy(StudioProxy *sp = nullptr, BronGroup *group = nullptr);
    explicit BronGroupProxy(const BronGroupProxy &p);
    virtual ~BronGroupProxy();

    enum eRoles{
        ADDRESS = Qt::UserRole + 1,
        INCPOWER,
        DECPOWER,
        DEVICES,
        COGNICOLOR,
        MINPOWER,
        MAXPOWER,
        MAXMINPOSSIBLEENERGY,
        MINMAXPOSSIBLEENERGY,
        STANDBY
    };


    virtual QVariant getData(int role) const override;
    virtual bool setData(int role, const QVariant &val) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    BronGroup *getGroup(){
        return m_group;
    }

    virtual QVariant comparer() override{
        return getData(ADDRESS);
    }

    void checkPowerLimits();
    void checkPowerLimitsWOSignals();

    QListIterator<std::shared_ptr<BronItemAPI> > getListIterator() const;


public slots:
    void onDeviceProxyUpdateQML(std::weak_ptr<DeviceProxy> dp);
    void onDeviceProxyUpdateQML(int role, std::weak_ptr<DeviceProxy> dp);
    void onDeviceRemoved(std::weak_ptr<Device> d);
    void onDeviceAdded(std::weak_ptr<Device> d);

signals:
    void groupProxyDataChanged(int index, QVector<int> what);
    void elementChanged(int element);


private:
    BronGroup *m_group;
    DeviceProxy m_deviceAPIPrototype;
    BronListModel m_deviceList;
    double m_minPower;  /**< holds the min power (Total energy appertrue) of devices in this studio */
    double m_maxPower;  /**< holds the max power (Total energy apperture) of devices in this studio */
    double m_maxMinPower;
    double m_minMaxPower;
    StudioProxy *m_studioProxy;

    void init();
};

inline QListIterator<std::shared_ptr<BronItemAPI> > BronGroupProxy::getListIterator() const{
    return m_deviceList.getListIterator();
}

}

Q_DECLARE_METATYPE(std::shared_ptr<QMLInterface::BronGroupProxy>)
Q_DECLARE_METATYPE(std::weak_ptr<QMLInterface::BronGroupProxy>)

#endif // BRONGROUPPROXY_H
