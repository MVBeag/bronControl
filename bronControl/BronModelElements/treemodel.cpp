#include "treemodel.h"
#include "bronitemapi.h"
#include "treerootproxy.h"

using namespace Framework;
using namespace QMLInterface;

Framework::TreeModel::TreeModel()
    : QAbstractItemModel(){
    m_rootItem = std::make_shared<TreeRootProxy>();
}

std::shared_ptr<TreeModel> TreeModel::getInstance(){
    static std::shared_ptr<TreeModel> gsptr(new TreeModel());
    return gsptr;
}

Framework::TreeModel::~TreeModel(){}

int Framework::TreeModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return 1;
}

bool Framework::TreeModel::appendChild(std::shared_ptr<BronItemAPI> child, const QModelIndex &parent){
    if(auto parentItem = getItem(parent)){
        beginInsertColumns(parent, m_rootItem->childCount(), m_rootItem->childCount());
        parentItem->insertChild(m_rootItem->childCount(), child);
        endInsertColumns();

        return true;
    }
    return false;

}

QVariant Framework::TreeModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid()){
        return QVariant();
    }

    if(auto item = getItem(index)){
        return item->getData(role);
    }
    else{
        return QVariant();
    }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const{
    if (!index.isValid()){
        return 0;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

std::shared_ptr<BronItemAPI> TreeModel::getItem(const QModelIndex &index) const{
    if (index.isValid()) {
        if(auto item = std::const_pointer_cast<BronItemAPI>((static_cast<BronItemAPI *>(index.internalPointer()))->shared_from_this())){
            return item;
        }
    }
    return m_rootItem;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const{
    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    if (parent.isValid() && parent.column() != 0){
        return QModelIndex();
    }


    if(auto parentItem = getItem(parent)){
        if(auto childItem = parentItem->child(row).lock()){
            return createIndex(row, column, childItem.get());
        }
    }
    return QModelIndex();
}

//bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent){
//    if(auto parentItem = getItem(parent).lock()){
//        bool success;

//        beginInsertRows(parent, position, position + rows - 1);
//        success = parentItem->insertChildren(position, rows);
//        endInsertRows();

//        return success;
//    }
//    return false;
//}

QModelIndex TreeModel::parent(const QModelIndex &child) const{
    if (!child.isValid()){
        return QModelIndex();
    }

    if(auto childItem = getItem(child)){
        if(auto parentItem = childItem->getParent()){
            if (parentItem == m_rootItem){
                return QModelIndex();
            }
            return createIndex(parentItem->row(), 0, parentItem.get());
        }
    }
    return QModelIndex();
}

//bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent){
//    if(auto parentItem = getItem(parent).lock()){
//        bool success = true;

//        beginRemoveRows(parent, position, position + rows - 1);
//        success = parentItem->removeChildren(position, rows);
//        endRemoveRows();

//        return success;
//    }
//    return false;
//}

int TreeModel::rowCount(const QModelIndex &parent) const{
    if(!parent.isValid()){
        return m_rootItem->childCount();
    }
    if(parent.column() > 0){
        return 0;
    }

    if(auto parentItem = getItem(parent)){
        return parentItem->childCount();
    }
    return 0;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if (role != Qt::EditRole)
        return false;

    bool result = false;
    if(auto item = getItem(index)){
        result = item->setData(index.column(), value);
        if (result){
            emit dataChanged(index, index);
        }
    }

    return result;
}
