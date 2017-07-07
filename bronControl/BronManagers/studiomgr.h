#ifndef STUDIOMGR_H
#define STUDIOMGR_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QNetworkConfiguration>
#include <QSharedData>
#include <QExplicitlySharedDataPointer>
#include "NetWorkScanner.h"
#include "deviceFinder.h"
#include "networkapi_impl.h"
#include "device.h"


namespace Framework{

class BronStudio;
/**
 * @brief The StudioMgr class manages the studios, stored or found
 */
class StudioMgr : public QObject, public QSharedData{
    Q_OBJECT
public:

    enum class settings : int {
        discoveryTime = 10000 /**< rescan time for discovery in ios and android */
    };


    static QExplicitlySharedDataPointer<StudioMgr> getInstance();
    ~StudioMgr();

    /**
     * @brief addStudio adds a studio to the mananger
     * @param s
     * @return true if added successful, else false
     */
    bool addStudio(BronStudio *s);

    /**
     * @brief removeStudio removes a studio from the manager
     * @param s
     * @return true if removed successful, else false
     */
    bool removeStudio(BronStudio *s);

   /**
    * @brief Name retrieves the name of the module active yet
    * @return
    */
   virtual QString Name()const;

   /**
    * @brief classType returns the class type as QString. This is a kind of reflection
    * @return
    */
   virtual QString classType() const;

    /**
    * @brief hasStudio checks if a studio is known
    * @param name
    * @return true if known, else false
    */
   bool hasStudio(const QString &name) const;

   /**
    * @brief getStudios return the studio with parameter name, if not found, nullptr is returned
    * @param name name of the studio
    * @return a list of the studios with this name if there are some, else nullptr
    */
   QList<BronStudio *> getStudios(const QString &name);

   /**
    * @brief getNumberOfStudios returns the number or known studios
    * @return
    */
   int getNumberOfStudios() const;

   /**
    * @brief setOffline if val == true, alle known studios and devices will go to offline (stop communicatin)
    * @param val
    */
   void setOffline(bool val);

   /**
    * @brief autoConnectPossible checks if the underlying os allows to start and stop networks within applications
    * @return
    */
   bool autoConnectPossible() const;

   /**
    * @brief getStudios
    * @return all known studios
    */
   QMultiMap<QString, BronStudio *> *getStudios();


   /**
    * @brief getDeviceFinder get the finder, this is the item, looking for new devices
    * @return
    */
   DeviceFinder *getDeviceFinder();


   /**
    * @brief CreateConnection creates and open an udp connection on given port
    * @param localPort port to open
    * @return pointer of connection or nullptr in case of failure
    */
   std::weak_ptr<NetworkAPI> CreateConnection(int localPort,  std::shared_ptr<Device> dev);

   /**
    * @brief getNetAccess
    * @return returns qtNetworkIntegration, the network management of this application
    */
   std::weak_ptr<NetworkAPI> getNetAccess();


   /**
    * @brief activateStudio activates studio and starts scan for devices
    * @param studio to activate
    * @param activate indicate to set online(true) or offline (false)
    * @return true if activated, false if not (e.g. studio not known)
    */
   bool activateStudio(bool activate, BronStudio *studio);

   /**
    * @brief createStudio by given ssid, studioAddress and pw
    * @param ssid   networkSSID
    * @param stdioAddr wifi address of new studio
    * @param pw passworkd
    * @param encr encryption Mode
    * @return true if new studio was created, false if not (e.g. studio already exists)
    */
   bool createStudio(QString ssid, int studioAddr, QString pw, int encr);

   /**
    * @brief populateDemoStudio creates the demo studio
    */
   void populateDemoStudio();

   /**
    * @brief getBackgroundUpdate return the state of the background update
    * @return true if enabled
    */
   bool getBackgroundUpdate() const;

   /**
    * @brief setBackgroundUpdate sets the state of the background update
    * @param val
    */
   void setBackgroundUpdate(bool val);

   /**
    * @brief setUserScale set the user defined scale
    * @param scale
    */
   void setUserScale(const float scale);

   /**
    * @brief getUserScale returns the user defined scale
    * @return
    */
   float getUserScale() const;

   /**
    * @brief freeze stops everything behind (devicetreads, communication, ...)
    */
   void freeze();

   /**
    * @brief unfreeze restarts everything behind (device threads, communication, ...)
    */
   void unfreeze();

   /**
    * @brief storeSettings stores the actual studio settings to disc
    */
   void storeSettings();

   /**
    * @brief readSettings reads the actual studio settings from disc
    */
   void readSettings();


 public slots:

