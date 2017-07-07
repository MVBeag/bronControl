import QtQuick 2.6
import QtQuick.Controls 2.0
import "."

BusyIndicator {
    id: control
    property var size: DisplayCtrl.dp(120)

    contentItem: Item {
        implicitWidth: size
        implicitHeight: size

        Item {
            id: item
            x: parent.width / 2 - size/2
            y: parent.height / 2 - size/2
            width: size
            height: size
            opacity: control.running ? 1 : 0

            Behavior on opacity {
                OpacityAnimator {
                    duration: 250
                }
            }

            RotationAnimator {
                target: item
                running: control.visible && control.running
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 1250
            }

            Repeater {
                id: repeater
                model: 1

                Rectangle {
                    x: item.width / 2 - width / 2
                    y: item.height / 2 - height / 2
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 5
                    color: BronColors.bronBlue
                    transform: [
                        Translate {
                            y: -Math.min(item.width, item.height) * 0.5 + 5
                        },
                        Rotation {
                            angle: index / repeater.count * 360
                            origin.x: 5
                            origin.y: 5
                        }
                    ]
                }
            }
        }
    }
}
