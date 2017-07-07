#include "NetWorkScanner.h"
#include "bronstudio.h"

using namespace Framework;



Framework::NetworkScanner::NetworkScanner(QObject *parent)
    : QAbstractListModel(parent)
    , QSharedData()
    , m_mgr(new QNetworkConfigurationManager())
    , m_desiredCfg(){
//    , m_scanTimer(){
    // keep track of configuration changes
//    m_scanTimer.setInterval(static_cast<int>(eSettings::rescanTime));
//    m_scanTimer.setSingleShot(false);
//    connect(&m_scanTimer, SIGNAL(timeout()), this, SLOT(TimerUpdateNets()));
    connect(m_mgr, &QNetworkConfigurationManager::updateCompleted, this, &NetworkScanner::UpdateNets);
//    // trigger scan
//    scan();
}

QExplicitlySharedDataPointer<NetworkScanner> Framework::NetworkScanner::getInstance(){
    static QExplicitlySharedDataPointer<NetworkScanner> s_instance(new NetworkScanner);
    return s_instance;
}


Framework::NetworkScanner::~NetworkScanner(){
//    m_scanTimer.stop();
    m_discoveredNets.clear();
}

QVariant Framework::NetworkScanner::data(const QModelIndex &index, int role) const{
    if(index.row() < 0 || index.row() >= m_discoveredNets.count())
        return QVariant();

    const QNetworkConfiguration &net = m_discoveredNets[index.row()];

    if(role == Name){
        return net.name();
    }

    return QVariant();
}

QHash<int, QByteArray> Framework::NetworkScanner::roleNames() const{
    static const QHash<int, QByteArray> roles(std::initializer_list<std::pair<int, QByteArray> >({
              {Name, "name"}}));
    return roles;
}

int Framework::NetworkScanner::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return m_discoveredNets.count();
}

void Framework::NetworkScanner::scan(){
    m_mgr->updateConfigurations();
}

void Framework::NetworkScanner::UpdateNets(){
    m_mgr->disconnect(SIGNAL(updateCompleted()));
    connect(m_mgr, SIGNAL(configurationRemoved(QNetworkConfiguration)), this, SLOT(NetRemoved(QNetworkConfiguration)));
    connect(m_mgr, SIGNAL(configurationAdded(QNetworkConfiguration)), this, SLOT(NetAdded(QNetworkConfiguration)));
    connect(m_mgr, SIGNAL(configurationChanged(QNetworkConfiguration)), this, SLOT(NetConfigChanged(QNetworkConfiguration)));

    QListIterator<QNetworkConfiguration> iter(m_mgr->allConfigurations());
    while(iter.hasNext()){
        QNetworkConfiguration conf = iter.next();
        if(conf.state().testFlag(QNetworkConfiguration::Active) || conf.state().testFlag(QNetworkConfiguration::Discovered)){
            if(!m_discoveredNets.contains(conf)){
                insertMyRow(m_discoveredNets.size(), conf);
            }
        }
    }
    emit firstScanComplete();
//    m_scanTimer.start();
}

//void Framework::NetworkScanner::TimerUpdateNets(){
//    QListIterator<QNetworkConfiguration> iter(m_mgr->allConfigurations());
//    while(iter.hasNext()){
//        QNetworkConfiguration conf = iter.next();
//        qDebug() << conf.name() << " " << conf.state();
//        if(conf.state().testFlag(QNetworkConfiguration::Active) || conf.state().testFlag(QNetworkConfiguration::Discovered)){
//            if(!m_discoveredNets.contains(conf)){
//                qDebug() << conf.name();
//                insertMyRow(m_discoveredNets.size(), conf);
//            }
//        }
//    }
//}



bool Framework::NetworkScanner::insertMyRows(int row, int count, QNetworkConfiguration cfg, const QModelIndex & parent){
    beginInsertRows(parent, row,  row+count-1);
    m_discoveredNets.append(cfg);
    endInsertRows();
    emit netInserted(cfg);
    emit countChanged(m_discoveredNets.size());
    return true;
}

bool Framework::NetworkScanner::removeMyRows(QNetworkConfiguration cfg, const QModelIndex & parent){
    if(m_discoveredNets.contains(cfg)){
        int i = m_discoveredNets.indexOf(cfg);
        beginRemoveRows(parent, i, i);
        m_discoveredNets.removeAt(i);
        endRemoveRows();
        emit netRemoved(cfg);
        emit countChanged(m_discoveredNets.size());
        return true;
    }
    else{
        return false;
    }
}

bool Framework::NetworkScanner::isAutoConnectPossible() const{
    return m_mgr->capabilities().testFlag(QNetworkConfigurationManager::CanStartAndStopInterfaces); //test does not work on mac, but underlying hardware do!!
}

void Framework::NetworkScanner::NetRemoved(const QNetworkConfiguration &cfg){
    if(m_discoveredNets.contains(cfg)){
        removeMyRows(cfg);
    }
}

void Framework::NetworkScanner::NetAdded(const QNetworkConfiguration &cfg){
    if(cfg.state().testFlag(QNetworkConfiguration::Active) || cfg.state().testFlag(QNetworkConfiguration::Discovered)){
        QListIterator<QNetworkConfiguration> iter(m_discoveredNets);
        bool isIn = false;
        while(iter.hasNext()){
            QNetworkConfiguration c = iter.next();
            if(c.name() == cfg.name()){
                isIn = true;
                break;
            }
        }
        if(!isIn){
            insertMyRow(rowCount(),cfg);
        }
    }
}

void Framework::NetworkScanner::NetConfigChanged(const QNetworkConfiguration &cfg){
//    qDebug() << "network configuration changed " << cfg.name();
//    if(cfg.state().testFlag(QNetworkConfiguration::Active)){
//        qDebug() << cfg.name() << " is active now";
//        QNetworkSession session(cfg);
//        qDebug() << session.interface().humanReadableName() << " " << session.interface().index();
//    }
//    else{
//        qDebug() << cfg.name() << " is inactive now";
//    }
    // emit unconditional
    emit netConfigChanged(cfg);
}

QList<QNetworkConfiguration> Framework::NetworkScanner::getNetworks() const{
    return m_discoveredNets;
}

QList<QNetworkConfiguration> Framework::NetworkScanner::getNetworksByName(QString name) const{
    QList<QNetworkConfiguration> ret;
    QListIterator<QNetworkConfiguration> iter(m_discoveredNets);
    while(iter.hasNext()){
        QNetworkConfiguration c = iter.next();
        if(c.name() == name){
            ret.append(c);
        }
    }
    return ret;
}

QNetworkConfiguration Framework::NetworkScanner::defaultConfig() const{
    return m_mgr->defaultConfiguration();
}
