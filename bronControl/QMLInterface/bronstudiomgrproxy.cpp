#include "bronstudiomgrproxy.h"
#include "studioproxy.h"
#include <QQmlEngine>
#include <QDebug>
#include "appcontrol.h"

using namespace Framework;


QMLInterface::BronStudioMgrProxy::BronStudioMgrProxy(QObject *parent)
    : QObject(parent)
    // allow auto search in studioMgr
    , m_studioMgr(StudioMgr::getInstance())
    , m_studioPrototype()
    , m_studioList("studioList",&m_studioPrototype,parent)
    , m_show(false){
    // fill studioList with m_studioMgr
    const QMap<QString, BronStudio*> *studios = m_studioMgr->getStudios();
    QMap<QString, BronStudio*>::const_iterator iter = studios->constBegin();
    QMap<QString, BronStudio*>::const_iterator end = studios->constEnd();
    for(; iter != end; ++iter){
        if(auto py = std::make_shared<StudioProxy>(*iter)){
            int ix = m_studioList.insert(py);
            if(ix != -1){
                connect(py.get(), SIGNAL(studioProxyDataChanged(int, QVector<int>)), &m_studioList, SLOT(dataChanged(int, QVector<int>)));//, Qt::QueuedConnection);
                py->setModelIx(ix);
            }
        }
    }
    connect(m_studioMgr.data(), &StudioMgr::studioAdded, this, &BronStudioMgrProxy::addStudio);//, Qt::QueuedConnection);
    connect(m_studioMgr.data(), &StudioMgr::studioRemoved, this, &BronStudioMgrProxy::removeStudio);//, Qt::QueuedConnection);
    connect(m_studioMgr.data(), &StudioMgr::userScaleChanged, this, &BronStudioMgrProxy::userScaleChanged);
    connect(m_studioMgr.data(), &StudioMgr::deviceSwVersionRead, this, &BronStudioMgrProxy::onDeviceSwVersionRead);
}

QMLInterface::BronStudioMgrProxy::BronStudioMgrProxy(const QMLInterface::BronStudioMgrProxy &p)
    : QObject(p.parent())
    , m_studioMgr(p.m_studioMgr)
    , m_studioPrototype()
    , m_studioList(p.m_studioList)
    , m_show(false){
    connect(m_studioMgr.data(), &StudioMgr::studioAdded, this, &BronStudioMgrProxy::addStudio);//, Qt::QueuedConnection);
    connect(m_studioMgr.data(), &StudioMgr::studioRemoved, this, &BronStudioMgrProxy::removeStudio);//, Qt::QueuedConnection);
    connect(m_studioMgr.data(), &StudioMgr::userScaleChanged, this, &BronStudioMgrProxy::userScaleChanged);
    connect(m_studioMgr.data(), &StudioMgr::deviceSwVersionRead, this, &BronStudioMgrProxy::onDeviceSwVersionRead);
}

QMLInterface::BronStudioMgrProxy::~BronStudioMgrProxy(){
    stopDiscovery();
}

bool QMLInterface::BronStudioMgrProxy::getAutoConnect(){
    if(m_studioMgr){
        return m_studioMgr->autoConnectPossible();
    }
    return false;
}


bool QMLInterface::BronStudioMgrProxy::createStudio(QVariant ssid, QVariant number, QVariant pw, QVariant encr){
    if(m_studioMgr){
        return m_studioMgr->createStudio(ssid.toString(), number.toInt(), pw.toString(), encr.toInt());
    }
    return false;
}

