/**
  * @class DevicePara
  * @brief device para represents the last step in reading or writing the paras
  * due to the fact, that paras are remote and the local image should represent the remote
  * state, paras are read and written with the following sceme:
  * Device.DevicePara.setData()
  *          --> add a request to the requestHandler (fifo)
  *          --> these request handler runs in a worker for reading or writing parameters. it uses a communication
  *          --> to communicate with the de
  *
  */

#ifndef DEVICEPARA_H
#define DEVICEPARA_H

#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QByteArray>
#include <QDataStream>
#include <memory>
#include "errors.h"
#include <QElapsedTimer>
#include <stdint.h>
#include <functional>

/* Software version */
#define SOFTWARE_VERSION_MIN            4907


namespace Framework{

class Device;
class Request; /**< foreward declaraion */

// used to check if para is valid or read other variables
typedef bool (*preReadFct)(Device &, const QVariant val);
inline bool everValid(Device &, const QVariant ){
    return true;
}
// used to setup dependend parameters
typedef void (*depFunction)(Device &);
inline void noDependency(Device &){
    return;
}


/**
 * @brief The DevicePara class represents a parameter without dependencies this parameter has the ability to communicate
 */
class DevicePara : public QObject, public std::enable_shared_from_this<DevicePara>{
    Q_OBJECT

public:
    enum class triggerReason : unsigned int{
        eNewTarget,       // command initiated set
        eNewVal,          // new value in setData, only valid if no eCmd is set
        eDoNothing        // signals a get command, do nothing with the stored values
    };
    Q_ENUM(triggerReason)

    /**
     * @brief The Setting enum controls the behaviour ot startup and run time of one parameter.
     */
    enum Setting{
        noOptions = 0x0, /**< nothing happens with the para */
        eInit = 0x01,    /**< read out during Init of using device */
        ePoll = 0x02     /**< polled during livetime of the device to keep the parameter synchron with the remote device */
    };
    Q_DECLARE_FLAGS(Settings, Setting)
    Q_FLAG(Settings)
    enum class Types : uint8_t{
        eInt,
        eString
    };
    Q_ENUM(Types)

    enum class State : uint8_t{
        eUndefined,
        eDefined
    };

    /**
     * @brief The eRxStatus enum gives info about the resonse
     */
    enum eRxStatus{
        eOk,        /**< everything is ok */
        eTimeOut,   /**< no response after n retries (n is device dependent) */
        eOther      /**< other error, eg. invalid resource , device dependent) */
    };

    /**
     * @brief DevicePara Constructor constructs an initialized parameter
     * @param id Identifier of the parameter
     * @param value value to initialize the parameter
     * @param type type, for future use
     * @param elder pointer to Device containing the parameter
     * @param s Settings parameter to control the behaviour
     */
    explicit DevicePara(Device * parent, int id, const QVariant value, Types type, float scale = 1.0f, DevicePara::Settings s = DevicePara::noOptions, preReadFct fct = &everValid, depFunction dfct = &noDependency);
    DevicePara(const DevicePara &p);
    virtual ~DevicePara(){}
    /**
     * @brief GetRemote triggers a remote read out of the paramter, this function runs asynchron! After completion or at an error, the
     * ParamStringCB or ParamIntCB function is called to retrieve the parameter or to get an error code
     */
    virtual void getRemote();

    /**
     * @brief GetRemote triggers a remote write of the paramter, this function runs asynchron! After completion or at an error, the
     * ParamStringCB or ParamIntCB function is called to retrieve the status or to get an error code
     * @param Value to set remote
     */
    virtual bool setRemote(const QVariant value, QList<std::function<bool()> > postCmd = QList<std::function<bool()> >());

    /**
      * @brief setRemote triggers a remote write of this parameter, written is the actual value
      */
    virtual bool setRemote();

    /**
     * @brief returns an init Request
     * @return
     */
    Request *GetInitRequest();

    /**
     * @brief GetPollRequest returns an Poll request
     * @return
     */
    Request *GetPollRequest();

    /**
     * @brief setDataForRemote sets the temporary data used to write to remote device
     * @param var value to write
     * @param commandTriggered indicates, that set was initiated by a commannd
     */
    void setDataForRemote(triggerReason commandTriggered = triggerReason::eDoNothing, const QVariant var = QVariant());

    DevicePara &operator=(const DevicePara &arg);

    virtual bool isDemo() const;

    /**
     * @brief getComTime
     * @return returns the runtime of the com (or timeout) in ms
     */
    qint64 getComTime() const;
    void startComTimer();


    /**
     * @brief setData setter for m_data. This functions emits a deviceDataChanged signal on successful setting (is has to be a new value)
     * @param val data to set.
     */
    bool setData(const QVariant &val, Errors::Id error = Errors::Id::E_NO_ERROR, bool doNotCheckValue = false);

