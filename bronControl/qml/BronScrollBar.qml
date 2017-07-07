import QtQuick 2.7
import QtQuick.Controls 2.1
import "."

ScrollBar{
    id: control

    property bool show: false

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                                         contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
                                          contentItem.implicitHeight + topPadding + bottomPadding)

    contentItem: Rectangle {
        id: sbself
        implicitWidth: DisplayCtrl.dp(5)
        implicitHeight: DisplayCtrl.dp(5)
        radius: width/2

        color: control.pressed ? BronColors.bronBlue :
               control.hovered ? BronColors.bronBlue: "white"

        visible: control.size < 1.0
        opacity: 0.0
    }

    background: Rectangle {
        implicitWidth: DisplayCtrl.dp(5)
        implicitHeight: DisplayCtrl.dp(5)

        color: "transparent"
        visible: control.size < 1.0
        opacity: 0.0
    }

    states: [
        State {
            name: "active"
            when: control.active || control.show
        }
    ]

    transitions: [
        Transition {
            to: "active"
            NumberAnimation { targets: sbself; property: "opacity"; to: 1.0 }
        },
        Transition {
            from: "active"
            SequentialAnimation {
                PauseAnimation { duration: 450 }
                NumberAnimation { targets: sbself; property: "opacity"; to: 0.0 }
            }
        }
    ]

}
