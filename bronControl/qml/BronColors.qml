pragma Singleton
import QtQuick 2.0
import ch.bron.BronModels 1.0

QtObject{
    id: globalColors

    property color overlay_background: "#88000000"
    property color keyboardGray: "#ff444444"
    property color windowsBg: "#ff333333"

    property color slideStatDarkBg: "#5584bce7"
    property color slideStatMediumBg: "#7784bce7"
    property color slideStatLightBg: "#AA84bce7"
    property color slideStatLineMarker: "#84bce7"

    property color bronBlue: "#ff78afdf"
    property color bronBlueDark: "#ff6195b7"
    property color bronGray: "#ff252a2e"
    property color bronBlack: "#ff1f272a"

    property color selctorOverlayDark: "#44ffffff"
    property color disabledGrey: "#909090"
    property color disabledGreyDark: "#404040"
    property color errorRed: "#ffbc1e1e"
    property color errorRedDark: "#ff781313"

    property color device_blue: "#197ECB"
    property color device_yellow: "#DDD117"
    property color device_green: "#7CD128"
    property color device_magenta: "#D128CF"
    property color device_cyan: "#28D1CF"
    property color device_red: "#ff0000"


    function intToColer(address){
        switch(address){
//        case ParaSelectors.NONE:
        case ParaSelectors.BLUE:                return device_blue
        case ParaSelectors.GREEN:               return device_green
        case ParaSelectors.MAGENTA:             return device_magenta
        case ParaSelectors.YELLOW:              return device_yellow
        case ParaSelectors.CYAN:                return device_cyan
//        case ParaSelectors.RED:                 return device_red
        case ParaSelectors.NONE:                return "white"
        default:                                return bronBlue
        }
    }
}
