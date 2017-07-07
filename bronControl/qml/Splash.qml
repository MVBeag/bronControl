import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import ch.bron.Globals 1.0
import "."


Page {
    id: root
    property var stdMgr
    property Item focusItem: this

    background: Rectangle{
        id:bg
        color: BronColors.bronBlack
    }

    Image{
        id: bronLogo
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        width: DisplayCtrl.portraitWidth/15
        NumberAnimation on width{
            from: DisplayCtrl.portraitWidth/15
            to: DisplayCtrl.portraitWidth/2
            duration: 1000
        }
        source:"qrc:///res/bron_logo.png"
    }

    Text{
        id: versionText
        anchors.left: parent.left
        anchors.leftMargin: DisplayCtrl.gridMargin
        anchors.bottom: parent.bottom
        color: "white"
        font.pointSize: DisplayCtrl.point20Size
        font.family: bronFontMedium.name
        text: "Version: " + Globals.version
    }

    Timer{
        id: pageTimer
        interval: 2000
        running: false
        onTriggered: stack.replace(Qt.resolvedUrl("Studios.qml"), {"studioMgr": root.stdMgr})
    }

    Component.onCompleted: {
        pageTimer.start()
    }
}
