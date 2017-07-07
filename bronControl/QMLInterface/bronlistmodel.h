#ifndef BRONLISTMODEL_H
#define BRONLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMutex>
#include <QMutexLocker>
#include "bronitemapi.h"
#include <limits>

namespace QMLInterface{


class BronListModel : public QAbstractListModel{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int element READ getElement NOTIFY elementChanged)
public:
    explicit BronListModel(QString s = QString("copy"), QObject *parent = nullptr);
    explicit BronListModel(QString s, BronItemAPI *proto, QObject *parent = nullptr);
    explicit BronListModel(const BronListModel &m);
    virtual ~BronListModel();

    // QAbstactListModel reimplemented methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int getElement(const QModelIndex &parent = QModelIndex()) const;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


    int insert(std::shared_ptr<BronItemAPI> item, bool changeIxs = true, bool changeLampIx = true);
    /**
     * @brief changedIndexes function to change persistent indexes
     * @param ixChanged
     */
    virtual void changedIndexes(int startChange);
    virtual void changedLampIndexes(int startChange);



    Q_INVOKABLE QVariant get(int index);
    Q_INVOKABLE QVariant get(int index, QString role);
    Q_INVOKABLE QVariant getAll(QString role);
    Q_INVOKABLE QVariant getMin(QString role);
    Q_INVOKABLE QVariant getMax(QString role);
    Q_INVOKABLE void callService(QString name, QVariant data);
    Q_INVOKABLE bool areSame(QString role);
    Q_INVOKABLE bool set(int index, QString role, const QVariant &val);
    Q_INVOKABLE bool setAll(QString role, const QVariant &val);
    Q_INVOKABLE void clear();
    std::shared_ptr<BronItemAPI> removeItemAt(int index,bool xcDevIx, bool xcLampIx);
    Q_INVOKABLE void removeAt(int index);
    Q_INVOKABLE void runService(int index, QString name, QVariant data);
    Q_INVOKABLE void setOnline(int index, bool val);

    /**
     * @brief getSize return the number of items in m_list
     * @return
     */
    int getSize()const;

    /**
     * @brief getItem returns the item at m_list[index]
     * @param index
     * @return  pointer to stored item in case of success, nullptr in case of fail (index to large)
     */
    std::shared_ptr<BronItemAPI> getItem(int index) const;
    QListIterator<std::shared_ptr<BronItemAPI> > getListIterator() const;
    QMutableListIterator<std::shared_ptr<BronItemAPI> > getMutableListIterator();


    bool contains(std::shared_ptr<BronItemAPI> it);
    int remove(std::shared_ptr<BronItemAPI> it, bool xcDevIx, bool xcLampIx);
    /**
     * @brief indexOf returns the index of it in the list or -1 in case of error
     * @param it
     * @return
     */
    int indexOf(std::shared_ptr<BronItemAPI> it) const;

public slots:
    void dataChanged(int ix, QVector<int> what);
    void onElementChanged(int ix);

signals:
    void countChanged(int newCount);
    void elementChanged(int element);

protected:
    mutable QMutex m_mutex;
    BronItemAPI *m_prototype;
    QList<std::shared_ptr<BronItemAPI> > m_list;
    int m_element;
};

inline int BronListModel::getElement(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return m_element;
}

inline void BronListModel::runService(int index, QString name, QVariant data){
    QMutexLocker lock(&m_mutex);
    auto it = getItem(index);
    if(it) it->runService(name, data);
}
inline void BronListModel::setOnline(int index, bool val){
    QMutexLocker lock(&m_mutex);
    auto it = getItem(index);
    if(it) it->setOnline(val);
}
inline int BronListModel::getSize()const{
    QMutexLocker lock(&m_mutex);
    return m_list.size();
}
inline QListIterator<std::shared_ptr<BronItemAPI> > BronListModel::getListIterator() const{
    QMutexLocker lock(&m_mutex);
    return QListIterator<std::shared_ptr<BronItemAPI> >(m_list);
}

inline QMutableListIterator<std::shared_ptr<BronItemAPI> > BronListModel::getMutableListIterator(){
    QMutexLocker lock(&m_mutex);
    return QMutableListIterator<std::shared_ptr<BronItemAPI> >(m_list);
}



inline void BronListModel::dataChanged(int ix, QVector<int> what){
    QMutexLocker lock(&m_mutex);
    QAbstractItemModel *i = dynamic_cast<QAbstractItemModel *>(this);
    if(i){
        emit i->dataChanged(index(ix,0), index(ix,0), what);
    }
}

inline void BronListModel::onElementChanged(int ix){
    QMutexLocker lock(&m_mutex);
    m_element = ix;
    emit elementChanged(ix);
}

}

Q_DECLARE_METATYPE(QMLInterface::BronListModel *)
Q_DECLARE_METATYPE(QMLInterface::BronListModel)

#endif // BRONLISTMODEL_H
