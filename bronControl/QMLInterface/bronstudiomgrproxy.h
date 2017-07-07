#ifndef BRONSTUDIOMGRPROXY_H
#define BRONSTUDIOMGRPROXY_H

#include <QObject>
#include <QVariant>
#include "bronitemapi.h"
#include "bronlistmodel.h"
#include "studiomgr.h"
#include "studioproxy.h"



using namespace Framework;

namespace QMLInterface{

/**
 * @brief The BronStudioMgrProxy class is a proxy to make the underlying Bron model working in QML
 */
class BronStudioMgrProxy : public QObject{
    Q_OBJECT

    Q_PROPERTY(BronListModel *studios READ getStudios NOTIFY studiosChanged)
    Q_PROPERTY(bool autoConnect READ getAutoConnect NOTIFY autoConnectChanged)
    Q_PROPERTY(QString error READ getError NOTIFY errorChanged)
    Q_PROPERTY(bool show READ getShow NOTIFY showChanged)
    Q_PROPERTY(float userScale READ getUserScale WRITE setUserScale NOTIFY userScaleChanged)
//    Q_PROPERTY(bool button READ getButton WRITE setButton NOTIFY buttonChanged)
public:
    explicit BronStudioMgrProxy(QObject *parent = nullptr);
    explicit BronStudioMgrProxy(const BronStudioMgrProxy &p);
    virtual ~BronStudioMgrProxy();

    /**
     * @brief getStudios getter for studios
     * @return BronListModel, so it is accessible from QML
     */
    BronListModel *getStudios();

    /**
     * @brief getAutoConnect checks if the underlying os allows to start and stop networks from
     * applications
     * @return
     */
    bool getAutoConnect();

    bool getShow() const;

    void setUserScale(const float val);
    float getUserScale()const;

//    void setButton(bool val);
//    bool getButton() const;

    /**
     * @brief getStudioList delivers a pointer to the studio proxy
     * @param index the index to deliver
     * @return pointer to proxy in case of success, nullptr if index is to big
     */
    Q_INVOKABLE QVariant getStudioProxy(int index) const{
        return QVariant::fromValue<std::shared_ptr<StudioProxy> >(std::dynamic_pointer_cast<StudioProxy>(m_studioList.getItem(index)));
    }

    /**
     * @brief getStudiosCount get the number of stored StudioProxy pointers in m_studioList
     * @return
     */
    Q_INVOKABLE int getStudiosCount() const{
        return m_studioList.getSize();
    }

    /**
     * @brief createStudio creates and inserts a new studio with given
     * @param ssid new ssid
     * @param number new studioAddress
     * @param pw network password
     * @param encr network encryption
     * @return true in case of success, false if not
     */
    Q_INVOKABLE bool createStudio(QVariant ssid, QVariant number, QVariant pw, QVariant encr);
    Q_INVOKABLE void deleteStudio(int index);

    /**
     * @brief setNetworkMode
     * @param actual studioname
     * @param actual studioAddress
     * @param enterprise true for enterprise mode, false for private mode
     * @param ssid network to choose
     * @param pw password
     * @param encr encription WPA, WPA2
     */
    Q_INVOKABLE void setNetworkMode(QVariant studioName, int studioAddress, bool enterprise, QVariant ssid, QVariant pw, QVariant encr);

    /**
     * @brief populateDemoStudio creates the demo studio
     */
    void populateDemoStudio();

    QString getError() const;

    /**
     * @brief startDiscovery starts an unconditional device discovery on all valid interfaces
     */
    void startDiscovery();

    /**
     * @brief stopDiscovery stops the discovery process
      */
    void stopDiscovery();

    /**
     * @brief getPossiblePasswords little helper to print possible stored passwords
     * @param ssid of network, password
     * @param address addres of the network
     * @return password if found, else empty string
     */

    Q_INVOKABLE QString getPossiblePassword(QString ssid);



signals:
    /**
     * @brief studiosChanged signal to inform QML that m_studioList has changed
     */
    void studiosChanged();
    void autoConnectChanged();
    void errorChanged(QString errorText);
    void showChanged(bool show);
//    void buttonChanged(bool button);
    void switchToStudio(int ix);
    void userScaleChanged(float userScale);
    void swVersionRead(int version, QString deviceType);

public slots:
    void addStudio(const BronStudio *);
    void removeStudio(const BronStudio *s);
    void onError(QString errorText);
    void onShow();
    void onHide();
    void onChangeOver(BronStudio *target);
    void onDeviceSwVersionRead(std::shared_ptr<Device> dev);

private:
    QExplicitlySharedDataPointer<StudioMgr> m_studioMgr;
    StudioProxy m_studioPrototype;
    BronListModel m_studioList;
    QString m_error;
    bool m_show;
};

inline BronListModel *BronStudioMgrProxy::getStudios(){
    return &m_studioList;
}

inline bool BronStudioMgrProxy::getShow() const{
    return m_show;
}

inline void BronStudioMgrProxy::setUserScale(const float val){
    if(m_studioMgr){
        if(val != m_studioMgr->getUserScale()){
            m_studioMgr->setUserScale(val);
        }
    }
}

inline float BronStudioMgrProxy::getUserScale() const{
    if(m_studioMgr){
        return m_studioMgr->getUserScale();
    }
    return 1.0f;
}

inline void BronStudioMgrProxy::populateDemoStudio(){
    if(m_studioMgr){
        m_studioMgr->populateDemoStudio();
    }
}

inline QString BronStudioMgrProxy::getError() const{
    return m_error;
}

inline void BronStudioMgrProxy::onError(QString errorText){
    m_error = errorText;
    emit errorChanged(errorText);
}



}

Q_DECLARE_METATYPE(QMLInterface::BronStudioMgrProxy)
#endif // BRONSTUDIOMGRPROXY_H
