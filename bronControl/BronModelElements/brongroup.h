#ifndef BRONGROUP_H
#define BRONGROUP_H

#include <memory>
#include <QObject>
#include <QList>
#include "paraSelectables.h"


namespace Framework{
class Device; /**< foreward declaration */

class BronGroup: public QObject{
    Q_OBJECT
public:
    BronGroup(QObject *parent = nullptr);
    BronGroup(const BronGroup &g);
    virtual ~BronGroup();

    bool setIncEnergy(float val);
    bool setDecEnergy(float val);

    /**
     * @brief getAddress return the address used of the devices in this group
     * @return
     */
    int getAddress() const;

    /**
     * @brief setAddress sets the address used by devices in this group
     * @param addr address of group
     */
    void setAddress(const int addr);

    /**
     * @brief GetColor returns the actual adjusted color
     * @return
     */
    ParaSelects::CognitionLightColor GetColor() const;

    float getCurGroupPower();

    void addDevice(std::weak_ptr<Device> dev);

    bool removeDevice(std::weak_ptr<Device> dev);

    /**
     * @brief getDevices returns a list of all devices within this group
     * @return
     */
    QList<std::weak_ptr<Device> > getDevices();

    float getMinCurGroupPower();

    float getMaxCurGroupPower();

    float getMinGroupPower();

    float getMaxGroupPower();

    int devicesCount() const;

    /**
     * @brief setStandby set the devices in the group unconditional in standby
     * @param standby value to set, true standby, false active
     * @return
     */
    bool setStandby(bool standby);

    /**
     * @brief getStandby get the state of standby, if all devices are in standby, a one is returned,
     * if all devices are active, a zero is returned, if the state of the devices is inconsistent, a two is
     * returned
     * @return state
     */
    int getStandby() const;

//    /**
//     * @brief setShowCogni switches the cogni color on for some seconds (about 30) if the cogni color is enabled
//     * @return
//     */
//    bool setShowCogni();

    /**
     * @brief wink caused devices in the group to give a feedback (siros via cognition color, scoro via test button)
     * @return
     */
    bool wink();
    /**
     * @brief setCogniEnabled enables the cognition color for this group
     * @param val
     * @return
     */
    bool setCogniEnabled(bool val);
    /**
     * @brief getCogniEnabled return the state of cogni enabled
     * @return 0 is disabled, 1 is enabled, -1 function not supported
     */
    int getCogniEnabled() const;

signals:
    void deviceRemoved(std::weak_ptr<Device> dev);
    void deviceAdded(std::weak_ptr<Device> dev);

private:
    QList<std::weak_ptr<Device> > m_devices;
    ParaSelects::CognitionLightColor m_cogniColor;

    int m_address;
};
inline int BronGroup::getAddress() const{
    return m_address;
}
inline void BronGroup::setAddress(const int addr){
    m_address = addr;
}
inline ParaSelects::CognitionLightColor BronGroup::GetColor() const{
    return m_cogniColor;
}
inline float BronGroup::getCurGroupPower(){
    return getMaxCurGroupPower();
}
inline QList<std::weak_ptr<Device> > BronGroup::getDevices(){
    return m_devices;
}
inline int BronGroup::devicesCount() const{
    return m_devices.size();
}

}

#endif // BRONGROUP_H