    /**
     * @brief data getter of m_data
     * @return a QVariant holding the data
     */
    virtual QVariant data() const{
        if(m_triggerResponse.isTargetReached())
            return m_data;
        else
            return m_triggerResponse.getResponseData();
    }

    virtual QVariant scaledData() const{
        return m_data.toFloat() * m_scaling;
    }


    /**
     * @brief scaledData returns the value for communication with scaling
     * @return
     */
    QVariant scaledDataForRemote() const;

    const char * stringBufferForRemote() const;

    int stringBufferSizeForRemote() const;

    /**
     * @brief Id get the Id given in the standard constructor
     * @return Id
     */
    int Id();

    /**
     * @brief GetElder Pointer to using device
     * @return pointer
     */
    Device &GetElder() const;

    /**
     * @brief Type get the type of the element, stored in constructor
     * @return Type as QMetaType::type (look at QT documataion about the type system
     */
    Types Type()const;

    /**
     * @brief GetSettings retrieves the actual settings
     * @return Settings
     */
    Settings GetSettings() const;

    bool operator == (const DevicePara& r) const;


    State getState() const;

signals:

    /**
     * @brief deviceParaDataChanged QT Signal to inform othe items about a data change. Is called in setData function
     * @param dp changed parameter
     */
    void deviceParaDataChanged(std::weak_ptr<DevicePara> dp);


    /**
     * @brief rxResponse signal for sequence handling
     * @param para pointer to this parameter
     * @param error the error occourd or no error
     */
    void rxResponse(std::weak_ptr<DevicePara> para, quint32 error);

public slots:


protected:
    class TriggerResponseData{
    public:

        TriggerResponseData(DevicePara &elder)
            : m_targetReached(true)
            , duration()
            , responseData()
            , m_cmdTriggered(triggerReason::eDoNothing)
            , m_elder(elder){
            duration.start();
        }
        ~TriggerResponseData(){
        }

        TriggerResponseData &operator = (const TriggerResponseData &r);

        const QVariant &getResponseData() const;
        const char * getResponseString() const;
        int getResponseBufferSize() const;
        void setResponseData(triggerReason commandTriggered, const QVariant value);
        qint64 getRuntimeMs() const;
        void startTimer();
        bool isTargetReached() const;

    private:
        bool m_targetReached;           /**< true, if target = value, false if not */
        QElapsedTimer duration;         /**< timer for precise measurement */
        QVariant responseData;          /**< temp storage for data should be written to remote device */
        QByteArray m_memory;            /**< Memory, needed for communication of strings, copy of m_data */
        triggerReason m_cmdTriggered;            /**< indicates that value change was forced by a command */
        DevicePara &m_elder;
    };


    Settings m_settings;                /**< hold the startup and runtime behaviour of the parameter */
    int m_id;                           /**< Identifier of the parameter */
    QVariant m_data;                    /**< Data of the parameter */
    QByteArray m_memory;                /**< Memory, needed for communication of strings, copy of m_data */
    Types m_type;                       /**< Data type, reserved for future use */
    Device &m_elder;                    /**< reference to using object */
    float m_scaling;                    /**< scaling for parameter from communication value */
    preReadFct m_preReadFct;
    depFunction m_depFct;
    State m_state;
    TriggerResponseData m_triggerResponse;
};

bool operator ==(const std::weak_ptr<DevicePara> &a, const std::weak_ptr<DevicePara> &b);


inline const QVariant &DevicePara::TriggerResponseData::getResponseData() const{
    return responseData;
}

inline qint64 DevicePara::TriggerResponseData::getRuntimeMs() const{
    return duration.elapsed();
}

inline void DevicePara::TriggerResponseData::startTimer(){
    duration.restart();
}

inline bool DevicePara::TriggerResponseData::isTargetReached() const{
    return m_targetReached;
}

inline void DevicePara::setDataForRemote(triggerReason commandTriggered, const QVariant var){
    m_triggerResponse.setResponseData(commandTriggered, var);
}
inline bool DevicePara::isDemo() const{
    return false;
}

inline qint64 DevicePara::getComTime() const{
    return m_triggerResponse.getRuntimeMs();
}

inline void DevicePara::startComTimer(){
    m_triggerResponse.startTimer();
}

inline DevicePara::State DevicePara::getState() const{
    return m_state;
}

class DefaultPara : public DevicePara{
public:
    DefaultPara(Device *  parent)
        : DevicePara(parent, 0, QVariant(0),Types::eInt){}
    virtual QVariant data() const override;
    virtual bool setRemote(const QVariant value, QList<std::function<bool()> > postCmd = QList<std::function<bool()> >()) override;
    virtual void getRemote() override;
    int id() const;
    void setId(int id);

private:
    int m_id;
};

inline void DefaultPara::getRemote(){
}

Q_DECLARE_OPERATORS_FOR_FLAGS(DevicePara::Settings)

}

#endif // DEVICEPARA_H
