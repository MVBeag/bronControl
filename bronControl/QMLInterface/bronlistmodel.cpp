#include "bronlistmodel.h"
#include <QDebug>
#include <QQmlEngine>
#include <QMutexLocker>

using namespace QMLInterface;

QMLInterface::BronListModel::BronListModel(QString s, QObject *parent)
    : QAbstractListModel(parent)
    , m_mutex(QMutex::Recursive)
    , m_prototype(nullptr)
    , m_list()
    , m_element(-1){
    setObjectName(s);
}

QMLInterface::BronListModel::BronListModel(QString s, BronItemAPI *proto, QObject *parent)
    : QAbstractListModel(parent)
    , m_mutex(QMutex::Recursive)
    , m_prototype(proto)
    , m_list()
    , m_element(-1){
    setObjectName(s);
}

BronListModel::BronListModel(const BronListModel &m)
    : QAbstractListModel(m.parent())
    , m_mutex(QMutex::Recursive)
    , m_prototype(m.m_prototype)
    , m_list(m.m_list)
    , m_element(m.m_element){
//        setObjectName(m.parent()->objectName());
    }

QMLInterface::BronListModel::~BronListModel(){
//    clear();
}

int QMLInterface::BronListModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    QMutexLocker lock(&m_mutex);
    return m_list.size();
}

QVariant QMLInterface::BronListModel::data(const QModelIndex &index, int role) const{
    if(index.row() < 0) { return QVariant();}
    QMutexLocker lock(&m_mutex);
    if(index.row() > m_list.size()){ return QVariant();}
    auto it = m_list.at(index.row());
    if(it){return it->getData(role);}
    return QVariant();
}

bool BronListModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(index.row() < 0) return false;
    QMutexLocker lock(&m_mutex);
    if(index.row() > m_list.size()) return false;
    auto it = m_list.at(index.row());
    if(it){
        if(it->setData(role, value)){
            return true;
        }
    }
    return false;
}

QHash<int, QByteArray> QMLInterface::BronListModel::roleNames() const{
    if(m_prototype){return m_prototype->roleNames();}
    return QHash<int, QByteArray>();
}


bool QMLInterface::BronListModel::removeRows(int row, int count, const QModelIndex &parent){
    if(row < 0) { return false;}
    if(count == 0){ return false;}
    QMutexLocker lock(&m_mutex);
    if((row + count) > m_list.size()){ return false;}
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++){
        m_list.takeAt(row);
    }
    changedIndexes(0);
    endRemoveRows();
    emit countChanged(rowCount());
    return true;
}

int QMLInterface::BronListModel::insert(std::shared_ptr<BronItemAPI> item, bool changeIxs, bool changeLampIx){
    if(item){
        QMutexLocker lock(&m_mutex);
        // search position in list
        if(m_list.size() == 0){
            QModelIndex mi;
            beginInsertRows(mi, 0, 0);
            m_list.append(item);
            if(changeIxs){
                changedIndexes(0);
            }
            if(changeLampIx){
                changedLampIndexes(0);
            }
            endInsertRows();
            emit countChanged(m_list.size());
            return 0;
        }
        else{
            int lix = 0;
            bool isInserted = false;
            QListIterator<std::shared_ptr<BronItemAPI> >iter(m_list);
            while(iter.hasNext()){
                auto api = iter.next();
                if(*item > *api){
                    isInserted = true;
                    QModelIndex mi;
                    beginInsertRows(mi, lix, lix);
                    m_list.insert(lix, item);
                    if(changeIxs){
                        changedIndexes(lix);
                    }
                    if(changeLampIx){
                        changedLampIndexes(lix);
                    }
                    endInsertRows();
                    emit countChanged(m_list.size());
                    return lix;
                }
                lix++;
            }
            if(!isInserted){
                QModelIndex mi;
                beginInsertRows(mi, lix, lix);
                m_list.append(item);
                if(changeIxs){
                    changedIndexes(lix);
                }
                if(changeLampIx){
                    changedLampIndexes(lix);
                }
                endInsertRows();
                emit countChanged(m_list.size());
                return lix;
            }
        }
    }
    return -1;
}

