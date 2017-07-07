/**
  * @abstract we use a proxy for every element in the underlying C++ model to get the data to QML
  * this ParaProxy is the proxy for DevicePara (a parameter of a device).
  *
  * Here are the accessor function from QML side
  */

#ifndef NETWORKPROXY_H
#define NETWORKPROXY_H

#include <memory>
#include <QObject>
#include <limits>
#include <QtNetwork/QNetworkInterface>
#include "bronitemapi.h"

/**
 * @brief The ParaProxy class represents a DevicePara object in QML
 */

namespace QMLInterface{


class NetworkProxy : public BronItemAPI{
    Q_OBJECT
public:

    /**
     * @brief ParaProxy consructor, is called without DevicePara, it constructs a prototye (only used to get the roleNames)
     * @param p DevicePara behind the proxy
     * @param parent
     */
    explicit NetworkProxy(std::weak_ptr<QNetworkInterface> para = std::weak_ptr<QNetworkInterface>());
    virtual ~NetworkProxy();
    NetworkProxy(const NetworkProxy &p);
    NetworkProxy &operator=(const NetworkProxy &p);

    enum eRoles{
        NAME = Qt::UserRole + 1,
        IDENTIFIER
    };
    Q_ENUM(eRoles)

    // implementation of BronItemAPI
    virtual QVariant getData(int role) const override;
    virtual bool setData(int role, const QVariant &v) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int getId() const;

    virtual QVariant comparer() override;


private:
    std::shared_ptr<QNetworkInterface> m_interface;
};


inline int NetworkProxy::getId() const{
    if(m_interface){
        return m_interface->index();
    }
    else
        return -1;
}

inline QVariant NetworkProxy::comparer(){
    if(m_interface){
        return m_interface->index();
    }
    return QVariant();
}

}

Q_DECLARE_METATYPE(std::shared_ptr<QMLInterface::NetworkProxy>)
Q_DECLARE_METATYPE(std::weak_ptr<QMLInterface::NetworkProxy>)
#endif // NETWORKPROXY_H
