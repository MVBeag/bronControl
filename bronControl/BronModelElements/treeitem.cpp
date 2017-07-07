#include "treeitem.h"
#include <QStringList>

using namespace Framework;

TreeItem::TreeItem(std::weak_ptr<TreeItem> parent)
    : m_parentItem(parent)
    , m_childItems(){}

TreeItem::~TreeItem(){}





int TreeItem::childNumber() const{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(std::const_pointer_cast<TreeItem>(shared_from_this()));

    return 0;
}

QVariant TreeItem::data(int role) const{
    Q_UNUSED(role)
    return QVariant(); //itemData.value(column);
}

bool TreeItem::insertChildren(int position, int count){
    if (position < 0 || position > m_childItems.size()){
        return false;
    }

    for (int row = 0; row < count; ++row) {
        auto item = std::make_shared<TreeItem>(shared_from_this());
        m_childItems.insert(position, item);
    }

    return true;
}

std::shared_ptr<TreeItem> TreeItem::parent(){
    return m_parentItem;
}

bool TreeItem::removeChildren(int position, int count){
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        m_childItems.takeAt(position);

    return true;
}

