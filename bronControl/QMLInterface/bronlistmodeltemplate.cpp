#include "bronlistmodeltemplate.h"


template<class T>
BronListModelTemplate<T>::BronListModelTemplate(QObject *parent)
    : QAbstractListModel(parent)
    , m_prototype()
    , m_list(){}

template<class T>
BronListModelTemplate<T>::BronListModelTemplate(const BronListModelTemplate<T> &m)
    : QAbstractListModel(m.parent())
    , m_prototype(m.m_prototype)
    , m_list(m.m_list){}

template<class T>
BronListModelTemplate<T>::~BronListModelTemplate(){
    clear();
}

template<class T>
int BronListModelTemplate<T>::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return m_list.size();
}


template<class T>
QVariant BronListModelTemplate<T>::data(const QModelIndex &index, int role) const{
    if(index.row() < 0) { return QVariant();}
    if(index.row() > m_list.size()){ return QVariant();}
    T * it = m_list.at(index.row());
    if(it){return it->getData(role);}
    return QVariant();
}

template<class T>
QHash<int, QByteArray> BronListModelTemplate<T>::roleNames() const{
    return m_prototype.roleNames();
//    if(m_prototype){return m_prototype->roleNames();}
//    return QHash<int, QByteArray>();
}

template<class T>
bool BronListModelTemplate<T>::insertRows(int row, QList<T *> &insertlist, const QModelIndex &parent){
    if(row < 0){return false;}
    if(insertlist.size() == 0){return false;}
    beginInsertRows(parent, row, row + insertlist.size() - 1);
    // insert backward to preserve order :-)
    for (auto it = insertlist.rbegin(); it != insertlist.rend(); ++it){
        m_list.insert(row, *it);
    }
    endInsertRows();
    emit countChanged(m_list.size());
    return true;
}

template<class T>
bool BronListModelTemplate<T>::removeRows(int row, int count, const QModelIndex &parent){
    if(row < 0) { return false;}
    if(count == 0){ return false;}
    if((row + count) > m_list.size()){ return false;}
    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++){
        BronItemAPI * item = m_list.takeAt(row);
        item->deleteLater();
        item = nullptr;
    }
    endRemoveRows();
    emit countChanged(rowCount());
    return true;
}

template<class T>
void BronListModelTemplate<T>::append(T *item){
    if(item){
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_list.append(item);
        endInsertRows();
        emit countChanged(m_list.size());
    }
}

template<class T>
QVariant BronListModelTemplate<T>::get(int index){
    if(index < 0){ return QVariant();}
    if(index > m_list.size()){ return QVariant();}
    QMap<QString, QVariant> itemData;
    T* pItem = m_list.at( index );
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

template<class T>
void BronListModelTemplate<T>::clear(){
    if(m_list.size() > 0){
        removeRows(0,m_list.size());
    }
}

template<class T>
T *BronListModelTemplate<T>::getItem(int index) const{
    if(m_list.size() > index){
        return m_list.at(index);
    }
    return nullptr;
}
