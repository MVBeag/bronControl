import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import "."


Item{
    id: root
    
    property alias selDevice: selectedDevice.text
    property color cogniCol: "white"
    property var device
    property var group: Item{}
    
    signal back();
    Component.onCompleted: deviceEnergy.forceActiveFocus()

    
    Rectangle{
        id: background
        width: DisplayCtrl.portraitWidth
        height: DisplayCtrl.portraitHeight / 3
        color: BronColors.bronBlue

        Rectangle{
            id: cogniCol

            width: DisplayCtrl.dp(60)
            height: width

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: DisplayCtrl.dp(20)

            color: root.cogniCol
        }

        Text{
            id: selectedDevice

            anchors.top: parent.top
            anchors.left: cogniCol.right
            anchors.leftMargin: DisplayCtrl.dp(20)

            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point20Size
            color: "white"
        }
        Text{
            id: selectedName

            anchors.top: selectedDevice.bottom
            anchors.left: cogniCol.right
            anchors.leftMargin: DisplayCtrl.dp(20)

            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point20Size
            color: "white"
            text: root.device.fullModelName
        }

        BronToolButton{
            id: devicebackButton

            anchors.top: parent.top
            anchors.right: parent.right

            KeyNavigation.tab: deviceEnergy
            KeyNavigation.backtab: deviceEnergy
            Keys.onReturnPressed:{
                event.accepted = true
                root.back()
            }


            font.family: googleMaterial.name
            text: DisplayCtrl.icClose

            onClicked: {
                root.back()
            }
        }

        EnergySetter{
            id: deviceEnergy

            width: DisplayCtrl.energySetterWidth
            height: parent.height  - selectedDevice.height - selectedName.height

            anchors.top: selectedName.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            onSetFocus: devicebackButton.forceActiveFocus()
            onSetBackFocus: devicebackButton.forceActiveFocus()


            color: BronColors.bronBlue
            adjustButtonFocusColor: BronColors.device_blue//root.cogniCol

            isActive: false
            min: 0.0
            max: 10.0
            currentMin: root.device.minEnergy
            currentMax: root.device.maxEnergy
            currentRangeMin: root.device.energyTotalAperture
            currentRangeMax: root.device.energyTotalAperture
            onIncPower: device.increasePower = increment
            onDecPower: device.decreasePower = decrement
        }
    }
    Rectangle{
        id: deviceSettingBottenLine
        width: parent.width
        height: DisplayCtrl.dp(2)
        anchors.top: background.bottom
        color: "white"//BronColors.disabledGrey
    }
    Rectangle{
        id: devicetransparentArea
        width: parent.width
        height: parent.height - deviceSettingBottenLine.height - background.height
        anchors.top: deviceSettingBottenLine.bottom
        color: "black"
        opacity: 0.4
        MouseArea{
            id: deviceBackArea
            anchors.fill: parent
            onClicked: {
                root.back()
            }
        }
    }
}
