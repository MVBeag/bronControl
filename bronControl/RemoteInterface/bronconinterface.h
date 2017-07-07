#ifndef BRONCONINTERFACE_H
#define BRONCONINTERFACE_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class BronConInterface : public QObject, public QSharedData{
    Q_OBJECT
public:
    enum settings{
        eDefaultPort = 49500
    };

    static QExplicitlySharedDataPointer<BronConInterface> getInstance(bool isServer);
    ~BronConInterface();

    void setPort(quint16 port);
    quint16 getPort() const;


    bool send(const QString &cmd, QString val);

signals:
    void newEnergy(float val);
    void higherEnergy(float val);
    void lowerEnergy(float val);
    void show();
    void hide();
    void onlineChanged(bool val);

private slots:
    void onRxData();

public slots:
    void onNewEnergy(double val);
    void onHigherEnergy(double val);
    void onLowerEnergy(double val);
    void onShow();
    void onHide();


private:
    QUdpSocket m_RxSock;
    QUdpSocket m_TxSock;
    QHostAddress m_senderAddr;
    bool m_isServer;
    quint16 m_rxPort;
    quint16 m_txPort;

    BronConInterface(bool isServer, QObject *parent = 0);

    qint64 sendData(QByteArray &sendData);
    void processRx(const QByteArray &rxData);
};



inline quint16 BronConInterface::getPort() const{
    return m_rxPort;
}

inline void BronConInterface::onNewEnergy(double val){
    send("newEnergy", QString::number(val));
}

inline void BronConInterface::onLowerEnergy(double){
    send("lowerEnergy", QString::number(0.1));
}

inline void BronConInterface::onShow(){
    send("show", "undefined");
}

inline void BronConInterface::onHide(){
    send("hide", "undefined");
}

inline void BronConInterface::onHigherEnergy(double){
    send("higherEnergy", QString::number(0.1));
}

inline qint64 BronConInterface::sendData(QByteArray &sendData){
    return m_TxSock.writeDatagram(sendData, m_senderAddr, m_txPort);
}

#endif // BRONCONINTERFACE_H
