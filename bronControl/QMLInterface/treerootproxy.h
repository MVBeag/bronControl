/**
  * @abstract we use a proxy for every element in the underlying C++ model to get the data to QML
  * this ParaProxy is the proxy for DevicePara (a parameter of a device).
  *
  * Here are the accessor function from QML side
  */

#ifndef TREEROOTPROXY_H
#define TREEROOTPROXY_H

#include <memory>
#include <QObject>
#include <limits>
#include <QtNetwork/QNetworkInterface>
#include "bronitemapi.h"

/**
 * @brief The ParaProxy class represents a DevicePara object in QML
 */

namespace QMLInterface{


class TreeRootProxy : public BronItemAPI{
    Q_OBJECT
public:

    /**
     * @brief ParaProxy consructor, is called without DevicePara, it constructs a prototye (only used to get the roleNames)
     * @param p DevicePara behind the proxy
     * @param parent
     */
    explicit TreeRootProxy();
    virtual ~TreeRootProxy();
    TreeRootProxy(const TreeRootProxy &p);
    TreeRootProxy &operator=(const TreeRootProxy &p);

    enum eRoles{
        NAME = Qt::UserRole + 1
    };
    Q_ENUM(eRoles)

    // implementation of BronItemAPI
    virtual QVariant getData(int role) const override;
    virtual bool setData(int role, const QVariant &v) override;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual QVariant comparer() override;
};


inline QVariant TreeRootProxy::comparer(){
    return QVariant();
}

}

Q_DECLARE_METATYPE(std::shared_ptr<QMLInterface::TreeRootProxy>)
Q_DECLARE_METATYPE(std::weak_ptr<QMLInterface::TreeRootProxy>)
#endif // TREEROOTPROXY_H
