#ifndef TREEMODEL_H
#define TREEMODEL_H


#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <memory>
#include "bronitemapi.h"


using namespace QMLInterface;

namespace Framework{


class TreeModel : public QAbstractItemModel{
public:
    static std::shared_ptr<TreeModel> getInstance();

    ~TreeModel();


    TreeModel(const TreeModel &) = delete;



    QVariant data(const QModelIndex &index, int role) const override;
//    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool appendChild(std::shared_ptr<BronItemAPI> child, const QModelIndex &parent = QModelIndex());

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

//    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) override;
//    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) override;
//    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
//    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

private:
    TreeModel();
    std::shared_ptr<BronItemAPI> getItem(const QModelIndex &index) const;
    std::shared_ptr<BronItemAPI> m_rootItem;
};

}

#endif // TREEMODEL_H