void QMLInterface::BronStudioMgrProxy::deleteStudio(int index){
    if(auto item = m_studioList.removeItemAt(index, true, false)){
        if(auto py = std::dynamic_pointer_cast<StudioProxy>(item)){
            BronStudio *studio = py->getStudio();
            // remove studio from settings
            if(studio){
                QSettings settings;
                settings.setFallbacksEnabled(false);
                settings.beginGroup("Studio Manager");
                settings.beginGroup("Studios");
                QListIterator<QString> studiosList(settings.childGroups());
                while(studiosList.hasNext()){
                    QString studioName;
                    QString studio_Sign = studiosList.next();
                    auto nameParts = studio_Sign.split("_#@");
                    if(nameParts.size()>=1) studioName = nameParts[0];
                    if(studioName == studio->Name()){
                        settings.remove(studio_Sign);
                    }
                }
                settings.endGroup();
                settings.endGroup();
                if(m_studioMgr){
                    QMultiMap<QString, BronStudio *> *studios = m_studioMgr->getStudios();
                    if(studios->contains(studio->Name())){
                        QList<BronStudio *> list = studios->values(studio->Name());
                        QMutableListIterator<BronStudio *> iter(list);
                        while(iter.hasNext()){
                            auto s = iter.next();
                            if(s == studio){
                                disconnect(m_studioMgr.data(), SIGNAL(netConfigChanged(const QNetworkConfiguration)), studio, SLOT(onNetConfigChanged(const QNetworkConfiguration)));
                                s->deleteDevices();
                                studio->deleteLater();
                            }
                        }

//                        BronStudio *s = studios->take(studio->Name());
//                        if(s == studio){
//                            disconnect(m_studioMgr.data(), SIGNAL(netConfigChanged(const QNetworkConfiguration)), studio, SLOT(onNetConfigChanged(const QNetworkConfiguration)));
//                            s->deleteDevices();
//                            studio->deleteLater();
//                        }
                    }
                }
            }
        }
    }
}

void QMLInterface::BronStudioMgrProxy::setNetworkMode(QVariant studioName, int studioAddress, bool enterprise, QVariant ssid, QVariant pw, QVariant encr){
    // get the request to change to enterprise mode
//    qDebug() << studioName << " " << studioAddress << " " << enterprise << " " << ssid << " " << pw;
    // first, find the actual studio (by name and address)
    int foundAt = -1;
    BronStudio *actStudio = nullptr;
    QString actName = studioName.toString();
    QListIterator<std::shared_ptr<BronItemAPI> > studios(m_studioList.getListIterator());
    while(studios.hasNext()){
        if(auto py = std::dynamic_pointer_cast<StudioProxy>(studios.next())){
            if(py->getName() == actName && py->getAddress() == studioAddress){
                foundAt = m_studioList.indexOf(py);
                actStudio = py->getStudio();
                break;
            }
        }
    }

    // nothing to do, if actStudio isn't found, shouldn't happen :-)
    if(actStudio == nullptr){
        return;
    }


    // second, check if target studio is available (ssid (name) and address)
    std::shared_ptr<StudioProxy>newProxy;
    studios = m_studioList.getListIterator();
    while(studios.hasNext()){
        if(auto py = std::dynamic_pointer_cast<StudioProxy>(studios.next())){
            if(py->getSsid() == ssid.toString() && py->getAddress() == studioAddress){
                py->setPw(pw.toString());
                py->setEncr(encr.toInt());
                newProxy = py;
                m_studioMgr->storeSettings();
                break;
            }
        }
    }


    // if target is not here, create a new one
    if(!newProxy){
        if(createStudio(ssid, actStudio->getAddress(), pw, encr)){
            studios = m_studioList.getListIterator();
            while(studios.hasNext()){
                if(auto py = std::dynamic_pointer_cast<StudioProxy>(studios.next())){
                    if(py->getSsid() == ssid.toString()){
                        newProxy = py;
                        break;
                    }
                }
            }
        }
    }

    // if both studios are available actStuio and newStdio
    if(-1 != foundAt && newProxy){
        actStudio->migrateToNewNetwork(enterprise, ssid.toString(), pw.toString(), static_cast<ParaSelects::EncryptionMode>(encr.toInt()));
    }
}

void QMLInterface::BronStudioMgrProxy::startDiscovery(){
    m_studioMgr->startDiscovery();
}

void QMLInterface::BronStudioMgrProxy::stopDiscovery(){
    m_studioMgr->stopDiscovery();
}

