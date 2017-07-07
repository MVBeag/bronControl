#include "BronNetworkConfigurationManager.h"

Framework::BronNetworkConfigurationManager::BronNetworkConfigurationManager(QObject *parent)
    : QObject(parent)
    , m_activeNetwork()
    , m_networks(){}

void Framework::BronNetworkConfigurationManager::setActiveNetwork(const QString &s){
    if(s != m_activeNetwork){
        m_activeNetwork = s;
    }
}
