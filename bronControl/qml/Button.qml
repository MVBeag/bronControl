import QtQuick 2.6
import "."

FocusScope {
    id: root

    width:  DisplayCtrl.itemSize
    height: DisplayCtrl.dp(60)

    property alias text: label.text
    signal clicked(var anObject)

    property bool isActive: false
    property bool areDifferent: false
    property color backgroundColor: BronColors.bronBlue
    property bool zoomAllowed: false
    property bool visited: false

    focus: true


    onAreDifferentChanged: {
        if(isActive == true){
            background.color = root.areDifferent ? "red" : backgroundColor
            label.color = "black"
        }
        else{
            background.color = root.areDifferent ? "red" : "black"
            label.color = "white"
        }
    }

    onIsActiveChanged: {
        if(isActive == true){
            background.color = backgroundColor
            label.color = "black"
        }
        else{
            background.color = "black"
            label.color = "white"
        }
    }

    Rectangle {
        id: background

        anchors.fill: parent
        color: "black"
        border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : parent.activeFocus ? "white" : BronColors.bronBlue
        border.width: DisplayCtrl.dp(4)
        opacity: root.enabled ? 1.0 : 0.5

        focus: true
        Text {
            id: label
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            padding: DisplayCtrl.dp(2)
            font.family: bronFontMedium.name
            font.pointSize: DisplayCtrl.point15Size * (zoomAllowed ? DisplayCtrl.userScale : 1.0)
            font.bold: true
            color: "white"
            text: "MOD. LIGHT"
        }

        Keys.onReturnPressed: {
            if(isActive == false){
                background.color = root.areDifferent ? "red" : backgroundColor
                label.color = "black"
            }
            else{
                background.color = root.areDifferent ? "lightred" : "black"
                label.color = "white"
            }
            isActive = !isActive
            root.clicked(root)
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked:{
                if(isActive == false){
                    background.color = root.areDifferent ? "red" : backgroundColor
                    label.color = "black"
                }
                else{
                    background.color = root.areDifferent ? "lightred" : "black"
                    label.color = "white"
                }
                isActive = !isActive
                root.clicked(root)
            }
            onEntered: root.visited = true
            onExited: root.visited = false
        }
    }
}
