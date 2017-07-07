import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import ch.bron.BronModels 1.0
import TextFieldDoubleValidator 1.0
import "."


FocusScope{
    id: root

    width: DisplayCtrl.portraitWidth

    property var deviceType
    property var device
    property string name: root.device.name

    property var intval: IntValidator{bottom: 1; top: 99}
    property var intval050: IntValidator{bottom: 0; top:50}
    property var intval150: IntValidator{bottom: 1; top:50}
    property var dblval010_3: TextFieldDoubleValidator{bottom: 0.000; decimals: 3; top: 10.000; notation: DoubleValidator.StandardNotation}
    property var dblval050_2: TextFieldDoubleValidator{bottom: 0.00; decimals: 2; top: 50.00; notation: DoubleValidator.StandardNotation}

    property var regval: RegExpValidator{}

    property bool on: true
    property bool off: false

    signal back();

    Component.onCompleted: btQuit.forceActiveFocus()

    function findTagFromValue(val,sel){
        if(Array.isArray(sel)){
            var i
            var length = sel.length
            for(i =  0; i < length; i++){
                if(val === sel[i].val){
                    return sel[i].tag
                }
            }

        }
        else{
            return undefined
        }
    }


    ColumnLayout{
        id: layout
        width: parent.width

        property real heightWoFlicker: header.height + DisplayCtrl.gridMargin * 2 + deviceMoreSettingBottenLine.height
        property real maxFlickerHeight: root.height - heightWoFlicker
        spacing: 0
        Item{
            id: header
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: childrenRect.height
            Rectangle{
                id: deviceMoreCogniCol
                width: DisplayCtrl.dp(60)
                height: width
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.05
                anchors.margins: DisplayCtrl.dp(20)
                color: BronColors.intToColer(root.device.cogniColor)
            }
            Text{
                id: selectedMoreName
                font.family: bronFontBold.name
                font.pointSize: DisplayCtrl.point20Size
                font.bold: true
                anchors.verticalCenter: deviceMoreCogniCol.verticalCenter
                anchors.left: deviceMoreCogniCol.right
                anchors.leftMargin: DisplayCtrl.dp(20)
                color: "white"
                text: root.device.fullModelName
            }
            Text{
                id: swVersion
                font.family: bronFontBold.name
                font.pointSize: DisplayCtrl.point20Size
                font.bold: true
                anchors.verticalCenter: deviceMoreCogniCol.verticalCenter
                anchors.left: selectedMoreName.right
                anchors.leftMargin: DisplayCtrl.dp(30)
                color: "white"
                text: "V"+(((root.device.swVersion >>> 16)/256).toFixed(2))
            }
            BronToolButton{
                id: btQuit
                anchors.verticalCenter: deviceMoreCogniCol.verticalCenter


                anchors.right: parent.right
                font.family: googleMaterial.name
                font.pointSize: DisplayCtrl.point30Size


                KeyNavigation.tab: delayLine
                KeyNavigation.backtab: resetDailyLine
                Keys.onReturnPressed:{
                    event.accepted = true
                    root.back()
                }
                text: DisplayCtrl.icClose
                onClicked: root.back()
            }
            Rectangle{
                id: deviceMoreSettingUpperLine
                property real minuend//mapToItem(deviceMore,0,0).y

                width: parent.width
                height: DisplayCtrl.dp(4)
                anchors.top: deviceMoreCogniCol.bottom
                anchors.topMargin: DisplayCtrl.gridMargin
                color: "white"
                Component.onCompleted:{
                    minuend = mapToItem(deviceMore,0,0).y + deviceMoreSettingUpperLine.height + DisplayCtrl.gridMargin + DisplayCtrl.gridMargin
                }
            }
        }
        Item{
            Layout.preferredHeight: DisplayCtrl.gridMargin
        }

        Flickable{
            id: flick

            // do the following to break dependency loops
            property real fheight
            Component.onCompleted: fheight = flick.height

            property real flickLeftHeight: layout.maxFlickerHeight - fheight

            Layout.preferredWidth: parent.width
            Layout.preferredHeight: flickLeftHeight > 0 ? contentHeight : contentHeight + flickLeftHeight
            Layout.alignment: Qt.AlignHCenter
            focus: true
            clip: true
            interactive: height < contentHeight ? true : false
            contentWidth: parent.width
            contentHeight:  scoroLayout.height

            ScrollBar.vertical: ScrollBar {
                id: controlSB

                implicitWidth: Math.max(background ? background.implicitWidth : 0,
                                                     contentItem.implicitWidth + leftPadding + rightPadding)
                implicitHeight: Math.max(background ? background.implicitHeight : 0,
                                                      contentItem.implicitHeight + topPadding + bottomPadding)


                contentItem: Rectangle {
                    id: handleCt

                    implicitWidth: DisplayCtrl.dp(5)
                    implicitHeight: DisplayCtrl.dp(5)

                    radius: width / 2
                    color: controlSB.pressed ? BronColors.bronBlue : "white"
                    visible: controlSB.size < 1.0
                    opacity: 1.0

                    states: State {
                        name: "active"
                        when: controlSB.active
                    }

                    transitions: [
                        Transition {
                            to: "active"
                            SequentialAnimation {
                                NumberAnimation { target: handleCt; duration: 200; property: "opacity"; to: 1.0 }
                            }
                        },
                        Transition {
                            from: "active"
                            SequentialAnimation {
                                PauseAnimation { duration: 450 }
                                NumberAnimation { target: handleCt; duration: 200; property: "opacity"; to: 0.0 }
                            }
                        }
                    ]
                }
            }

            ColumnLayout{
                id: scoroLayout
                width: parent.width * 0.9
                focus: true
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: DisplayCtrl.gridMargin
                TextInputLine{
                    id: delayLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: standbyLine
                    prevFocusItem: btQuit
                    focus: true
                    hints: Qt.ImhFormattedNumbersOnly
                    name: "LAMP DELAY"
                    isPW: false
                    validate: dblval050_2
                    unit: "s"
                    readOnly: false
                    actValue: root.device.delay
                    decPlaces: 2
                    onValue: root.device.delay = result.val
                }
                ButtonInputLine{
                    id: standbyLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: modellingLightLine
                    prevFocusItem: delayLine
                    focus: true
                    name: "STANDBY"
                    actValue: root.device.standby
                    isPush: false
                    onValue: root.device.standby = result.val
                }
                ComboInputLine{
                    id: modellingLightLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: sequenceLine
                    prevFocusItem: standbyLine
                    focus: true
                    name: "MODELLING LIGHT"
                    selections: ScoroSelects.pilotLigthMode
                    selRole: "tag"
                    actValue: root.device.modLight === 0 ? 0 : root.device.modLightMode + 1
                    onValue:{
                        if(result.val === 0){
                            root.device.modLight = 0
                        }
                        else{
                            root.device.modLight = 1
                            root.device.modLightMode = result.val - 1
                        }
                    }
                }
                TextInputLine{
                    id: sequenceLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: intervalLine
                    prevFocusItem: modellingLightLine
                    focus: true
                    hints: Qt.ImhDigitsOnly
                    name: "SEQUENCE"
                    validate: intval050
                    actValue: root.device.sequence
                    onValue: root.device.sequence = result.val
                }
                TextInputLine{
                    id: intervalLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: alternateNumDevicesLine
                    prevFocusItem: sequenceLine
                    focus: true
                    hints: Qt.ImhFormattedNumbersOnly
                    name: "INTERVAL"
                    unit:"s"
                    decPlaces: 3
                    validate: dblval010_3
                    actValue: root.device.pauseTime
                    onValue: root.device.pauseTime = result.val
                }
                ComboInputLine{
                    id: alternateNumDevicesLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: alternateDevicesDelay
                    prevFocusItem: intervalLine
                    focus: true
                    name: actValue < 3 ?"FREEMASK" : "ALTERNATE"
                    selections: ScoroSelects.alternateNumDevices
                    selRole: "tag"
                    actValue: root.device.application + 1
                    onValue: root.device.application = result.val - 1
                }
                ComboInputLine{
                    id: alternateDevicesDelay
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: t01Line
                    prevFocusItem: alternateNumDevicesLine
                    focus: true
                    name: alternateNumDevicesLine.actValue < 3 ? "FREEMASK STATE" : "ALTERNATE STATE"
                    selections: ScoroSelects.delayDeviceAlternate
                    selRole: "tag"
                    actValue: root.device.maskgroup
                    onValue: root.device.maskgroup = result.val
                }
                ComboInputLine{
                    id: t01Line
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: colorTempAdjustLine
                    prevFocusItem: alternateNumDevicesLine
                    focus: true
                    name: "T01"
                    selections: ScoroSelects.t01
                    selRole: "tag"
                    actValue: root.device.flashTimeSetting
                    onValue: root.device.flashTimeSetting = result.val
                }
                TextInputLine{
                    id: colorTempLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    name: "COLOR TEMP"
                    isPW: false
                    validate: intval
                    readOnly: true
                    actValue: root.findTagFromValue(root.device.colorTemp, ScoroSelects.colorTemp)
                    decPlaces: -1
                }
                ButtonButtonInputLine{
                    id: colorTempAdjustLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: chargeTimeLine
                    prevFocusItem: t01Line
                    focus: true
                    leftName: "+200K"
                    rightName: "-200K"
                    actValue: 0
                    actDValue: 0
                    isPush: true
                    onValue: root.device.colorTemp = result.val
                    onDepSelector: root.device.colorTemp = !result.val
                }
                ComboInputLine{
                    id: chargeTimeLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: dimmerLine
                    prevFocusItem: colorTempAdjustLine
                    focus: true
                    name: "CHARGE TIME"
                    selections: ScoroSelects.chargeTime
                    selRole: "tag"
                    actValue: root.device.slowCharge
                    onValue: root.device.slowCharge = result.val
                }
                ComboInputLine{
                    id: dimmerLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: cellLine
                    prevFocusItem: chargeTimeLine
                    focus: true
                    name: "DIMMER"
                    selections: ScoroSelects.onOff
                    selRole: "tag"
                    actValue: root.device.dimmer
                    onValue: root.device.dimmer = result.val
                }
                ComboInputLine{
                    id: cellLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: rfsLine
                    prevFocusItem: dimmerLine
                    focus: true
                    name: "CELL"
                    selections: ScoroSelects.sensitivityPhotoCell
                    selRole: "tag"
                    actValue: root.device.cell === 0 ? 0 : root.device.cellSens + 1
                    onValue:{
                        if(result.val === 0){
                            root.device.cell = 0
                        }
                        else{
                            root.device.cell = 1
                            root.device.cellSens = result.val - 1
                        }
                        //                    root.device.cell = result.val
                    }
                }
                ComboInputLine{
                    id: rfsLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: brightnessTestLine
                    prevFocusItem: cellLine
                    focus: true
                    name: "FLASH CONTROL"
                    selections: ScoroSelects.flashControl
                    selRole: "tag"
                    actValue: root.device.irRfs
                    onValue: root.device.irRfs = result.val
                }
                ComboInputLine{
                    id: brightnessTestLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: rdySigVolLine
                    prevFocusItem: rfsLine
                    focus: true
                    name: "BRIGHTNESS TEST"
                    selections: ScoroSelects.intensityTestKey
                    selRole: "tag"
                    actValue: root.device.brightnessTest
                    onValue: root.device.brightnessTest = result.val
                }
                ComboInputLine{
                    id: rdySigVolLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: rdySigModeLine
                    prevFocusItem: brightnessTestLine
                    focus: true
                    name: "READY SIG VOL"
                    selections: ScoroSelects.rdySoundVolume
                    selRole: "tag"
                    actValue: root.device.audio
                    onValue: root.device.audio = result.val
                }
                ComboInputLine{
                    id: rdySigModeLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: brightnessDispLine
                    prevFocusItem: rdySigVolLine
                    focus: true
                    name: "READY SIG MODE"
                    selections: ScoroSelects.rdySoundMode
                    selRole: "tag"
                    actValue: root.device.audioMode
                    onValue: root.device.audioMode = result.val
                }
                ComboInputLine{
                    id: brightnessDispLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: resetDailyLine
                    prevFocusItem: rdySigModeLine
                    focus: true
                    name: "BRIGHTNESS DISP"
                    selections: ScoroSelects.intensityDisplay
                    selRole: "tag"
                    actValue: root.device.dispIntensity
                    onValue: root.device.dispIntensity = result.val
                }
                TextInputLine{
                    id: dailyCounterLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    name: "DAILY COUNTER"
                    isPW: false
                    validate: intval
                    readOnly: true
                    actValue: root.device.dailyCounter
                    decPlaces: 0
                }
                TextInputLine{
                    id: flashCounterLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    name: "FLASH COUNTER"
                    isPW: false
                    validate: intval
                    readOnly: true
                    actValue: root.device.overallCounter
                    decPlaces: 0
                }
                ButtonInputLine{
                    id: resetDailyLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: btQuit
                    prevFocusItem: brightnessDispLine
                    focus: true
                    name: "RES DAILY COUNTER"
                    actValue: 0
                    isPush: true
                    onValue: root.device.resetDailyCount = 1
                }
            }
        }
        Item{
            Layout.preferredHeight: DisplayCtrl.gridMargin
        }
        Rectangle{
            id: deviceMoreSettingBottenLine
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: DisplayCtrl.dp(2)
            color: "white"//BronColors.disabledGrey
        }
    }

    Rectangle{
        id: deviceMore
        anchors.fill: layout
        z: layout.z - 1
        color: BronColors.bronBlack
    }
    Rectangle{
        anchors.top: layout.top
        anchors.left: layout.right
        width: DisplayCtrl.dp(2)
        height: layout.height
    }

}
