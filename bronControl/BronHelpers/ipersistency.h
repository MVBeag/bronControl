#ifndef IPERSISTENCY_H
#define IPERSISTENCY_H

#include <QJsonObject>

class IPersistency{
public:
    /**
     * @brief The Settings enum control the settings during the copy process
     */
    enum class Settings: uint32_t{
        EXACT_COPY    = 0x00000001,    /**< do an exact copy */
        ONLY_MAIN     = 0x00000002,    /**< only the aperture is copied */
        ONLY_ENHANCED = 0x00000004,    /**< parameters like speed and sound are also copied */
        ONLY_APP      = 0x00000008,    /**< only FREEMASK and ALTERNATE are copied */
        ALL           = ONLY_MAIN + ONLY_ENHANCED + ONLY_APP,
        IGNORE_MAIN_WARNINGS        = 0x00020000, /**< ignore issues on main paras */
        IGNORE_ENHANCED_WARNINGS    = 0x00040000, /**< ignroe issues on enhanced paras */
        IGNORE_APP_WARNINGS         = 0x00080000, /**< ignore issues on applications */
    };

    /**
     * @brief The Warnings enum control the warning level
     */
    enum class Warnings: uint32_t{
        WARN_MAIN     = 0x00000002,     /**< warn only about main incompatibilities */
        WARN_ENHANCED = 0x00000004,     /**< warn only about the enhanced parameter incompatibilities */
        WARN_APP      = 0x00000008,     /**< warn only about app incompatibilities */
        WARN_ALL      = WARN_MAIN + WARN_ENHANCED + WARN_APP
    };

    /**
     * @brief The Status enum give the user status back
     */
    enum class Status : uint8_t{
        OK = 0,             /**< everythings is ok */
        LOWER_MIN_ENERGY,   /**< the actual paras have a lower minEnergy */
        HIGHER_MAX_ENERGY,  /**< the actual paras have a higher maxEnergy */
        APP_NOT_APPLICABLE, /**< the special applications FREEMASK or ALTERNATE are not compatible */
        FEWER_DEVICES,      /**< the actual studio has fewer devices */
        MORE_DEVICES,       /**< the actual studio has more devices */
    };

    /**
     * @brief The UserAction enum contains commands, given by user (caused by status or warning)
     */
    enum class UserAction: uint8_t{
        eIGNORE = 0x01,      /**< ignore last issue */
        eABORT = 0x02,       /**< stop */
        eROLLBACK = 0x04     /**< restore previous paras */
    };

    IPersistency()
        : m_settings(Settings::ALL)
        , m_warnings(Warnings::WARN_ALL)
        , m_status(Status::OK){}
    virtual ~IPersistency(){}

    /**
     * @brief read the data stored in json
     * @param json object containing the storee data
     */
    virtual void read(const QJsonObject &json) = 0;

    /**
     * @brief write the data stored in json
     * @param json object to store
     */
    virtual void write(QJsonObject &json) const = 0;

    /**
     * @brief getSettings getter for settings
     * @return stored settings
     */
    Settings getSettings() const;

    /**
     * @brief setSettings control the settings
     * @param settings new settings
     */
    void setSettings(const Settings &settings);

    /**
     * @brief getWarnings getter for warnings
     * @return last stored warning
     */
    Warnings getWarnings() const;

    /**
     * @brief setWarnings store new warning
     * @param warnings
     */
    void setWarnings(const Warnings &warnings);

    /**
     * @brief getSstatus getter for last status
     * @return last status
     */
    Status getStatus() const;

    /**
     * @brief setStatus set new status
     * @param status
     */
    void setStatus(const Status &status);

    /**
     * @brief ignore implement behaviour for ignoring
     */
    virtual void ignore() = 0;

    /**
     * @brief abort implement behaviour for abort
     */
    virtual void abort() = 0;

    /**
     * @brief rollback implement behaviour for rollback
     */
    virtual void rollback() = 0;

    void userAction(UserAction action);

private:
    Settings m_settings;
    Warnings m_warnings;
    Status   m_status;

};

#endif // IPERSISTENCY_H

inline IPersistency::Settings IPersistency::getSettings() const{
    return m_settings;
}

inline void IPersistency::setSettings(const Settings &settings){
    m_settings = settings;
}

inline IPersistency::Warnings IPersistency::getWarnings() const{
    return m_warnings;
}

inline void IPersistency::setWarnings(const Warnings &warnings){
m_warnings = warnings;
}

inline IPersistency::Status IPersistency::getStatus() const{
    return m_status;
}

inline void IPersistency::setStatus(const Status &status){
    m_status = status;
}

inline void IPersistency::userAction(IPersistency::UserAction action){
    switch(action){
    case UserAction::eIGNORE: ignore(); break;
    case UserAction::eABORT:  abort(); break;
    case UserAction::eROLLBACK: rollback(); break;
    }
}
