#ifndef NETWORKSCANNER
#define NETWORKSCANNER

#include <QObject>
#include <QVariant>
#include <QtNetwork>
#include <QTimer>
#include <QAbstractListModel>
#include <QNetworkConfiguration>
namespace Framework{

class BronStudio;

/**
 * @brief The NetworkScanner class scans for available networks, networks are valid if the network is of type wifi,
 * if the name is bron-studio, the network is also valid, due to the fact, that the user can change networks,
 * other wifi networks ars shown!
 *
 * NetworkScanner is a singleton, one scanner per application!
 */
class NetworkScanner : public QAbstractListModel, public QSharedData{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    public:

        enum class eSettings : int{
            rescanTime = 10000      // ms
        };
        Q_ENUM(eSettings)

        /**
         * @enum NetworkScanner::NetworkRoles, used to select roles by HMI
         * @brief The NetworkRoles enum used in QML to get the networks as List Model
         */
        enum NetworkRoles{
            Name = Qt::UserRole + 1, /**< select the name property */
        };

        /**
         * @brief getInstance singelton pattern
         * @return
         */
        static QExplicitlySharedDataPointer<NetworkScanner> getInstance();

        /**
         * @brief ~NetworkScanner constructor, destroys all internal data and the m_instance object
         */
        virtual ~NetworkScanner();

        /**
         * @brief data access function for QML
         * @param index list index to access
         * @param role role to get (see NetworkRoles)
         * @return QVariant of the data, in case of not found, an invalid QVariant is returend
         */
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

        QHash<int, QByteArray> roleNames() const;
        /**
         * @brief rowCount return the number of elements in the list
         * @param parent
         * @return
         */
        int rowCount(const QModelIndex &parent = QModelIndex()) const;

        /**
         * @brief isAutoConnectPossible
         * @return return true, if the underlying operating system allows starting and stopping networks
         */
        bool isAutoConnectPossible() const;

        /**
         * @brief scan starts manually a new network scan
         */
        void scan();

        /**
         * @brief getNetworks get all known networks
         * @return
         */
        QList<QNetworkConfiguration> getNetworks()const;

        /**
         * @brief getNetworksByName returns known configurations with the given name
         * @param name of the desired QNetworkConfiguration
         * @return List of QNetworkConfigurations
         */
        QList<QNetworkConfiguration> getNetworksByName(QString name) const;

        QNetworkConfiguration defaultConfig() const;

        QNetworkConfigurationManager *getNetworkCfgMgr() const;

    signals:

        /**
         * @brief netInserted signal is emitted if a new network was detected
         * @param n
         */
        void netInserted(const QNetworkConfiguration &n);

        /**
         * @brief netRemoved signal is emitted if a known network isn't seen anymore
         * @param n
         */
        void netRemoved(const QNetworkConfiguration &n);

        /**
         * @brief connectionError signal is emitted, if a connection attempt fail
         * @param ssid of network, were error occours
         * @param reason
         */
        void connectionError(const QString ssid, const QString reason);

        /**
         * @brief firstScanComplete called after the first scan is complete
         */
        void firstScanComplete();

        /**
         * @brief netConfigChanged is emitted if a known networkConfiguration has a state change
         * @param cfg
         */
        void netConfigChanged(const QNetworkConfiguration cfg);

        /**
         * @brief countChanged emitted if the number of networks has changed
         * @param newCount
         */
        void countChanged(int newCount);
    private:
        QNetworkConfigurationManager *m_mgr;
        QNetworkConfiguration m_desiredCfg;
        QList<QNetworkConfiguration> m_discoveredNets;
        int m_idxToRemove;
//        QTimer m_scanTimer;

        NetworkScanner(QObject *parent = NULL);



        /**
         * @brief insertRows insert Rows to the List called by NetAdded, keeps the GUI clean
         * @param row   where to insert
         * @param count how many to insert
         * @param parent
         * @return
         */
        bool insertMyRows(int row, int count, QNetworkConfiguration cfg, const QModelIndex & parent = QModelIndex());

        /**
         * @brief removeRows remove Rows from the list, keeps the GUI clean
         * @param cfg   QNetworkConfiguration cfg to remove
         * @param parent
         * @return
         */
        bool removeMyRows(QNetworkConfiguration cfg, const QModelIndex & parent = QModelIndex());

        bool insertMyRow(int arow, QNetworkConfiguration cfg, const QModelIndex &aparent = QModelIndex()){
            return insertMyRows(arow, 1, cfg, aparent);
        }



private slots:
        /**
         * @brief UpdateNets checks if the new network was in m_discoveredNets
         */
        void UpdateNets();


//        /**
//         * @brief TimerUpdateNets after the first scan is complete, we initiate a timer based check of networks, to check
//         * the system is only informing about changes, to keep the studio list up to date, we use these timer based update here
//         */
//        void TimerUpdateNets();

        /**
         * @brief NetRemoved called on configurationRemoved signal
         * @param cfg
         */
        void NetRemoved(const QNetworkConfiguration &cfg);

        /**
         * @brief NetAdded called on configuration added signal
         * @param cfg
         */
        void NetAdded(const QNetworkConfiguration &cfg);

        /**
         * @brief NetConfigChanged signals a change in a network config
         * @param cfg
         */
        void NetConfigChanged(const QNetworkConfiguration &cfg);

};

inline QNetworkConfigurationManager *NetworkScanner::getNetworkCfgMgr() const{
    return m_mgr;
}


}

#endif // NETWORKSCANNER
