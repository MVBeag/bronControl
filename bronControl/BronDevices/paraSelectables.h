#ifndef PARASELECTABLES_h
#define PARASELECTABLES_h

#include <QObject>
#include "mw_parameter_description.h"

namespace Framework{


class ParaSelects : public QObject{
    Q_OBJECT
public:
    ParaSelects(){}
    ParaSelects(const ParaSelects &p)
        : QObject(p.parent()){}
    // copies form global.h Sirros Project

    enum class ThermoAlarmType: uint8_t {
      NONE = 0,
      IGBT,
      CONDENSATOR,
      RESISTOR
    };
    Q_ENUM(ThermoAlarmType)

    enum class AlarmType : uint8_t {
      NONE = 0,
      LAMP,      /* F1 */
      DISCHARGE,
      CHARGE,    /* F3 */
      AFTERGLOW, /* F2 */
      WRONG_SP,  /* F8 */
      SYSSNIV    /* F9 */
    };
    Q_ENUM(AlarmType)

    enum class syncMode : uint8_t {
      DISABLED = 0,
      CE,
      RF,
      ALL
    };
    Q_ENUM(syncMode)

    enum class SequenceModes : uint8_t {
        DISABLED = 0,
        MASTER,
        SLAVE
    };
    Q_ENUM(SequenceModes)

    enum class SpeedMode {
        notSupported = -1,
        Normal = 0,
        Speed,
        ScoroSpeed,
        HS,
        HSS
    };
    Q_ENUM(SpeedMode)

    enum class Applications : uint8_t{
        None = 0,
        Sequence,
        Freemask,
        Alternate
    };
    Q_ENUM(Applications)

    enum class ModelingLightMode : uint8_t {
      FULL = 0,
      PROP
    };
    Q_ENUM(ModelingLightMode)

    enum class RFMode : uint8_t {
      NONE = 0,
      RFS
    };
    Q_ENUM(RFMode)

    enum class CognitionLightColor : uint8_t {
      BLUE = 0, // = 0xff197ECB,
      GREEN, // = 0xff7CD128,
      MAGENTA, // = 0xffD128CF,
      YELLOW, // = 0xffDDD117,
      CYAN, // = 0xff28D1CF,
//      RED,// = 0xffff0000
      NONE
    };
    Q_ENUM(CognitionLightColor)

    enum class FreemaskMode : uint8_t {
        DISABLED = 0,
        FREEMASK,
        ALTERNATE_3_MASKS,
        ALTERNATE_4_MASKS
    };
    Q_ENUM(FreemaskMode)

    enum class MaskGroup : uint8_t {
        DISABLED = 0,
        ACTIVE,
        WAIT1,
        WAIT2,
        WAIT3
    };
    Q_ENUM(MaskGroup)

    enum class ApertureMode : uint8_t {
        FSTOP = 0,
        JOULE = 1
    };
    Q_ENUM(ApertureMode)

    enum class EncryptionMode : uint8_t{
        WEP = 0,
        WPA,
        WPA2
    };
    Q_ENUM(EncryptionMode)

    enum class DeviceCommState : uint8_t{
        Offline,
        Online,
        BadCom
    };
    Q_ENUM(DeviceCommState)
    enum class NetworkState : uint8_t{
        NetworkUnknown = 0,               //< not known to system,
        NetworkKnownNotVisible,       //< known Config, but actual not visible (e.g. not in range)
        NetworkKnownAccessable,       //< known, can be used to start a session
        NetworkActive                 //< known and active
    };
    Q_ENUM(NetworkState)


    enum class RemoteCtrlSwitch: uint8_t{
        off = 0,
        ir,
        rfs,
        ir_rfs
    };
    Q_ENUM(RemoteCtrlSwitch)

    enum class RemoteCtrlCellRfs: uint8_t{
        SirosRcOff = 0,
        SirosRcCell,
        SirosRcRfs,
        SirosRcRfsCell
    };
    Q_ENUM(RemoteCtrlCellRfs)

    // RemoteCtrlSwitch and RemoteCtrlCellRfs combined
    enum class RemoteCtrl: uint8_t{
        RcOff = 0,
        RcRfs,
        RcIr,
        RcIrRfs,
        RcCell,
        RcCellRfs,
        RcCellIr,
        RcCellIrRfs
    };
    Q_ENUM(RemoteCtrl)
};

}

Q_DECLARE_METATYPE(Framework::ParaSelects)
Q_DECLARE_METATYPE(Framework::ParaSelects::RemoteCtrl)


#endif // PARASELECTABLES_h
