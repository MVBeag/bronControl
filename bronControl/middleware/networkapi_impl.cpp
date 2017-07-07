#include "networkapi_impl.h"
#include "studiomgr.h"
#include "device.h"
#include "remoteaddressinfo.h"
#include "device.h"




static unsigned char crc8_bytecalc(unsigned char reg, unsigned char byte){
  bool bFlag;
  unsigned char polynom = 0xd5; /* Generatorpolynom */

  for (int i = 0; i < 8; i++){
    bFlag = 0 != (reg & 0x80);
    reg <<= 1;
    if (0 != (byte & 0x80)){
      reg |= 1;
    }
    byte <<= 1;
    if(bFlag){
      reg ^= polynom;
    }
  }

  return reg;
}

/*----------------------------------------------------------------------------*/

static unsigned char crc8_messagecalc(const unsigned char *msg, int len){

  unsigned char reg = 0;

  for (int i = 0; i < len; i++) {
    reg = crc8_bytecalc(reg, msg[i]);
  }

  reg = crc8_bytecalc(reg, 0);

  return (reg);
}


//has to be a singelton and manage the connected devices for its own! It is only possible to bind
// to one local port --> do it manually on receiving (select device by incomming address


NetworkAPIImpl::NetworkAPIImpl(QObject *parent, uint16_t localPort)
    : NetworkAPI(parent)
    , m_hostList()
    , m_socket(this)
    , m_localPort(localPort)
    , m_mutex(QMutex::Recursive){
    qRegisterMetaType<uint16_t>("uint16_t");
//    connect(&m_socket, &QUdpSocket::stateChanged, this, &qtNetworkIntegration::onStateChanged);
    // get out the local port and hostaddress
    if(m_socket.bind(m_localPort), QUdpSocket::ShareAddress){
        connect(&m_socket, SIGNAL(readyRead()), this, SLOT(ReadData()));
    }
}


NetworkAPIImpl::~NetworkAPIImpl(){
    m_socket.close();
}

NetworkAPI::Error NetworkAPIImpl::Init(){
    return Error::eNoError;
}

bool NetworkAPIImpl::remove(std::weak_ptr<Device> dev){
    QMutexLocker lock(&m_mutex);
    auto iter = m_hostList.begin();
    auto end = m_hostList.constEnd();
    for(; iter != end; ++iter){
        auto iterD = iter.value();
        if(auto mapDev = iterD.lock()){
            if(auto d = dev.lock()){
                if(mapDev->getHiddenId() == d->getHiddenId()){
                    m_hostList.erase(iter);
                    return true;
                }
            }
        }
    }

    return false;
}


void NetworkAPIImpl::ReadData(){
//    QMutexLocker lock(&m_mutex);
//    QByteArray datagram;

    while (m_socket.hasPendingDatagrams()) {
//        datagram.resize(m_socket.pendingDatagramSize());

//        m_socket.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        auto data = m_socket.receiveDatagram();

        if(data.isValid()){
          auto sender = data.senderAddress();
          auto senderPort = static_cast<quint16>(data.senderPort());

//        if(datagram.size() >= 1){
            auto datagram = data.data();
            unsigned char val1 = datagram[datagram.size() - 1];
            unsigned char val2 = crc8_messagecalc((const unsigned char *)datagram.data(), (int)(datagram.size() - 1));

            if ( val1 == val2 ){
                QMutexLocker lock(&m_mutex);
                QHostAddress ipV4(sender.toIPv4Address());
                if(m_hostList.contains(ipV4)){
                    auto hlist = m_hostList.values(ipV4);
                    QListIterator<std::weak_ptr<Device> >iter(hlist);
                    RemoteAddressInfo ra(sender.toIPv4Address(), senderPort);
                    while(iter.hasNext()){
                        if(auto d = iter.next().lock()){
                            emit d->udpReadData(ra, datagram);
                        }
                    }
                }
                else{
                    // new responding devices are here!!
                    DeviceFinder *df = StudioMgr::getInstance()->getDeviceFinder();
                    df->checkResponse(ipV4, senderPort, data.interfaceIndex());
                }
            }
        }
    }
}

TimerAPI *NetworkAPIImpl::StartTimer(uint32_t timeoutInMs, TimerAPI::timerCB timerCB, Request *req){
    TimerAPI *timer = new TimerAPI(req, timerCB);
    if(NULL != timer){
        timer->OneShot(timeoutInMs);
        return timer;
    }
    return nullptr;
}

void NetworkAPIImpl::StopAndReleaseTimer(TimerAPI *timer){
    if(nullptr != timer){
        timer->deleteLater();
    }
}



void NetworkAPIImpl::onSend(RemoteAddressInfo remoteAddress, QByteArray sndData, uint16_t length){
    QMutexLocker lock(&m_mutex);
    QHostAddress host(remoteAddress.remoteAddress().address);
    // add checksum to data
//    length++;
//    QByteArray sendData(QByteArray::fromRawData(sndData, length));
    sndData[length-1] = crc8_messagecalc(reinterpret_cast<unsigned char*>(sndData.data()), length -1 );
    if(m_socket.writeDatagram(sndData,length, host, remoteAddress.remoteAddress().port ) == -1){
        qDebug() << m_socket.errorString();
    }
}

void NetworkAPIImpl::ReleaseConnection(std::weak_ptr<Device> dev){
    QMutexLocker lock(&m_mutex);
    if(auto d = dev.lock()){
        if(auto net = d->m_network.lock()){
            net->remove(dev);
        }
    }
}

//void qtNetworkIntegration::onStateChanged(QAbstractSocket::SocketState state){
//    qDebug() << "new Socket State: " << static_cast<int>(state);
//}


bool operator<(const QHostAddress &a1, const QHostAddress &a2){
    return a1.toIPv4Address() < a2.toIPv4Address();
}

void NetworkAPIImpl::append(QHostAddress host, std::weak_ptr<Device> d){
    QMutexLocker lock(&m_mutex);
    m_hostList.insert(host, d);
}