void QMLInterface::BronListModel::changedIndexes(int startChange){
    // ixChanged denotes the change index in list
    QMutexLocker lock(&m_mutex);
    int ix = 0;
    QListIterator<std::shared_ptr<BronItemAPI> >iter(m_list);
    while(iter.hasNext()){
        auto item = iter.next();
        if(ix >= startChange){
            item->changeIndex(ix);
        }
        ix++;
    }
}

void QMLInterface::BronListModel::changedLampIndexes(int startChange){
    // ixChanged denotes the change index in list
    QMutexLocker lock(&m_mutex);
    int ix = 0;
    QListIterator<std::shared_ptr<BronItemAPI> >iter(m_list);
    while(iter.hasNext()){
        auto item = iter.next();
        if(ix >= startChange){
            item->changeLampIx(ix);
        }
        ix++;
    }
}



QVariant QMLInterface::BronListModel::get(int index){
    QMutexLocker lock(&m_mutex);
    if(index < 0){ return QVariant();}
    if(index > m_list.size()){ return QVariant();}
    QMap<QString, QVariant> itemData;
    auto pItem = m_list.at( index );
    if ( pItem ){
        QHashIterator<int, QByteArray> aRolesItr( pItem->roleNames() );
        while ( aRolesItr.hasNext() ){
            aRolesItr.next();
            itemData.insert( aRolesItr.value(), QVariant( pItem->getData( aRolesItr.key() ) ) );
        }
        return QVariant( itemData );
    }
    return QVariant();
}

QVariant QMLInterface::BronListModel::get(int index, QString role){
    if(index < 0){ return QVariant();}
    QMutexLocker lock(&m_mutex);
    if(index > m_list.size()){ return QVariant();}
    QMap<QString, QVariant> itemData;
    auto pItem = m_list.at( index );
    if ( pItem ){
        QHashIterator<int, QByteArray> i( pItem->roleNames() );
        while(i.hasNext()){
            i.next();
            if(i.value() == role.toUtf8()){
                return pItem->getData(i.key());
            }
        }
    }
    return QVariant();
}

QVariant QMLInterface::BronListModel::getAll(QString role){
    QMutexLocker lock(&m_mutex);
    QListIterator<std::shared_ptr<BronItemAPI> >liter(m_list);
    int key = 0;
    bool isFound = false;
    QList<QVariant> itemData;
    while(liter.hasNext()){
        auto item = liter.next();
        if(!isFound){
            QHashIterator<int, QByteArray> i( item->roleNames() );
            while(i.hasNext()){
                i.next();
                if(i.value() == role.toUtf8()){
                    isFound = true;
                    key = i.key();
                    break;
                }
            }
        }
        if(isFound){
            itemData.append(item->getData(key));
        }
    }
    return QVariant(itemData);
}

QVariant QMLInterface::BronListModel::getMin(QString role){
    QMutexLocker lock(&m_mutex);
    QListIterator<std::shared_ptr<BronItemAPI> >liter(m_list);
    int key = 0;
    bool isFound = false;
    QVariant min(10.0);
    while(liter.hasNext()){
        auto item = liter.next();
        if(!isFound){
            QHashIterator<int, QByteArray> i( item->roleNames() );
            while(i.hasNext()){
                i.next();
                if(i.value() == role.toUtf8()){
                    isFound = true;
                    key = i.key();
                    break;
                }
            }
        }
        if(isFound){
            if(item->getData(key) < min){
                min = item->getData(key);
            }
        }
    }
    if(isFound)
        return min;
    else
        return QVariant();
}

QVariant QMLInterface::BronListModel::getMax(QString role){
    QMutexLocker lock(&m_mutex);
    QListIterator<std::shared_ptr<BronItemAPI> >liter(m_list);
    int key = 0;
    bool isFound = false;
    QVariant max(0.0);
    while(liter.hasNext()){
        auto item = liter.next();
        if(!isFound){
            QHashIterator<int, QByteArray> i( item->roleNames() );
            while(i.hasNext()){
                i.next();
                if(i.value() == role.toUtf8()){
                    isFound = true;
                    key = i.key();
                    break;
                }
            }
        }
        if(isFound){
            if(item->getData(key) > max){
                max = item->getData(key);
            }
        }
    }
    if(isFound)
        return max;
    else
        return QVariant();

}

