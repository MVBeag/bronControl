pragma Singleton
import QtQuick 2.7
import ch.bron.Resolution 1.0
import QtQuick.Window 2.0

// try to get the same size on every display

Item {
    id: res

    readonly property int landscape: 0
    readonly property int portrait: 1
    readonly property real minZoom: 0.5
    readonly property real maxZoom: 4

//    Component.onCompleted: showSettings()

    ResolutionControl{
        id: reso
        textScale: 1.0
    }

    property Scale userZoom: Scale{
        id: zoom
        property real _scale: studioMgr.userScale
//        on_ScaleChanged: console.log("_scale " + _scale)
        xScale: zoom._scale
        yScale: zoom._scale
    }

    property real userScale: zoom._scale
    property real targetScale: 1.0

    onTargetScaleChanged: studioMgr.userScale = targetScale

    function dp(value){
        return scale < 1 ? reso.dp(value) * scale : reso.dp(value)
    }
    function sp(value){
        return scale < 1 ? reso.sp(value) * scale : reso.sp(value)
    }
    function dpPrivate(value){
        return reso.dp(value)
    }

    function showSettings(){
        console.log("orientation " + orientation)
        console.log("standardPortraitWidth " + standardPortraitWidth)
        console.log("scale " + scale)
        console.log("portraitWidth " + portraitWidth)
        console.log("portraitHeight " + portraitHeight)
        console.log("energySetterWidth " + energySetterWidth)
        console.log("itemSize " + itemSize)
        console.log("gridMargin " + gridMargin)
        console.log("minWidthDots " + minWidthDots)
        console.log("minWidthMM " + minWidthMM)
        console.log("mobilePlattform " + isMobilePlattform + " " + Qt.platform.os)
        console.log("userScale " + userScale)
        reso.showInfo()
    }

    // 0 landscape, 1 protrait
    readonly property var orientation: Screen.width > Screen.height ? landscape : portrait
    property bool isMobilePlattform: (Qt.platform.os === "android" || Qt.platform.os === "ios") ? true : false

    // minWidth holds the minimum of width and height of the real viewing area. Everything is derived
    // from minWidth
    readonly property real minWidthMM: reso.minWidthMM
    readonly property real minWidthDots: reso.minWidthDots
    // first designs were base on 160DPI (function dp), itemSize dp(220), gridMargin dp(20)
    //                                   (itemSize * 4 + gridMargin * 3) * 10 / 9
    property real standardPortraitWidth: (dpPrivate(220) * 4 + dpPrivate(20) * 3) * 10/9
    property real scale: minWidthDots < standardPortraitWidth ? minWidthDots/standardPortraitWidth
                                                              : isMobilePlattform ? minWidthDots/standardPortraitWidth : 1

    property real portraitWidth: standardPortraitWidth * scale
    property real energySetterWidth: portraitWidth * 9 / 10
    property real itemSize: energySetterWidth * 11 / 47
    property real gridMargin: itemSize/11
    property real portraitHeight: portraitWidth/3*4


    property real point80Size: 80 * scale
    property real point30Size: 30 * scale
    property real point25Size: 25 * scale
    property real point24Size: 24 * scale
    property real point20Size: 20 * scale
    property real point15Size: 15 * scale
    property real point10Size: 10 * scale
    property real point5Size:  5  * scale
    property real point50Size: 50 * scale

    // google material icons fronts translate mappings
    property string icBack: "\ue5c4"
    property string icDone: "\ue876"
    property string icClose: "\ue14c"
    property string icDelete: "\ue872"
    property string icCreate: "\ue150"
    property string icHelp: "\ue887"
    property string icSettings: "\ue8b8"
    property string icKeyboardArrorLeft: "\ue314"
    property string icKeyboardArrowRight: "\ue315"
    property string icReset: "\ue042"
    property string icModLight: "\ue90f"  //"e430"
    property string icDelay: "\ue8e4\ue430"
    property string icSequence: "\ue627\ue430"
    property string icVisibility: "\ue8f4"
}
