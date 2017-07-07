#ifndef ETHADDRESS_H
#define ETHADDRESS_H

#include <QObject>
#include <QProcess>
#include <QHostAddress>

class ethAddress : public QObject{
    Q_OBJECT
public:
    explicit ethAddress(QHostAddress ip, QObject *parent = 0);
    ~ethAddress();
    QString getEthernetAddress() const;

signals:
    void ethResolved(ethAddress *);

public slots:
    void readStandardOutput();
    void onfinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QString m_ethAddress;
    QProcess *m_arp;
    QString m_ip;

};

inline QString ethAddress::getEthernetAddress() const{
    return m_ethAddress;
}

#endif // ETHADDRESS_H
