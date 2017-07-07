#include "bronconinterface.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

// use a port in the range (49152-65535)

// singleton


BronConInterface::BronConInterface(bool isServer, QObject *parent)
    : QObject(parent)
    , QSharedData()
    , m_RxSock()
    , m_TxSock()
    , m_senderAddr(QHostAddress::LocalHost)
    , m_isServer(isServer)
    , m_rxPort(isServer ? eDefaultPort : eDefaultPort+1)
    , m_txPort(isServer ? m_rxPort+1 : m_rxPort-1){
    if(m_RxSock.bind(QHostAddress::LocalHost, m_rxPort)){
        connect(&m_RxSock, SIGNAL(readyRead()), this, SLOT(onRxData()));
    }
}

QExplicitlySharedDataPointer<BronConInterface> BronConInterface::getInstance(bool isServer){
    static QExplicitlySharedDataPointer<BronConInterface> i(new BronConInterface(isServer));
    return i;
}


BronConInterface::~BronConInterface(){
    m_RxSock.abort();
}

void BronConInterface::setPort(quint16 port){
    m_rxPort = m_isServer ? port : port+1;
    m_txPort = m_isServer ? m_rxPort+1 : m_rxPort-1;
    disconnect(&m_RxSock, SIGNAL(readyRead()), this, SLOT(onRxData()));
    m_RxSock.abort();
    if(m_RxSock.bind(QHostAddress::LocalHost, m_rxPort)){
        connect(&m_RxSock, SIGNAL(readyRead()), this, SLOT(onRxData()));
    }
}

bool BronConInterface::send(const QString &cmd, QString val){
    QJsonObject jObj;
    jObj[cmd] = val;
    QJsonDocument jDoc(jObj);
    QByteArray sndData = jDoc.toJson(QJsonDocument::Compact);
    return sendData(sndData);
}

void BronConInterface::onRxData(){
    while (m_RxSock.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_RxSock.pendingDatagramSize());
        quint16 senderPort;

        m_RxSock.readDatagram(datagram.data(), datagram.size(), &m_senderAddr, &senderPort);
        processRx(datagram);
    }
}

void BronConInterface::processRx(const QByteArray &rxData){
    QJsonDocument jDoc = QJsonDocument::fromJson(rxData);
    QJsonObject jobj = jDoc.object();
    QVariantMap map = jobj.toVariantMap();
    QMapIterator<QString, QVariant> iter(map);
    while(iter.hasNext()){
        iter.next();
        if(iter.key() == "newEnergy"){
            emit newEnergy(iter.value().toFloat());
        }
        else if(iter.key() == "higherEnergy"){
            emit higherEnergy(iter.value().toFloat());
        }
        else if(iter.key() == "lowerEnergy"){
            emit lowerEnergy(iter.value().toFloat());
        }
        else if(iter.key() == "show"){
            emit show();
        }
        else if(iter.key() == "hide"){
            emit hide();
        }
        else if(iter.key() == "onlineChanged"){
            emit onlineChanged(iter.value().toBool());
        }
    }
}
