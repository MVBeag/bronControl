#ifndef STUDIOMGRSEQUENCE_H
#define STUDIOMGRSEQUENCE_H

#include <memory>
#include <functional>
#include <QObject>
#include <QList>
#include "device.h"
#include "errors.h"

namespace Framework{



class markedDevice{
    friend bool operator==(const markedDevice &rhs, const markedDevice &lhs);
public:
    markedDevice(std::weak_ptr<Device> d)
        : sDevice(d)
        , ready(false){}
    ~markedDevice(){}

    std::weak_ptr<Device> sDevice;
    bool ready;
};

bool operator==(const markedDevice &rhs, const markedDevice &lhs);

inline bool operator==(const std::shared_ptr<markedDevice> &lhs, const std::shared_ptr<markedDevice> &rhs){
    return *(lhs.get()) == *(rhs.get());
}





class StudioSequence : public QObject, public std::enable_shared_from_this<StudioSequence>{
    Q_OBJECT
public:
    enum class eErrors : uint8_t{
        noError,
        eDeviceCmd,
    };
    Q_ENUM(eErrors)


    explicit StudioSequence(uint32_t nr, QString name);
    ~StudioSequence();

    void append(std::weak_ptr<Device> d);

    void storeFunction(std::function<int ()> f);

    QString getName() const;
    void close();

    void fireReadyDevices();

signals:
    void devSequenceDone(std::shared_ptr<StudioSequence>);

public slots:
    void onDevSequenceDone(std::weak_ptr<Device> dev, Errors::Id error);

private:

    bool m_isOpen;
    uint32_t m_seqNr;
    eErrors m_lastError;
    QString m_name;
    QList<std::shared_ptr<markedDevice> > m_seqDevices;
//    QList<std::weak_ptr<Device> > m_seqDevices;
    std::function<int ()> m_fct;
};



inline QString StudioSequence::getName() const{
    return m_name;
}

inline void StudioSequence::close(){
    m_isOpen = false;
    fireReadyDevices();
}

}

Q_DECLARE_METATYPE(std::shared_ptr<Framework::StudioSequence>)
#endif // STUDIOMGRSEQUENCE_H
