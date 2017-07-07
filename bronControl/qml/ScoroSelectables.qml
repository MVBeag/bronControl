pragma Singleton
import QtQuick 2.0

QtObject{
    id: scoroSelects
    property var deviceDesc:[
        {val: 0, tag: "invalid"},
        {val: 1, tag: "ScoroE1600"},
        {val: 2, tag: "ScoroE3200"},
        {val: 3, tag: "ScoroS1600"},
        {val: 4, tag: "ScoroS3200"}
    ]
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // be careful semantic changed to scoro selecor
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    property var pilotLigthMode: [
        {val: 0, tag: "OFF"},
        {val: 1, tag: "PROPMAX"},
        {val: 2, tag: "FULL"},
        {val: 3, tag: "LOW"},
        {val: 4, tag: "PROP. 1"},
        {val: 5, tag: "PROP. 2"},
        {val: 6, tag: "PROP. 3"},
        {val: 7, tag: "PROP. 4"},
        {val: 8, tag: "PROP. 5"}
    ]
    property var t01:[
        {val: 0, tag: "OPT"},
        {val: 4, tag: "others 1"},
        {val: 5, tag: "others 2"},
        {val: 6, tag: "others 3"},
        {val: 7, tag: "others 4"},
        {val: 8, tag: "others 5"},
        {val: 9, tag: "others 6"},
        {val: 128, tag: "MIN"},
        {val: 255, tag: "ACCEPT SUGGEST."}
    ]
    property var sequRange: {min: 0; max: 50}
    property var intervalTRange: {min: 2; max: 50000} // [ms]
    property var delay: {min: 0; max: 50000} // [ms]
    property var numDevicesAlternate: {min: 2; max: 4}
    property var alternateNumDevices:  [
        {val: 2, tag: "2"},
        {val: 3, tag: "3"},
        {val: 4, tag: "4"}
    ]
    property var delayDeviceAlternate: [
        {val: 0, tag: "OFF"},
        {val: 1, tag: "ACTIVE"},
        {val: 2, tag: "WAIT 1"},
        {val: 3, tag: "WAIT 2"},
        {val: 4, tag: "WAIT 3"}
    ]
//    //SP_PCSTATE,				/* not implemented */
//    SP_MEMORY_STORE, 			/* -p set; -c n=1-4 to store current settings into memory slot */
//    SP_MEMORY_RECALL, 			/* -p set; -c n=1-4 to recall memory slot into current settings */
//    SP_REFRESH_DATA, 			/* -p set; -c makes scoro send every block (probably never used) */
//    SP_RESET_AUX, 				/* -p set; -c make scoro reset the auxiliary settings */
//    SP_STORE_AUX,				/* -p get/set; -c make scoro save the auxiliary settings */
    //SP_OPTIONS_REG1,			/* implemented with the six below */
    property var sensitivityPhotoCell: [  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {val: 0, tag: "OFF"},             // semantic changed for gui not identic with scorodevice
        {val: 1, tag: "LOW"},             //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {val: 2, tag: "HIGH"}
    ]

    property var intensityTestKey: [
        {val: 0, tag: "BRIGHT"},
        {val: 1, tag: "DARK"}
    ]
//    SP_DISPLAY_SPEED_APERTURE,  /* -c n=1 display 10 (respectively 9 if SP_DISPLAY_APERTURE_9=1) as the maximal aperture in speed mode */
//    SP_REDUCED_PILOTLIGHT,      /* -c n=1 reduce the pilotlight during charging */
    property var intensityDisplay: [
        {val: 0, tag: "ENV. DEPENDENT"},
        {val: 1, tag: "LOW"},
        {val: 2, tag: "MEDIUM"},
        {val: 3, tag: "HIGH"}
    ]
    property var rdySoundMode: [
        {val: 0, tag: "STANDARD"},
        {val: 1, tag: "ADDR. SPECIFIC"}
    ]
    property var rdySoundVolume: [
        {val: 0, tag: "OFF"},
        {val: 1, tag: "QUIET"},
        {val: 2, tag: "MEDIUM"},
        {val: 3, tag: "NOISY"}
    ]

    property var rdySoundVolumeScoroE: [
        {val: 0, tag: "OFF"},
        {val: 2, tag: "MEDIUM"}
    ]

//    property var colorTemp: [
//        {val: 0, tag: "increment"},
//        {val: 1, tag: "decrement"},
//        {val: 2, tag: "suggest. takeover"}
//    ]
    property var alarmReason: [
        {val: 0, tag: "AFTERGLOW"},
        {val: 1, tag: "OVERCHARGE"},
        {val: 2, tag: "TECH. ISSUE"}
    ]
    property var thermoState: [
        {val: 1, tag: "FLASH CAPACITY"},
        {val: 2, tag: "LAMP 1"},
        {val: 3, tag: "LAMP 2"},
        {val: 4, tag: "LAMP 3"}
    ]

    property var colorTemp: [
        {val: 0, tag: "SETTING"},
        {val: 1, tag: "SETTING"},
        {val: 2, tag: "-400K"},
        {val: 3, tag: "-200K"},
        {val: 4, tag: "OPT"},
        {val: 5, tag: "+200K"},
        {val: 6, tag: "+400K"},
        {val: 7, tag: "+600K"},
        {val: 8, tag: "+800K"}
    ]

    property var chargeTime: [
        {val: 0, tag: "FAST"},
        {val: 1, tag: "SLOW"}
    ]

    property var onOff: [
        {val: 0, tag: "OFF"},
        {val: 1, tag: "ON"}
    ]
/* -p/-c ir/rfs (3), --/rfs (2), ir/-- (1), off (0) */
    property var flashControl: [
        {val: 0, tag: "OFF"},
        {val: 1, tag: "IR/--"},
        {val: 2, tag: "--/RFS"},
        {val: 3, tag: "IR/RFS"}
    ]
//    SP_FLASH_TEST,				/* -p set, does already exist */
//    SP_PROP_OVERFLOW,           /* -p get, -c something with the modellinglight? */
//    SP_T01_OVERFLOW,            /* -c when increasing the aperture, the flash duration increases too and the desired setting might not be possible anymore. Scoro blocks... accept with SP_T01 = 0xff */
//    SP_INTERVAL_OVERFLOW,       /* -c when increasing the aperture, the minimal interval time increases and the desired setting might not be possible anymore. Scoro blocks... accept with SP_INTERVAL >= min */
//    SP_COLORTEMP_OVERFLOW,      /* -c when increasing the aperture, high color temperature is no more possible and the desired setting might not be possible anymore. Scoro blocks... accept with SP_COLOR_TEMP = 2 */
//    SP_SLOWCHARGE_OVERFLOW,     /* -c never occured to me... */
//    SP_MIN_INTERVAL,            /* -c minimal interval time that can be set (depending on aperture and chare mode slow/fast) */
//    SP_MEM_APERTURE_L1,			/* -p get, -c first request a block (101-104) and then read the below parameters */

    function getT01TimeText(flashtime, t01setting){
        if(t01setting === undefined){
            return "UNKNOWN"
        }
        else{
            var i
            var text = undefined
            for(i = 0; i < t01.length; i++){
                if(t01setting == t01[i].val){
                    text = t01[i].tag
                    break
                }
            }
            if(text === "OPT" || text === "MIN"){
                text = " " + text
            }
            else{
                text = ""
            }

            return "t01=" + flashtime + text
        }
    }
}
