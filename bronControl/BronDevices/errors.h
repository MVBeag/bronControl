#ifndef ERRORS_H
#define ERRORS_H

#include <QObject>
namespace Framework{

class Errors : public QObject{
    Q_OBJECT

    public:
        enum class Id : uint32_t{
            E_NO_ERROR            = 0L,
            PARAM_INEXISTENT      = 1L,
            PARAM_WRONG_TYPE      = 2L,
            PARAM_OUT_OF_RANGE    = 3L,
            BUFFER_TOO_SMALL      = 4L,
            OUT_OF_MEMORY         = 5L,
            MANY_REQUESTS         = 6L,
            ENCODING              = 7L,
            PARAM_READ_ONLY       = 8L,
            PARAM_TIMEOUT         = 9L,

            OSAPI_PORT_IN_USE     = 0x1000UL,
            OSAPI_TIMER_FAIL      = 0x1001UL,
            OSAPI_DID_NOT_CONNECT = 0x1002UL,
            OSAPI_CONNECT_FAILED  = 0x1003UL,

            BCOM_VERSIONSMISMATCH = 0x2000UL,
            BCOM_WRONG_SERVICEID  = 0x2001UL,
            BCOM_PROTOCOL_ERROR   = 0x2002UL,

            LOCAL_PTR_INVALID     = 0xfffffffbUL,
            LOCAL_INIT_ERROR      = 0xfffffffcUL,
            RESOURCE_NOT_AVAIL    = 0xfffffffdUL,
            CODE_OSAPI_UNKNOWN    = 0xfffffffeUL,
            CODE_UNKNOWN          = 0xffffffffUL
        };
        Q_ENUM(Id)

        Errors();
        Errors(Id id);
        Errors(const Errors &i);
        Errors(uint32_t id);

        ~Errors(){}

        Id GetId() const;

        operator uint32_t() const;
        QString Name();

    private:
        const Id m_id;
};


}
Q_DECLARE_METATYPE(Framework::Errors)
Q_DECLARE_METATYPE(Framework::Errors::Id)

#endif // ERRORS_H