QString QMLInterface::BronStudioMgrProxy::getPossiblePassword(QString ssid){
    if(!ssid.isEmpty()){
        QListIterator<std::shared_ptr<BronItemAPI> > studios(m_studioList.getListIterator());
        while(studios.hasNext()){
            if(auto py = std::dynamic_pointer_cast<StudioProxy>(studios.next())){
                if(py->getSsid() == ssid){
                    return py->getPw();
                }
            }
        }

        studios = m_studioList.getListIterator();
        while(studios.hasNext()){
            if(auto py = std::dynamic_pointer_cast<StudioProxy>(studios.next())){
                if(py->getSsid().startsWith(ssid)){
                    return py->getPw();
                }
            }
        }
    }
   return QString();
}

void QMLInterface::BronStudioMgrProxy::addStudio(const BronStudio *s){
    // check, if we have this already
    int size = m_studioList.getSize();
    bool isIn = false;
    for(int i = 0; i < size; i++){
        if(auto it = m_studioList.getItem(i)){
            if(auto st = std::dynamic_pointer_cast<StudioProxy>(it)){
                if(st->getName() == s->Name()){
                    if(st->getAddress() == s->getAddress()){
                        isIn = true;
                    }
                }
            }
        }
    }
    if(!isIn){
        if(auto proxy = std::make_shared<StudioProxy>(const_cast<BronStudio*>(s))){
            connect(proxy.get(), SIGNAL(studioProxyDataChanged(int, QVector<int>)), &m_studioList, SLOT(dataChanged(int, QVector<int>)));
            connect(proxy.get(), SIGNAL(error(QString)), this, SLOT(onError(QString)));
            m_studioList.insert(proxy, true, false);
        }
    }

}

void QMLInterface::BronStudioMgrProxy::removeStudio(const BronStudio *s){
    // try to find the proxy of this studio
    int size = m_studioList.getSize();
    for(int i = 0; i < size; i++){
        if(auto st = std::dynamic_pointer_cast<StudioProxy>(m_studioList.getItem(i))){
            if(st->getName() == s->Name()){
                if(st->getAddress() == s->getAddress()){
                    m_studioList.removeAt(i);
                    disconnect(st.get(), SIGNAL(studioProxyDataChanged(int, QVector<int>)), &m_studioList, SLOT(dataChanged(int, QVector<int>)));
                    disconnect(st.get(), SIGNAL(error(QString)), this, SLOT(onError(QString)));
                }
            }
        }
    }
}

void QMLInterface::BronStudioMgrProxy::onShow(){
    if(!m_show){
        m_show = true;
        emit showChanged(m_show);

    }
}

void QMLInterface::BronStudioMgrProxy::onHide(){
    if(m_show){
        m_show = false;
        emit showChanged(m_show);
    }
}

void QMLInterface::BronStudioMgrProxy::onChangeOver(BronStudio *target){
    // find target in studioList
    QListIterator<std::shared_ptr<BronItemAPI> >iter(m_studioList.getListIterator());
    while(iter.hasNext()){
        if(auto sp = std::dynamic_pointer_cast<StudioProxy>(iter.next())){
            if(sp->getSsid() == target->Ssid()){
                if(sp->getStudio()->getAddress() == target->getAddress()){
                    emit switchToStudio(sp->getModelIx());
                }
            }
        }
    }
}

void QMLInterface::BronStudioMgrProxy::onDeviceSwVersionRead(std::shared_ptr<Device> dev){
    if(dev){
        int version = dev->getSWVersion();
        Device::eDeviceTypes type = dev->getType();
        QString sType = "";
        bool doEmit = false;
        switch(type){
            case Device::eDeviceTypes::eSiros:  doEmit = true; sType = "sirosPlain"; break;
            case Device::eDeviceTypes::eSirosS: doEmit = true; sType = "sirosS"; break;
            case Device::eDeviceTypes::eSirosL: doEmit = true; sType = "sirosL"; break;
            case Device::eDeviceTypes::eScoro: doEmit = true; sType = "scoro"; break;
            case Device::eDeviceTypes::eGenLamp:doEmit = true; sType = "genLamp"; break;
            default: return;
        }

        if(doEmit){
            emit swVersionRead(version, sType);
        }
    }
}

