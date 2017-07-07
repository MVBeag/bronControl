#ifndef BRONLISTMODELTEMPLATE_H
#define BRONLISTMODELTEMPLATE_H

#include <QAbstractListModel>

template<class T>
class BronListModelTemplate : public QAbstractListModel{
public:
    explicit BronListModelTemplate(QObject *parent = nullptr);
    explicit BronListModelTemplate(const BronListModelTemplate<T> &m);
    virtual ~BronListModelTemplate();

    // QAbstactListModel reimplemented methods
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool insertRows(int row, QList<T *>&insertlist, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void append(T * item);
    QVariant get(int index);
    void clear();

    inline T *getItem(int index) const;

signals:
    void countChanged(int newCount);

protected:
    T m_prototype;
    QList<T *> m_list;
};


#endif // BRONLISTMODEL_H
