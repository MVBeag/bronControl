

#ifndef FrameworkOnly

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQmlContext>
#include <QScreen>
#include "timerapi.h"
#include "networkapi_impl.h"
#include <QtGlobal>
#include <QtQml>
#include <QObject>
#include <QQueue>
#include <QIcon>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QSettings>
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif
#include <QFileSelector>
#include "NetWorkScanner.h"
#include "resolution.h"
#include "devicesirros.h"
#include "devicesirrosL.h"
#include "devicesirrosS.h"
#include "devicescoro.h"
#include "genLamp.h"
#include "bronstudio.h"
#include "studiomgr.h"
#include "deviceproxy.h"
#include "bronstudiomgrproxy.h"
#include "globalinit.h"
#include "remoteaddressinfo.h"
#include "request.h"
#include "errors.h"
#include "bronconinterface.h"
#include "appcontrol.h"
#include "generalinfo.h"
#include "texteditdoublevalidator.h"
#include "texteditintvalidator.h"
#include <QLocale>
#include "treemodel.h"
#include "networkproxy.h"

using namespace QMLInterface;
using namespace Framework;

static QObject *globalsProvider(QQmlEngine *engine, QJSEngine *scriptEngine){
      Q_UNUSED(engine)
      Q_UNUSED(scriptEngine)
      return new GeneralInfo();
  }

int main(int argc, char *argv[]){

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    foreach (QScreen * screen, QGuiApplication::screens())
            screen->setOrientationUpdateMask(Qt::LandscapeOrientation |
                                             Qt::PortraitOrientation |
                                             Qt::InvertedLandscapeOrientation |
                                             Qt::InvertedPortraitOrientation);
    QCoreApplication::setOrganizationName("Bron Elektronik AG");
    QCoreApplication::setOrganizationDomain("bron.ch");
    QCoreApplication::setApplicationName("BronControl");
    QSettings::setDefaultFormat(QSettings::IniFormat);
//    QLocale::setDefault(QLocale::system());//QLocale::c());
//    QLocale local;
//    qDebug() << "system qlocal " << QLocale::languageToString(local.language());
    QSettings settings;
    settings.setFallbacksEnabled(false);
    settings.beginGroup("AppSettings");
    settings.setValue("Version", GeneralInfo::sVersion());
    settings.endGroup();

    qRegisterMetaType<RemoteAddressInfo>();
    qRegisterMetaType<Request *>();
    // give qRegisterMetaType a string as name, otherwise it is not found
    qRegisterMetaType<std::weak_ptr<Framework::Device> >("std::weak_ptr<Device>");
    qRegisterMetaType<std::shared_ptr<Framework::Device> >("std::shared_ptr<Device>");
    qRegisterMetaType<std::weak_ptr<Framework::DevicePara> >("std::weak_ptr<DevicePara>");
    qRegisterMetaType<std::shared_ptr<Framework::DevicePara> >("std::shared_ptr<DevicePara>");

    qRegisterMetaType<std::weak_ptr<QMLInterface::BronItemAPI> >("std::weak_ptr<BronItemAPI>");
    qRegisterMetaType<std::shared_ptr<QMLInterface::BronItemAPI> >("std::shared_ptr<BronItemAPI>");
    qRegisterMetaType<std::weak_ptr<QMLInterface::DeviceProxy> >("std::weak_ptr<DeviceProxy>");
    qRegisterMetaType<std::shared_ptr<QMLInterface::DeviceProxy> >("std::shared_ptr<DeviceProxy>");

    qRegisterMetaType<std::shared_ptr<Framework::StudioSequence> >("std::shared_ptr<StudioSequence>");
    qRegisterMetaType<Framework::Errors::Id>("Errors::Id");
    AppControl *ctrl = AppControl::getInstance(&app);
    app.installEventFilter(ctrl);

    app.setWindowIcon(QIcon("qrc:///res/BronCon.icns"));

    BronStudioMgrProxy stdMgrProxy(&app);
    stdMgrProxy.populateDemoStudio();
    stdMgrProxy.startDiscovery();

    QObject::connect(&app, &QApplication::applicationStateChanged, StudioMgr::getInstance().data(), &StudioMgr::onApplicationStateChanged);

    QExplicitlySharedDataPointer<NetworkScanner> netScanner = NetworkScanner::getInstance();
    auto networkTree = TreeModel::getInstance();
    auto interfaces = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface i, interfaces){
        if(!i.flags().testFlag(QNetworkInterface::IsLoopBack) && i.flags().testFlag(QNetworkInterface::IsUp) && i.flags().testFlag(QNetworkInterface::CanBroadcast)){
            if(!i.humanReadableName().isEmpty()){
                networkTree->appendChild(std::make_shared<NetworkProxy>(std::make_shared<QNetworkInterface>(i)));
            }
        }
    }
    QQmlApplicationEngine engine;
    QQmlFileSelector selector(&engine);
    engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());
    engine.rootContext()->setContextProperty("studioMgr", &stdMgrProxy);
    engine.rootContext()->setContextProperty("netScanner",netScanner.data());
    engine.rootContext()->setContextProperty("networks", networkTree.get());

    qmlRegisterType<ParaProxy>("ch.bron.BronModels",1,0,"ParaProxy");
    qmlRegisterType<StudioProxy>("ch.bron.BronModels",1,0,"StudioProxy");
    qmlRegisterType<DeviceProxy>("ch.bron.BronModels",1,0,"DeviceProxy");
    qmlRegisterType<BronListModel>("ch.bron.BronModels",1,0,"BronListModel");
    qmlRegisterType<Resolution>("ch.bron.Resolution",1,0,"ResolutionControl");
    qmlRegisterType<TextFieldDoubleValidator>("TextFieldDoubleValidator", 1,0,
                                                  "TextFieldDoubleValidator");
    // register some uncreatable types for enum access
    qmlRegisterUncreatableType<ParaSelects>("ch.bron.BronModels",1,0,"ParaSelectors", "only for enums");
    qmlRegisterSingletonType<GeneralInfo>("ch.bron.Globals", 1, 0, "Globals", globalsProvider);
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));
#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
#endif

    int ret = app.exec();
    return ret;
}