void QMLInterface::BronListModel::callService(QString name, QVariant data){
    QMutexLocker lock(&m_mutex);
    QListIterator<std::shared_ptr<BronItemAPI> >liter(m_list);
    while(liter.hasNext()){
        liter.next()->runService(name,data);
    }
}

bool QMLInterface::BronListModel::areSame(QString role){
    QMutexLocker lock(&m_mutex);
    QListIterator<std::shared_ptr<BronItemAPI> >liter(m_list);
    int key = 0;
    bool isFound = false;
    QVariant firstFound;
    QList<QVariant> itemData;
    while(liter.hasNext()){
        auto item = liter.next();
        if(!isFound){
            QHashIterator<int, QByteArray> i( item->roleNames() );
            while(i.hasNext()){
                i.next();
                if(i.value() == role.toUtf8()){
                    isFound = true;
                    key = i.key();
                    firstFound = item->getData(key);
                    break;
                }
            }
        }
        if(isFound){
            if(firstFound != item->getData(key)){
                return false;
            }
        }
    }
    return isFound;
}

bool QMLInterface::BronListModel::setAll(QString role, const QVariant &val){
    QMutexLocker lock(&m_mutex);
    QListIterator<std::shared_ptr<BronItemAPI> >liter(m_list);
    int key = 0;
    bool isFound = false;
    while(liter.hasNext()){
        auto item = liter.next();
        if(!isFound){
            QHashIterator<int, QByteArray> i( item->roleNames() );
            while(i.hasNext()){
                i.next();
                if(i.value() == role.toUtf8()){
                    isFound = true;
                    key = i.key();
                    break;
                }
            }
        }
        if(isFound){
            item->setData(key, val);
        }
    }
    return isFound;
}




bool QMLInterface::BronListModel::set(int index, QString role, const QVariant &val){
    if(index < 0){ return false;}
    QMutexLocker lock(&m_mutex);
    if(index > m_list.size()){ return false;}
    if(auto pItem = m_list.at( index )){
        QHashIterator<int, QByteArray> i( pItem->roleNames() );
        while(i.hasNext()){
            i.next();
            if(i.value() == role.toUtf8()){
                return pItem->setData(i.key(),val);
            }
        }
    }
    return false;
}

void QMLInterface::BronListModel::clear(){
    QMutexLocker lock(&m_mutex);
    if(m_list.size() > 0){
        removeRows(0,m_list.size());
    }
}

std::shared_ptr<BronItemAPI> QMLInterface::BronListModel::removeItemAt(int index, bool xcDevIx, bool xcLampIx){
    QMutexLocker lock(&m_mutex);
    if(index >= 0 && index < m_list.size()){
        beginRemoveRows(QModelIndex(),index,index);
        auto item = m_list.takeAt(index);
        if(xcDevIx){
            changedIndexes(index);
        }
        if(xcLampIx){
            changedLampIndexes(index);
        }
        endRemoveRows();
        emit countChanged(rowCount());
        return item;
    }
    return nullptr;
}

void QMLInterface::BronListModel::removeAt(int index){
    QMutexLocker lock(&m_mutex);
    if(index >= 0 && index < m_list.size()){
        beginRemoveRows(QModelIndex(),index,index);
        m_list.takeAt(index);
        changedIndexes(0);
        endRemoveRows();
        emit countChanged(rowCount());
    }
}

std::shared_ptr<BronItemAPI> QMLInterface::BronListModel::getItem(int index) const{
    QMutexLocker lock(&m_mutex);
    if(m_list.size() >= index){
        return m_list.at(index);
    }
    return nullptr;
}

bool QMLInterface::BronListModel::contains(std::shared_ptr<BronItemAPI> it){
    QMutexLocker lock(&m_mutex);
    return m_list.contains(it);
}

int QMLInterface::BronListModel::remove(std::shared_ptr<BronItemAPI> it, bool xcDevIx, bool xcLampIx){
    QMutexLocker lock(&m_mutex);
    if(m_list.contains(it)){
        int index = m_list.indexOf(it);
        if(nullptr != removeItemAt(index, xcDevIx, xcLampIx)){
            return index;
        }
    }
    return -1;
}

int QMLInterface::BronListModel::indexOf(std::shared_ptr<BronItemAPI> it) const{
    QMutexLocker lock(&m_mutex);
    if(m_list.contains(it)){
        return m_list.indexOf(it);
    }
    return -1;
}
