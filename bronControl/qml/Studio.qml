import QtQuick 2.7
import QtQuick.Controls 2.0
import ch.bron.BronModels 1.0
import "."


Item {
    id: studio

    width:  DisplayCtrl.itemSize
    height: DisplayCtrl.itemSize
//    transform: DisplayCtrl.userZoom

    property var number
    property alias studioName: nameText.text
    property var groupsModel
    property var devicesModel
    property bool wiggle: false
    property var netWorkState
    property var hasFocus: false

    enabled: studio.studioName === "Bron-Demo0" ? true : studio.netWorkState === StudioProxy.Active ? true : false
    opacity: enabled ? 1.0 : 0.7

    Rectangle {
            id: background
            anchors.fill: parent
            color: BronColors.bronBlue
            border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : hasFocus ? "white" : BronColors.bronBlue
            border.width: DisplayCtrl.dp(4)

            // text for energy, main information of this control in the middle
            Text {
                id: numberText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: DisplayCtrl.dp(-35)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: bronFontBold.name
                font.bold: true
                font.pixelSize: DisplayCtrl.sp(parent.height / 1.8)
                color: "white"
                text: studio.number === -1 ? "?" : studio.number.toFixed(0)

            }

            Rectangle{
                id: nameSection
                width: parent.width
                height: parent.height/4
                anchors.bottom: parent.bottom

                color: BronColors.bronBlueDark
                border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : hasFocus ? "white" : BronColors.bronBlue
                border.width: DisplayCtrl.dp(4)

                Text{
                    id: nameText
                    anchors.fill: parent
                    padding: DisplayCtrl.dp(4)

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.family: bronFont.name
                    font.bold: true
                    font.pointSize: DisplayCtrl.point20Size
                    minimumPointSize: Math.max(DisplayCtrl.point5Size,1)
                    fontSizeMode: Text.Fit
                    text: ""
                    color: "white"
                }
            }
    }

    onWiggleChanged:{
        if(studio.studioName !== "Bron-Demo0"){
            if(wiggle){
                wiggleAnimation.start()
            }
            else{
                wiggleAnimation.stop()
            }
        }
    }


    SequentialAnimation{
        id: wiggleAnimation
        loops: Animation.Infinite
        alwaysRunToEnd: true
        RotationAnimation { target: studio; property: "rotation"; from: 0; to: 3; duration: 50; direction: RotationAnimation.Clockwise }
        RotationAnimation { target: studio; property: "rotation"; from: 3; to: -3; duration: 100; direction: RotationAnimation.Counterclockwise }
        RotationAnimation { target: studio; property: "rotation"; from: -3; to: 0; duration: 50; direction: RotationAnimation.Clockwise }
    }


}
