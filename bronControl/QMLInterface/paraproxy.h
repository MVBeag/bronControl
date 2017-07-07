/**
  * @abstract we use a proxy for every element in the underlying C++ model to get the data to QML
  * this ParaProxy is the proxy for DevicePara (a parameter of a device).
  *
  * Here are the accessor function from QML side
  */

#ifndef PARAPROXY_H
#define PARAPROXY_H

#include <memory>
#include <QObject>
#include <limits>
#include "devicepara.h"
#include "bronitemapi.h"

/**
 * @brief The ParaProxy class represents a DevicePara object in QML
 */

using namespace Framework;

namespace QMLInterface{


class ParaProxy : public BronItemAPI{
    Q_OBJECT
public:

    /**
     * @brief ParaProxy consructor, is called without DevicePara, it constructs a prototye (only used to get the roleNames)
     * @param p DevicePara behind the proxy
     * @param parent
     */
    explicit ParaProxy(std::weak_ptr<DevicePara> para = std::weak_ptr<DevicePara>());
    virtual ~ParaProxy();
    ParaProxy(const ParaProxy &p);
    ParaProxy &operator=(const ParaProxy &p);

    enum eRoles{
        VALUE = Qt::UserRole + 1,
        IDENTIFIER,
        DEVICE_ROLE
    };
    Q_ENUM(eRoles)

    // implementation of BronItemAPI
    virtual QVariant getData(int role) const override;
    virtual bool setData(int role, const QVariant &v) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int getId() const;

    virtual QVariant comparer() override;

    void setRole(int val);

    int getRole() const;

public slots:
    void onDeviceParaDataChanged(std::weak_ptr<DevicePara> dp);


signals:
    void paraProxyDataChanged(std::weak_ptr<ParaProxy> pp);


private:
    std::weak_ptr<DevicePara> m_p;
    int m_role;
    void makeConnections();
};


inline void ParaProxy::setRole(int val){
    m_role = val;
}

inline int ParaProxy::getRole() const{
    return m_role;
}




inline int ParaProxy::getId() const{
    if(auto p = m_p.lock()){
        return p->Id();
    }
    else
        return 0;
}

inline QVariant ParaProxy::comparer(){
    if(auto p = m_p.lock()){
        return static_cast<int>(p->Id());
    }
    return QVariant();
}

inline void ParaProxy::onDeviceParaDataChanged(std::weak_ptr<DevicePara> dp){
    Q_UNUSED(dp);
    emit paraProxyDataChanged(std::dynamic_pointer_cast<ParaProxy>(shared_from_this()));
}


}

Q_DECLARE_METATYPE(std::shared_ptr<QMLInterface::ParaProxy>)
Q_DECLARE_METATYPE(std::weak_ptr<QMLInterface::ParaProxy>)
#endif // PARAPROXY_H
