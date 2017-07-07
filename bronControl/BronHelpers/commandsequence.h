#ifndef COMMANDSEQUENCE_H
#define COMMANDSEQUENCE_H

#include <stdint.h>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QString>
#include <QSharedData>
#include "deviceaction.h"

namespace Framework{
class DevicePara;
class Device;

/**
 * @abstract CommandSequence is a helper class to send commands without inerruption to the remote device. Due to the fact, that there could be requests and retries
 * it is in the normal operation not guranteed to do this. At some points of usage, you need Sequences. One thing is the Network change, there it has to be
 * garanteed that the password, the encryption and the new ssid are received by the remote device prior to call join network. One other things are typically
 * so called application setups. In this case, a list of commands has to be processed by the remote device.
 */


class CommandSequence : public QObject, public QSharedData{
    Q_OBJECT
public:
    explicit CommandSequence(const uint32_t nr, QString name);
    CommandSequence(const CommandSequence &c);

    ~CommandSequence(){}

    /**
     * @brief insertPara inserts a parameter into the command sequence
     * @param para para to insert
     */
    void insertPara(std::weak_ptr<DevicePara> para);

    /**
     * @brief registeringFinished closes the sequence for further commands.
     */
    void registeringFinished();

    /**
     * @brief addActionn is the work to be done after the sequnce has finished without error. There is no need to use DeviceActions
     * @param act
     */
    void addAction(DeviceAction act);

    /**
     * @brief number unique sequence number to separate sequences. The user has to guarantee that this number us unique
     */
    const uint32_t number;

    /**
     * @brief isEmpty
     * @return true if sequence is empty, else false
     */
    bool isEmpty() const;

    /**
     * @brief isOpen check if sequence is open for registering
     * @return
     */
    bool isOpen() const;

    /**
     * @brief getName
     * @return returns the given name
     */
    QString getName() const;


signals:
    /**
     * @brief sequenceDone signals, that the sequence is done. If no error is occoured, the action(s) are called
     * @param nr sequence number
     * @param error signals an error. In case of error, sequenceDone is emitted with an error. If more than one error occours, only the last one is signaled
     */
    void sequenceDone(uint32_t nr, Errors::Id error);

public slots:
    /**
     * @brief onRxResponse has to be called on parameter reception. If the para is in the sequence, it is removed. If no parameters are left and no error,
     * DeviceAction(s) arae called
     * @param para received
     * @param error error on reception
     */
    void onRxResponse(std::weak_ptr<DevicePara> para, quint32 error);

private:
    Errors::Id m_error;
    bool m_openForRegistering;
    QList<std::weak_ptr<DevicePara> > m_paras;
    QList<DeviceAction> m_SequenceActions;
    QString m_name;
};

inline void CommandSequence::insertPara(std::weak_ptr<DevicePara> para){
    if(m_openForRegistering){
        m_paras.append(para);
    }
}

inline void CommandSequence::registeringFinished(){
    m_openForRegistering = false;
}

inline bool CommandSequence::isEmpty() const {
    return m_paras.isEmpty();
}

inline bool CommandSequence::isOpen() const{
    return m_openForRegistering;
}

inline QString CommandSequence::getName() const{
    return m_name;
}


}

#endif // COMMANDSEQUENCE_H
