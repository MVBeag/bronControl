#ifndef BRONITEMAPI_H
#define BRONITEMAPI_H

#include <memory>
#include <QObject>
#include <QHash>
#include <QString>
#include <QDebug>
#include <QModelIndex>

/**
 * @brief The BronItemAPI standard API interface for BronListTypes it is used in BronListModel and TreeModel to represent data
 * given by the derivated model elements. The access to the roles is done via roles!
 */

namespace QMLInterface{



class BronItemAPI : public QObject, public std::enable_shared_from_this<BronItemAPI>{
    Q_OBJECT
public:
    explicit BronItemAPI();
    BronItemAPI(const BronItemAPI &);
    virtual ~BronItemAPI(){}

    virtual QVariant getData(int role) const = 0;
    virtual bool setData(int role, const QVariant &v) = 0;
    virtual QHash<int, QByteArray> roleNames() const = 0;
    virtual void runService(QString name, QVariant data);

    int changeIndex(int ix);
    int changeLampIx(int ix);

    virtual bool operator > (BronItemAPI & lhs);
    virtual QVariant comparer();

    virtual void setOnline(bool val);

    int getModelIx() const;

    void setModelIx(int val);

    int getLampIx() const;
    void setLampIx(int lampIx);

    // tree access functions
    std::shared_ptr<BronItemAPI> getParent();
    std::weak_ptr<BronItemAPI> child(int number);
    int childNumber() const;
    int childCount() const;
    void insertChild(int position, std::shared_ptr<BronItemAPI> child);
    int row() const;
    int childPos(const std::weak_ptr<BronItemAPI> item);

private:
    int m_modelIx;
    int m_lampIx;
    std::shared_ptr<BronItemAPI> m_parentItem;
    QList<std::shared_ptr<BronItemAPI> > m_childItems;
};

//inline QVariant BronItemAPI::getData(int role) const{
//    Q_UNUSED(role)
//    return QVariant();
//}

//inline QHash<int, QByteArray> BronItemAPI::roleNames() const{
//    return QHash<int, QByteArray>();
//}

//inline bool BronItemAPI::setData(int role, const QVariant &v){
//    Q_UNUSED(role)
//    Q_UNUSED(v)
//    return false;
//}



inline void BronItemAPI::runService(QString, QVariant){
//    qDebug() << QString("service: ") << name << QString(" not supported");
}

inline bool BronItemAPI::operator >(BronItemAPI &lhs){
    return comparer() < lhs.comparer();
}

inline QVariant BronItemAPI::comparer(){
//    qDebug() << "implement comparer function in derived class";
    return QVariant();
}

inline void BronItemAPI::setOnline(bool val){
    Q_UNUSED(val);
//    qDebug() << "implement setOnline function in derived class";
}

inline int BronItemAPI::getModelIx() const{
    return m_modelIx;
}

inline void BronItemAPI::setModelIx(int val){
    m_modelIx = val;
}

inline int BronItemAPI::getLampIx() const{
    return m_lampIx;
}

inline void BronItemAPI::setLampIx(int lampIx){
    m_lampIx = lampIx;
}

inline std::shared_ptr<BronItemAPI> BronItemAPI::getParent(){
    return m_parentItem;
}

inline std::weak_ptr<BronItemAPI> BronItemAPI::child(int number){
    return m_childItems.value(number);
}

inline int BronItemAPI::childCount() const{
    return m_childItems.count();
}


}

Q_DECLARE_METATYPE(std::shared_ptr<QMLInterface::BronItemAPI>)
Q_DECLARE_METATYPE(std::weak_ptr<QMLInterface::BronItemAPI>)



#endif // BRONITEMAPI_H
