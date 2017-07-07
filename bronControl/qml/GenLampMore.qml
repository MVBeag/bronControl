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
    property var regval: RegExpValidator{}
    property var dblval010_3: TextFieldDoubleValidator{bottom: 0.000; decimals: 3; top: 10.000; notation: DoubleValidator.StandardNotation}
    property var dblval050_2: TextFieldDoubleValidator{bottom: 0.00; decimals: 2; top: 50.00; notation: DoubleValidator.StandardNotation}

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
            //    Text{
            //        id: swVersion
            //        font.family: bronFontBold.name
            //        font.pointSize: DisplayCtrl.point20Size
            //        font.bold: true
            //        anchors.verticalCenter: deviceMoreCogniCol.verticalCenter
            //        anchors.left: selectedMoreName.right
            //        anchors.leftMargin: DisplayCtrl.dp(30)
            //        color: "white"
            //        text: "V"+root.device.swVersion
            //    }
            BronToolButton{
                id: btQuit
                anchors.verticalCenter: deviceMoreCogniCol.verticalCenter
                anchors.right: parent.right
                font.family: googleMaterial.name
                font.pointSize: DisplayCtrl.point30Size

                KeyNavigation.tab: addressLine
                KeyNavigation.backtab: standbyLine
                Keys.onReturnPressed:{
                    event.accepted = true
                    root.back()
                }
                focus: true
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
                    id: addressLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: standbyLine
                    prevFocusItem: btQuit
                    focus: true
                    hints: Qt.ImhDigitsOnly
                    name: "LAMP ADDRESS"
                    isPW: false
                    validate: intval
                    readOnly: false
                    actValue: root.device.address
                    decPlaces: 0
                    onValue: root.device.address = result.val
                }
                ButtonInputLine{
                    id: standbyLine
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                    nextFocusItem: btQuit
                    prevFocusItem: standbyLine
                    focus: true
                    name: "STANDBY"
                    actValue: root.device.standby
                    isPush: false
                    onValue: root.device.standby = result.val
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
