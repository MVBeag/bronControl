#include "BronNetworkConfigurationManager.h"

using namespace Framework;
static BronNetworkConfigurationManager *s_instance = nullptr;

Framework::BronNetworkConfigurationManager::BronNetworkConfigurationManager(QObject *parent)
    : QObject(parent)
    , m_mgr()
    , m_activeNetwork()
    , m_networks()
    , m_timer(){
    connect(&m_mgr, SIGNAL(updateCompleted()), this, SIGNAL(updateCompleted()));
    connect(&m_mgr, SIGNAL(configurationRemoved(const QNetworkConfiguration &)), this, SLOT(onConfigurationRemoved(const QNetworkConfiguration &)), Qt::QueuedConnection);
    connect(&m_mgr, SIGNAL(configurationAdded(const QNetworkConfiguration &)), this, SLOT(onConfigurationAdded(const QNetworkConfiguration &)), Qt::QueuedConnection);
    connect(&m_mgr, SIGNAL(configurationChanged(const QNetworkConfiguration &)), this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)), Qt::QueuedConnection);
}

Framework::BronNetworkConfigurationManager::~BronNetworkConfigurationManager(){
    disconnect(&m_mgr, SIGNAL(updateCompleted()), this, SLOT(UpdateNets()));
    disconnect(&m_mgr, SIGNAL(configurationRemoved(QNetworkConfiguration)), this, SLOT(NetRemoved(QNetworkConfiguration)));
    disconnect(&m_mgr, SIGNAL(configurationAdded(QNetworkConfiguration)), this, SLOT(NetAdded(QNetworkConfiguration)));
}

BronNetworkConfigurationManager *Framework::BronNetworkConfigurationManager::getInstance(){
    if(s_instance == nullptr){
        s_instance = new BronNetworkConfigurationManager();
    }
    return s_instance;
}

void Framework::BronNetworkConfigurationManager::destroy(){
    if(s_instance){
        s_instance->deleteLater();
        s_instance = nullptr;
    }
}

BronNetworkConfig Framework::BronNetworkConfigurationManager::getActiveNetwork() const{
    return BronNetworkConfig();
}

void Framework::BronNetworkConfigurationManager::setActiveNetwork(const QString &s){
    if(s != m_activeNetwork.name()){
        m_activeNetwork.setName(s);
    }
}

QListIterator<Framework::BronNetworkConfig> Framework::BronNetworkConfigurationManager::allConfigurations(QNetworkConfiguration::StateFlags filter) const{
    QList<BronNetworkConfig> ret;
    QListIterator<QNetworkConfiguration> iter(m_mgr.allConfigurations(filter));
    while(iter.hasNext()){
        ret.append(iter.next());
    }
    return ret;
}

void Framework::BronNetworkConfigurationManager::updateConfigurations(){
    m_mgr.updateConfigurations();
}

BronNetworkConfig Framework::BronNetworkConfigurationManager::defaultConfiguration() const{
    BronNetworkConfig ret(m_mgr.defaultConfiguration());
    return ret;
}

void Framework::BronNetworkConfigurationManager::newNetworks(QList<JavaNetworkData> &nets){
    Q_UNUSED(nets);
}

void Framework::BronNetworkConfigurationManager::lostNetworks(QList<Framework::JavaNetworkData> &nets){
    Q_UNUSED(nets);
}

void Framework::BronNetworkConfigurationManager::firstScanComplete(QList<JavaNetworkData> &nets){
    Q_UNUSED(nets);
}

void Framework::BronNetworkConfigurationManager::configChanged(JavaNetworkData cfg){

}

void Framework::BronNetworkConfigurationManager::onConfigurationRemoved(const QNetworkConfiguration &c){
    emit configurationRemoved(BronNetworkConfig(c));
}

void Framework::BronNetworkConfigurationManager::onConfigurationAdded(const QNetworkConfiguration &c){
    emit configurationAdded(BronNetworkConfig(c));
}

void Framework::BronNetworkConfigurationManager::onConfigurationChanged(const QNetworkConfiguration &c){
    emit configurationChanged(BronNetworkConfig(c));
}

void Framework::BronNetworkConfigurationManager::onNetScanTimerTimeout(){
    m_timer.stop();
}

void Framework::BronNetworkConfigurationManager::onStartTimer(){

}

bool Framework::BronNetworkConfigurationManager::getSessionRequred() const{
    return m_mgr.capabilities().testFlag(QNetworkConfigurationManager::NetworkSessionRequired);
}
