import QtQuick 2.7
import ch.bron.Globals 1.0
import "."


Item {
    id: root

    signal back()

    Rectangle{
        id: visualSide

        width: parent.width * 2 / 3
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left

        color: BronColors.bronBlack

        Rectangle{
            id: settingsFrame
            anchors.top: parent.top
            width: parent.width
            height: DisplayCtrl.dp(70)
            color: BronColors.bronBlue
            BronToolButton{
                id: btQuit
                anchors.verticalCenter: settingsFrame.verticalCenter
                anchors.right: parent.right
                font.family: googleMaterial.name
                font.pointSize: DisplayCtrl.point30Size
                focusColor: "white"

//                Keys.onTabPressed: standbyLine.forceActiveFocus()
//                Keys.onBacktabPressed: standbyLine.forceActiveFocus()
                Keys.onReturnPressed:{
                    event.accepted = true
                    root.back()
                }
                text: DisplayCtrl.icClose
                onClicked: root.back()
            }

        }
        Rectangle{
            id: settingsFrameEnd
            anchors.top: settingsFrame.bottom
            width: parent.width
            height: DisplayCtrl.dp(2)
            color: "white"
        }



        Text{
            id: versionText

            anchors.top: settingsFrameEnd.bottom
            anchors.topMargin: DisplayCtrl.dp(10)
            anchors.left: parent.left

            font.family: bronFontMedium.name
            font.pointSize: DisplayCtrl.point20Size
            color: "white"
            leftPadding: parent.width * 0.05
            elide: Text.ElideRight
            fontSizeMode: Text.HorizontalFit
            maximumLineCount: 1
            text: "App Information"
        }
        Text{
            id: version

            anchors.top: versionText.bottom
            anchors.left: parent.left

            font.family: bronFontLight.name
            font.pointSize: DisplayCtrl.point15Size
            color: "white"
            leftPadding: parent.width * 0.05
            elide: Text.ElideRight
            fontSizeMode: Text.HorizontalFit
            maximumLineCount: 1
            text: "bronControl " + Globals.version
        }

        Text{
            id: linkText
            anchors.top: version.bottom
            anchors.topMargin: DisplayCtrl.dp(50)
            anchors.left: parent.left
            font.family: bronFontMedium.name
            leftPadding: parent.width * 0.05

            font.pointSize: DisplayCtrl.point20Size
            linkColor: BronColors.bronBlue
            text: "<a href=\"http://www.bron.ch\">broncolor Homepage</a>"
            onLinkActivated: {
                Qt.openUrlExternally(link)
            }
        }
    }
    Rectangle{
        width: parent.width - visualSide.width
        height: parent.height
        anchors.top: parent.top
        anchors.left: visualSide.right
        color: "transparent"
        MouseArea{
            anchors.fill: parent
            onPressed: back()
        }
    }
}