   /**
    * @brief netAdded called if a net network was detected by the system. If the configuration name
    * contains "Bron-Studio", a studio is created and added automatically
    * @param n
    */
   void netAdded(const QNetworkConfiguration &n);

   /**
    * @brief netRemoved is called, if network was removed from the systems known network configuraions
    * if one of the registered studios uses this configuration and is online, the studio is switched offline
    * @param
    */
   void netRemoved(const QNetworkConfiguration &n);

   /**
    * @brief checkAndInsertPossibleStudios after a network scan has finished, we insert all studios
    * with the name "Bron-Studio" automatically to the known studios list
    */
   void checkAndInsertPossibleStudios();

   /**
    * @brief newIdentifiedDevice called if a new initialized device was found
    * @param dev the device has no parent, take care about this. The ownership
    * is not defined. The intention is to give the ownership to the studio,
    * the device is belonging
    */
   void newIdentifiedDevice(std::weak_ptr<Device> d);

   /**
    * @brief startResolving starts the discovery process for connected items
    * @param studio to discover
    */
   void startDiscovery(BronStudio *studio = nullptr);

   /**
    * @brief stopResolving stops the dicovery process
    * @param studio to undiscover
    */
   void stopDiscovery(BronStudio *studio = nullptr);

   /**
    * @brief onNetConfigChanged emitted if the network configuration cfg is changed
    * @param cfg
    */
   void onNetConfigChanged(const QNetworkConfiguration cfg);
#if defined Q_OS_IOS
        /**
         * @brief onDiscoveryTimeout retriggers an start/stop of discoverey, on IOS and Android, no networkChanged signal
         * is emmitted on a network change in WIFI (Qt 5.8). This slot is used to emulate that behaviour
         */
        void onDiscoveryTimeout();
        
#elif defined Q_OS_ANDROID
   /**
    * @brief onDiscoveryTimeout retriggers an start/stop of discoverey, on IOS and Android, no networkChanged signal
    * is emmitted on a network change in WIFI (Qt 5.8). This slot is used to emulate that behaviour
    */
   void onDiscoveryTimeout();
#endif

   /**
    * @brief onApplicationStateChanged handles QGuiApplication::applicationStateChanged signals
    * @param state
    */
   void onApplicationStateChanged(Qt::ApplicationState state);

   /**
    * @brief onSwVersionRead called if device has read the sw version, can be used to handle version dependent functionality
    * @param dev
    */
   void onSwVersionRead(std::shared_ptr<Device> dev);

signals:
   void studioAdded(const BronStudio* studio);
   void studioRemoved(const BronStudio *studio);

   /**
    * @brief networkScanFinished signal is emitted if the first scan for networks has finished
    */
   void networkScanFinished();

   void netConfigChanged(const QNetworkConfiguration cfg);
   void userScaleChanged(const float userScale);
   void deviceSwVersionRead(std::shared_ptr<Device> dev);

private:
    StudioMgr(QObject *parent = nullptr);
    QMutex m_mutex;
    QMultiMap<QString, BronStudio*> m_studios;          /**< has to be a multimap, in enterprize mode, there can be more than one studio in the same enterprize network */
    QExplicitlySharedDataPointer<NetworkScanner> m_netScanner;
    DeviceFinder m_deviceFinder;
    std::shared_ptr<NetworkAPI> m_netAccess;
    QList<QNetworkAddressEntry> m_knownScanAddresses;
    bool m_backgroundUpdate;
#if defined Q_OS_IOS
    QTimer m_discoveryTimer;
#elif defined Q_OS_ANDROID
    QTimer m_discoveryTimer;
#endif
    float m_userScale;
};



inline DeviceFinder *StudioMgr::getDeviceFinder(){
    return &m_deviceFinder;
}

inline std::weak_ptr<NetworkAPI> StudioMgr::getNetAccess(){
    return m_netAccess;
}

inline bool StudioMgr::getBackgroundUpdate() const{
    return m_backgroundUpdate;
}

inline void StudioMgr::setBackgroundUpdate(bool val){
    m_backgroundUpdate = val;
}

inline void StudioMgr::setUserScale(const float scale){
    if(scale != m_userScale){
        m_userScale = scale;
        emit userScaleChanged(m_userScale);
    }
}

inline float StudioMgr::getUserScale() const{
    return m_userScale;
}

inline void StudioMgr::onSwVersionRead(std::shared_ptr<Device> dev){
    emit deviceSwVersionRead(dev);
}

using SharedStudioMgr = QExplicitlySharedDataPointer<StudioMgr>;


}

#endif // STUDIOMGR_H
