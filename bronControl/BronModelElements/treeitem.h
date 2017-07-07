/**
  * @abstract TreeItem is the data representation of the treeModel
  * it contails a QList of children, each children is a treeItem itself and has its own representation
  */
#ifndef TREEITEM_H
#define TREEITEM_H

#include <memory>
#include <QList>
#include <QVariant>
#include <QVector>

namespace Framework {

class TreeItem: public std::enable_shared_from_this<TreeItem>{
public:
    explicit TreeItem(std::weak_ptr<TreeItem> parent);
    ~TreeItem();

    std::weak_ptr<TreeItem> child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int role) const;
    bool insertChildren(int position, int count);
    std::shared_ptr<TreeItem> parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
private:
    std::shared_ptr<TreeItem> m_parentItem;
    QList<std::shared_ptr<TreeItem> > m_childItems;
};

inline std::weak_ptr<TreeItem> TreeItem::child(int number){
    return m_childItems.value(number);
}

inline int TreeItem::childCount() const{
    return m_childItems.count();
}

inline int TreeItem::columnCount() const{
    return 0;
}

}
#endif // TREEITEM_H
