import QtQuick 2.7
import "."


Rectangle{
    id: helpFrame

    property var hidden: Item{}
    property alias text: help.text
    property alias horizontalAlignment: help.horizontalAlignment
    signal clicked()

    x:  hidden.x
    y:  hidden.y
    width: hidden.width
    height: hidden.height
//    border.color: "white"
//    border.width: DisplayCtrl.dp(2)
    color: "transparent"
    opacity: 1

    Text {
        id: help

        anchors.fill: parent
        opacity: 1
    //    readOnly: true
        focus: false
        font.family: bronFontMedium.name
        font.pointSize: DisplayCtrl.point25Size
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        fontSizeMode: Text.Fit
        minimumPointSize: Math.min(DisplayCtrl.point5Size,1)
        wrapMode: Text.WordWrap
        text: qsTr("define help text")
        color: "white"
        MouseArea{
            anchors.fill: parent
            onClicked: helpFrame.clicked()
        }
    }
}