#else


#include <QCoreApplication>
#include <QScreen>
#include <QtGlobal>
#include <QObject>
#include <QQueue>

#include <QHostAddress>
#include <QNetworkInterface>
#include "NetWorkScanner.h"
#include "devicesirros.h"
#include "bronstudio.h"
#include "studiomgr.h"
#include "remoteaddressinfo.h"


using namespace Framework;


void OnNewDeviceUp(std::weak_ptr<Device> dev){
    if(auto d = dev.lock()){
        qDebug() << "fire test flash";
        d->setTest();
        d->getPara(DeviceSiros::ENERGY_TOTAL_APPERTURE)->setRemote(5.0);
        d->setTest();
        d->getPara(DeviceSiros::ENERGY_TOTAL_APPERTURE)->setRemote(4.0);
    }
}


void StudioAdded(const BronStudio *s){
    BronStudio *bs = const_cast<BronStudio *>(s);
    if(bs){
        qDebug() << "studio Added " << bs->Ssid() << " " << bs->getAddress();
        bs->setPolling(false);

        QObject::connect(bs, &BronStudio::newDeviceUp, OnNewDeviceUp);


        QListIterator<std::shared_ptr<Device> > iter(bs->getDevices());
        qDebug() << "get studio devices ";
        while(iter.hasNext()){
            auto dev = iter.next();
            qDebug() << dev->getName() << " " << dev->getHostAddress().toString();
            dev->setTest();
        }
    }
}


int main(int argc, char *argv[]){
    QCoreApplication app(argc, argv);

    qRegisterMetaType<RemoteAddressInfo>();
    qRegisterMetaType<Request *>();
    // give qRegisterMetaType a string as name, otherwise it is not found
    qRegisterMetaType<std::weak_ptr<Framework::Device> >("std::weak_ptr<Device>");
    qRegisterMetaType<std::shared_ptr<Framework::Device> >("std::shared_ptr<Device>");
    qRegisterMetaType<std::weak_ptr<Framework::DevicePara> >("std::weak_ptr<DevicePara>");
    qRegisterMetaType<std::shared_ptr<Framework::DevicePara> >("std::shared_ptr<DevicePara>");
    qRegisterMetaType<std::shared_ptr<Framework::StudioSequence> >("std::shared_ptr<StudioSequence>");


    // get studio Mgr with disabled autoNet search
    auto smgr = StudioMgr::getInstance();
    // starts the discovery process on all valid interface channels
    smgr->startDiscovery();


    QObject::connect(smgr.data(), &StudioMgr::studioAdded, StudioAdded);
    return app.exec();

}


#endif
