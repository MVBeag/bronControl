import QtQuick 2.7
import QtQuick.Controls 2.0
import ch.bron.BronModels 1.0
import "."

ItemDelegate {
    id: control
    hoverEnabled: true
    contentItem: Text {
        rightPadding: control.spacing
        text: control.text
        font: control.font
        color: highlighted ? ((control.down || control.hovered) ? "black" : BronColors.bronBlue) : "white"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        visible: control.text
    }
    background: Rectangle {
        opacity: enabled ? 1 : 0.3
        color: (control.down || control.hovered) ? BronColors.bronBlue : "black"
    }
}
