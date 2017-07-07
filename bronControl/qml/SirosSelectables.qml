pragma Singleton
import QtQuick 2.0
import ch.bron.BronModels 1.0

Item{
//typedef enum EnumSyncModeCell {
//  SYNC_MODE_CELL_OFF = 0,
//  SYNC_MODE_CELL_ON,
//  SYNC_MODE_CELL_BLINK
//} SyncModeCell;
    property var syncCellMode:[
        {val: 0, tag: "OFF"},
        {val: 1, tag: "ON"},
        {val: 2, tag: "BLINK"}
    ]
//enum ThermoAlarmType {
//  THERMO_NONE = 0,
//  THERMO_IGBT,
//  THERMO_CONDENSATOR,
//  THERMO_RESISTOR
//};
    property var thermoAlarmType:[
        {val: 0, tag: "NONE"},
        {val: 1, tag: "IGBT"},
        {val: 2, tag: "CAPACITOR"},
        {val: 3, tag: "RESISTOR"}
    ]
//enum AlarmType {
//  ALARM_NONE = 0,
//  ALARM_LAMP,      /* F1 */
//  ALARM_DISCHARGE,
//  ALARM_CHARGE,    /* F3 */
//  ALARM_AFTERGLOW, /* F2 */
//  ALARM_WRONG_SP,  /* F8 */
//  ALARM_SYSSNIV    /* F9 */
//};
    property var alarmType:[
        {val: 0, tag: "NONE"},
        {val: 1, tag: "LAMP"}, // F1
        {val: 2, tag: "DISCHARGE"},
        {val: 3, tag: "CHARGE"}, // F3
        {val: 4, tag: "AFTERGLOW"}, // F2
        {val: 5, tag: "WRONGSP"}, // F8
        {val: 6, tag: "SYSSNIV"} // F9
    ]
//    typedef enum EnumSyncMode {
//      SYNC_MODE_DISABLED = 0,
//      SYNC_MODE_CE,
//      SYNC_MODE_RF,
//      SYNC_MODE_ALL
//    } SyncMo
    property var syncMode:[
        {val: 0, tag: "DISABLED"},
        {val: 1, tag: "CELL"},
        {val: 2, tag: "RFS"},
        {val: 3, tag: "CELL/RFS"}
    ]

    function convertOverallSyncToSirosSync(sel){
        switch(sel){
        case ParaSelectors.RcOff: return ParaSelectors.SirosRcOff
        case ParaSelectors.RcRfs: return ParaSelectors.SirosRcRfs
        case ParaSelectors.RcIr:  return ParaSelectors.SirosRcOff
        case ParaSelectors.RcIrRfs: return ParaSelectors.SirosRcRfs
        case ParaSelectors.RcCell: return ParaSelectors.SirosRcCell
        case ParaSelectors.RcCellRfs: return ParaSelectors.SirosRcRfsCell
        case ParaSelectors.RcCellIr: return ParaSelectors.SirosRcCell
        case ParaSelectors.RcCellIrRfs: return ParaSelectors.SirosRcRfsCell
        default: return ParaSelectors.SirosRcOff
        }
    }

    function convertSirosSyncToOverallSync(sel){
        switch(sel){
        case ParaSelectors.SirosRcOff: return ParaSelectors.RcOff
        case ParaSelectors.SirosRcCell: return ParaSelectors.RcCell
        case ParaSelectors.SirosRcRfs: return ParaSelectors.RcRfs
        case ParaSelectors.SirosRcRfsCell: return ParaSelectors.RcCellRfs
        default: return ParaSelectors.RcOff
        }
    }


//    typedef enum EnumWIFIMode {
//      WIFI_MODE_OFF = 0,
//      WIFI_MODE_ON,
//      WIFI_MODE_SYNCHRO
//    } WIFIMode;
    property var wifiMode:[
        {val: 0, tag: "OFF"},
        {val: 1, tag: "ON"},
        {val: 2, tag: "SYNCHRO"}
    ]

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // semantic changed
    // typedef enum EnumModelingLightMode {
    //   MODELING_LIGHT_MODE_FULL = 0,
    //   MODELING_LIGHT_MODE_PROP
    // } ModelingLightMode;
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    property var modLightMode:[
        {val: 0, tag: "OFF"},
        {val: 1, tag: "FULL"},
        {val: 2, tag: "PROP."}
    ]
//    typedef enum EnumRFMode {
//      RF_MODE_NONE = 0,
//      RF_MODE_RFS
//    } RF
    property var rfMode:[
        {val: 0, tag: "NONE"},
        {val: 1, tag: "RFS"}
    ]
//    typedef enum EnumCognitionLightColor {
//      NONE = 0,
//      BLUE,
//      GREEN,
//      MAGENTA,
//      YELLOW,
//      CYAN,
//      RED
//    } CognitionLightColor;
    property var cogniLightColor:[
        {val: 0, tag: "none"},
        {val: 1, tag: "blue"},
        {val: 2, tag: "green"},
        {val: 3, tag: "magenta"},
        {val: 4, tag: "yellow"},
        {val: 5, tag: "cyan"},
        {val: 6, tag: "red"}
    ]
//    typedef enum _EnumWlanLoginEncType
//    {
//      ENC_TYPE_WEP = 0,
//      ENC_TYPE_WPA,
//      ENC_TYPE_WPA2
//    } EnumWlanLoginEncType;
    property var wlanEncMode:[
        {val: 0, tag: "WEP"},
        {val: 1, tag: "WPA"},
        {val: 2, tag: "WPA2"}
    ]
//    enum FreemaskAlternateType {
//      DISABLE = 0,
//      FREEMASK_ON,
//      ALTERNATE_WITH_3_GROUPS,
//      ALTERNATE_WITH_4_GROUPS
//    };
    property var freemaskAlternateType:[
        {val: 0, tag: "DISABLE"},
        {val: 1, tag: "FREEMASK"},
        {val: 2, tag: "ALTERNATE 3"}, // alternate with three groups
        {val: 3, tag: "ALTERNATE 4"}
    ]
//    enum MaskgroupType {
//      STANDBY_OFF = 0,
//      MAIN,
//      MASK1,
//      MASK2,
//      MASK3
//    };
    property var maskGroupType:[
        {val: 0, tag: "OFF"},
        {val: 1, tag: "MAIN"},
        {val: 2, tag: "MASK1"},
        {val: 3, tag: "MASK2"},
        {val: 4, tag: "MASK3"}
    ]
//    enum SynchronizedSequenceMode {
//      SS_OFF = 0,
//      MASTER,
//      SLAVE
//    };
    property var synchronizedSequenceMode:[
        {val: 0, tag: "OFF"},
        {val: 1, tag: "MASTER"},
        {val: 2, tag: "SLAVE"}
    ]
    property var onOff: [
        {val: 0, tag: "OFF"},
        {val: 1, tag: "ON"}
    ]

    function findValFromTag(sel, tag){
        if(Array.isArray(sel)){
            var i
            var length = sel.length
            for(i = 0; i < length; i++){
                if(tag === sel[i].tag)
                    return sel[i].val
            }
        }
        return undefined
    }

    function findTagFromVal(sel, val){
        if(Array.isArray(sel)){
            var i
            var length = sel.length
            for(i = 0; i < length; i++){
                if(val === sel[i].val){
                    return sel[i].tag
                }
            }
        }
        return undefined
    }


}
