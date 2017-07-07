import QtQuick 2.6
import ch.bron.BronModels 1.0
import "."

FocusScope {
    id: root

    width:  background.width //+ lampList.width
    height: DisplayCtrl.itemSize
//    transform: DisplayCtrl.userZoom

    property alias flashTime: timeText.text
    property var generator

    property int address: 1
    property bool standby: false

    property real innerWidth: contentWidth

    signal moreClicked(var generator)
    signal generatorClicked(var generator)
    signal longClicked(var generator)
    signal moreLongClicked(var generator)

    focus: true
    property Item focusItem: background


    property int apertureMode: Selectables.fStopRange
    property string mainText: apertureMode === Selectables.fStopRange ? generator.energyTotalAperture.toFixed(1) :
                                                                        (generator.apertureJ.toFixed(1) < 10 ? generator.apertureJ.toFixed(1) + "J" :
                                                                                                               generator.apertureJ.toFixed(0) + "J")
    Rectangle {
        id: background
        width:  DisplayCtrl.itemSize
        height: DisplayCtrl.itemSize

        anchors.left: parent.left
        color: BronColors.bronBlack
        border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : activeFocus ? "white" : BronColors.bronBlue
        border.width: DisplayCtrl.dp(4)


        focus: true
//        KeyNavigation.right: lampList
        Keys.onPressed: {
            if (event.key === Qt.Key_Return){
                if(event.modifiers & Qt.ShiftModifier){
                    event.accepted = true
                    focusItem = background
                    root.moreClicked(root)
                }
                else{
                    event.accepted = true
                    focusItem = background
                    root.generatorClicked(root)
                }
            }
        }

        Text {
            id: modelName
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: DisplayCtrl.dp(10)
            font.family: bronFont.name
            font.pixelSize: DisplayCtrl.sp(parent.height / 10)
            text: root.generator.fullModelName
            color: root.generator.standby ? "white" : BronColors.bronBlue
        }

        Text {
            id: energyText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: DisplayCtrl.dp(-35)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: bronFontBold.name
            font.bold: true
            font.pointSize: DisplayCtrl.point24Size
            color: root.generator.standby ? "white" : BronColors.bronBlue
            text: root.generator.standby ? "--" : mainText
//            onTextChanged: jouleText.text = root.generator.apertureJ.toFixed(1) + " J"
        }

        Text {
            id: timeText
            anchors.top: energyText.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: bronFontBold.name
            font.bold: true
            font.pointSize: DisplayCtrl.point10Size
            opacity: root.generator.standby ? 0.0 : 1.0
            color: BronColors.bronBlue
            text: ScoroSelects.getT01TimeText(root.generator.flashDuration, root.generator.flashTimeSetting)
        }

        Rectangle {
            id: bottomBackground
            width: parent.width
            height: parent.height / 4
            anchors.bottom: parent.bottom
            color: BronColors.bronBlueDark
            border.color: background.activeFocus ? "white" : color
            border.width: DisplayCtrl.dp(4)
            opacity: root.generator.standby ? 0.0 : 1.0

            MouseArea{
                id: moreButton
                anchors.fill: parent
                onPressAndHold: {
                    background.forceActiveFocus()
                    root.moreLongClicked(root)
                }

                onClicked: {
                    background.forceActiveFocus()
                    root.moreClicked(root)
                }
            }
        }
        Text{
            id: modLight
            anchors.bottom: bottomBackground.bottom
            anchors.bottomMargin: DisplayCtrl.dp(5)
            anchors.left: bottomBackground.left
            anchors.leftMargin: DisplayCtrl.dp(5)
            font.family: googleMaterial.name
            font.pixelSize: DisplayCtrl.sp(parent.height / 9)
            horizontalAlignment: Qt.AlignLeft
            color: "white"
            visible: root.generator.modLight > 0
            text: DisplayCtrl.icModLight
        }
        Text{
            id: delay
            anchors.bottom: modLight.top
            //                anchors.bottomMargin: DisplayCtrl.dp(5)
            anchors.left: bottomBackground.left
            anchors.leftMargin: DisplayCtrl.dp(5)
            font.family: googleMaterial.name
            font.pixelSize: DisplayCtrl.sp(background.height / 10)
            horizontalAlignment: Qt.AlignLeft
            color: "white"
            visible: root.generator.delay > 0
            text: DisplayCtrl.icDelay
        }
        Text{
            id: sequ
            anchors.bottom: bottomBackground.bottom
            anchors.bottomMargin: DisplayCtrl.dp(5)
            anchors.right: bottomBackground.right
            anchors.leftMargin: DisplayCtrl.dp(5)
            font.family: googleMaterial.name
            font.pixelSize: DisplayCtrl.sp(background.height / 10)
            horizontalAlignment: Qt.AlignRight
            color: "white"
            visible: root.generator.sequence > 0
            text: DisplayCtrl.icSequence
        }

        Text{
            anchors.fill: bottomBackground
            font.family: bronFontBold.name
            font.pixelSize: DisplayCtrl.sp(background.height / 10)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "white"
            text: "MORE"
        }
        Text{
            id: remote
            function setText(sel){
                switch(sel){
                case ParaSelectors.RcOff: return ""
                case ParaSelectors.RcRfs: return "RFS"
                case ParaSelectors.RcIr:  return "IR"
                case ParaSelectors.RcIrRfs: return "IR/RFS"
                case ParaSelectors.RcCell: return "CELL"
                case ParaSelectors.RcCellRfs: return "CELL/RFS"
                case ParaSelectors.RcCellIr: return "CELL/IR"
                case ParaSelectors.RcCellIrRfs: return "CELL/IR/RFS"
                default: return ""
                }
            }

            anchors.top: bottomBackground.top
            anchors.right: bottomBackground.right
            anchors.rightMargin: DisplayCtrl.dp(5)
            font.family: bronFontMedium.name
            font.pixelSize: DisplayCtrl.sp(background.height / 15)
            horizontalAlignment: Qt.AlignRight
            color: "white"
            visible: root.generator.remoteSwitch !== ParaSelectors.RcOff//root.device.delay > 0
            text: setText(root.generator.remoteSwitch)
        }


        MouseArea{
            id: lampButton
            width: parent.width
            height: parent.height  - bottomBackground.height
            anchors.top: parent.top
            onPressAndHold: {
                background.forceActiveFocus()
                root.longClicked(root)
            }

            onClicked: {
                background.forceActiveFocus()
                root.generatorClicked(root)
            }
        }
    }
}
