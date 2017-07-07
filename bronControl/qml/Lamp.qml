import QtQuick 2.7
import ch.bron.BronModels 1.0
import "."


Item {
    id: root

    width:  DisplayCtrl.itemSize
    height: DisplayCtrl.itemSize
    property Scale zoom: Scale{}

    transform: root.zoom

    property alias cogniColor: cogniColor.color
    property alias channel: lampChannel.text
    property bool areDifferent: false

    property var device
    property bool updating: false
    property bool useChannel: false
    property bool selectionMode: false
    property bool selected: false
    property string selectionText: ""
    property color selectionColor: BronColors.bronBlue
    property bool hasFocus: false
    property int apertureMode: Selectables.fStopRange
    property string mainText: apertureMode === Selectables.fStopRange ? device.energyTotalAperture.toFixed(1) :
                                                                        (device.apertureJ.toFixed(1) < 10 ? device.apertureJ.toFixed(1) + "J" :
                                                                                                            device.apertureJ.toFixed(0) + "J")

    signal moreClicked(var anObject)
    signal lampClicked(var anObject)
    signal longClicked(var anObject)
    signal moreLongClicked( var anObject)


    Rectangle {
            id: background

            anchors.fill: parent
//            width: parent.width
//            height: parent.height
            color: root.selectionMode ? (root.selected ? root.selectionColor : "black")
                                      : root.device.standby ? BronColors.bronBlack : updating ? "orange" : BronColors.bronBlue
            border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : root.hasFocus ? "white" : BronColors.bronBlue
            border.width: DisplayCtrl.dp(4)

            // text for model name upper left corner
            Text {
                id: modelName
                visible: useChannel == false
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: DisplayCtrl.dp(10)
                font.family: bronFont.name
                font.pixelSize: DisplayCtrl.sp(parent.height / 10)
                color: root.selectionMode ? "white" : (root.device.standby ? "white" : root.selectionMode ? "white" : "black")
                text: root.device.fullModelName
            }

            // lamp channel number upper right corner
            Rectangle {
                id: lampChannelNummber
                visible: useChannel == true
                width: parent.width / 5.5
                height: parent.height / 5.5
                anchors.top: parent.top
                anchors.left: parent.left
                color: BronColors.bronBlack

                Rectangle{
                    id: lampChannelTextBackground
                    width: parent.width * 0.85
                    height: parent.height * 0.85
                    anchors.top: parent.top
                    anchors.left: parent.left
                    color: root.selectionMode ? (root.selected ? root.selectionColor : "black") : BronColors.bronBlue
                    Text{
                        id: lampChannel
                        anchors.fill: parent
                        font.family: bronFont.name
                        font.pixelSize: DisplayCtrl.sp(parent.height / 1.5)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "1"
                        color: root.selectionMode ? (root.selected ? "black" : "white") : "black"
                    }
                }
            }

            // cogni color symbol upper right corner
            Rectangle {
                id: cogniBack
                width: parent.width / 5.5
                height: parent.height / 5.5
                anchors.top: parent.top
                anchors.right: parent.right
                color: BronColors.bronBlack

                Rectangle{
                    id: cogniColor
                    width: parent.width * 0.85
                    height: parent.height * 0.85
                    anchors.top: parent.top
                    anchors.right: parent.right
                    Text{
                        id: groupAddress
                        anchors.fill: parent
                        font.family: bronFont.name
                        font.pixelSize: DisplayCtrl.sp(parent.height / 1.5)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: device.address !== undefined ? device.address : ""
                        color: "black"
                    }
                }
            }

            // text for energy, main information of this control in the middle
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
                color: root.selectionMode ? (root.selected ? "black" : "white") : root.device.standby ? "white" : "black"
                text: root.device.standby ? "--" : mainText
//                onTextChanged: jouleText.text = root.device.apertureJ.toFixed(1) + " J"
            }

//            // second text line undere the energy text
//            Text {
//                id: jouleText
//                anchors.top: energyText.bottom
//                anchors.topMargin: DisplayCtrl.dp(-10)
//                anchors.horizontalCenter: parent.horizontalCenter
//                horizontalAlignment: Text.AlignHCenter
//                verticalAlignment: Text.AlignVCenter
//                font.family: bronFontBold.name
//                font.pixelSize: DisplayCtrl.sp(parent.height / 10)
//                color: root.selectionMode ? "white" : "black"
//                opacity: root.device.standby ? 0.0 : 1.0
//                text: root.device.apertureJ.toFixed(1) + " J"
//            }

            // third text line undere the energy text
            Text {
                id: timeText
                anchors.top: energyText.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font.family: bronFontBold.name
                font.bold: true
                font.pointSize: root.device.speed === ParaSelectors.HS ? DisplayCtrl.point15Size : DisplayCtrl.point10Size

                visible: root.device.flashDuration === undefined ? false: true

                color: root.selectionMode ? "white" : "black"
                opacity: root.device.standby ? 0.0 : 1.0
                text: root.device.speed === ParaSelectors.HS ? "HS" :
                                                               "1/"+ root.device.flashDuration + " s"
            }

            // more button at bottom of the control
            Rectangle {
                id: bottomBackground
                width: parent.width
                height: parent.height / 4
                visible: root.selectionMode ? false : true
                anchors.bottom: parent.bottom
                color: BronColors.bronBlueDark
                opacity: root.device.standby ? 0.0 : 1.0
//                border.color: root.hasFocus ? "white" : BronColors.bronBlueDark
                border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : root.hasFocus ? "white" : BronColors.bronBlue
                border.width: DisplayCtrl.dp(4)

                MouseArea{
                    id: moreButton
                    anchors.fill: parent
                    onClicked: root.moreClicked(root)
                    onPressAndHold: root.moreLongClicked(root)
                }
            }
            Text{
                id: modLight
                anchors.bottom: bottomBackground.bottom
                anchors.bottomMargin: DisplayCtrl.dp(5)
                anchors.left: bottomBackground.left
                anchors.leftMargin: DisplayCtrl.dp(5)
                font.family: googleMaterial.name
                font.pixelSize: DisplayCtrl.sp(background.height / 10)
                horizontalAlignment: Qt.AlignLeft
                color: "white"
                visible: root.device.modLight > 0
                text: DisplayCtrl.icModLight
            }
            Text{
                id: delay
                anchors.bottom: modLight.top
                anchors.left: bottomBackground.left
                anchors.leftMargin: DisplayCtrl.dp(5)
                font.family: googleMaterial.name
                font.pixelSize: DisplayCtrl.sp(background.height / 10)
                horizontalAlignment: Qt.AlignLeft
                color: "white"
                visible: root.device.delay > 0
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
                visible: root.device.sequence > 1
                text: DisplayCtrl.icSequence
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
                visible: root.device.remoteSwitch !== ParaSelectors.RcOff//root.device.delay > 0
                text: setText(root.device.remoteSwitch)
            }

            Text{
                id: txtExt
                anchors.fill: bottomBackground
                font.family: bronFontBold.name
                font.pixelSize: DisplayCtrl.sp(background.height / 10)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "white"//root.areDifferent ? "red" : "white"
                text: root.selectionMode ? (root.selected ? root.selectionText : "--") : "MORE"
            }

            MouseArea{
                id: lampButton
                width: parent.width
                height: parent.height  - bottomBackground.height
                anchors.top: parent.top
                onClicked: root.lampClicked(root)
                onPressAndHold: root.longClicked(root)
            }
        }

    // trigger read out of sw version form device, to start the mechanism that makes sure that device was read and actual
    Component.onCompleted: {
        device.swVersion = 0
    }
}
