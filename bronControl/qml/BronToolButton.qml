import QtQuick 2.7
import QtQuick.Controls 2.0
import ch.bron.BronModels 1.0
import "."

ToolButton{
    id: control

    property color focusColor: BronColors.bronBlue
    property bool hide: false

    property real impHeight: ctext.contentHeight*8.5/10

    property color unselectColor: "white"

    highlighted: activeFocus===true

    padding: 0
    focus: true
    font.pointSize: DisplayCtrl.isMobilePlattform ? DisplayCtrl.point50Size : DisplayCtrl.point25Size
    contentItem: Text {
        id: ctext
        text: control.text
        font: control.font
        anchors.fill: parent
        padding: 0
        // change color in focus
        color: hide ? "transparent" : control.activeFocus ? focusColor : unselectColor
        fontSizeMode: Text.Fit
        minimumPointSize: Math.max(DisplayCtrl.point5Size,1)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideMiddle
    }

    // make background not to change in case of focus
    background: Rectangle{
        anchors.fill: parent
        color: "transparent"
    }
}
