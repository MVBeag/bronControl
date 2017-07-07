import QtQuick 2.7
import QtQuick.Controls 2.0
import "."

ComboBox{
    id: cbMore
    signal selected()

    property bool isOpen: false

    focus: true
    MouseArea{
        anchors.fill: parent
        onClicked: {
            forceActiveFocus()
            if(popup.visible)  popup.close()
            else popup.open()
        }
    }

    padding: 0
    hoverEnabled: true
    delegate: BronItemDelegate {
        width: cbMore.width
        height: cbMore.height
        text: modelData
        font: cbMore.font
        highlighted: cbMore.highlightedIndex == index
    }

    indicator: Canvas {
        id: canvas
        x: cbMore.width - width - cbMore.rightPadding
        y: cbMore.topPadding + (cbMore.availableHeight - height) / 2
        width: DisplayCtrl.dp(20)// 12
        height: DisplayCtrl.dp(20/3*2) // 8
        contextType: "2d"

        onPaint: {
            if(cbMorePopup.visible){
                context.reset();
                context.moveTo(width, height);
                context.lineTo(width / 2, 0);
                context.lineTo(0, height);
                context.closePath();
                context.fillStyle = "white";
                context.fill();
            }
            else{
                context.reset();
                context.moveTo(0, 0);
                context.lineTo(width, 0);
                context.lineTo(width / 2, height);
                context.closePath();
                context.fillStyle = "white";
                context.fill();
            }
        }
    }

    contentItem: Text {
        leftPadding: 0//cbMore.mirrored && cbMore.indicator ? cbMore.indicator.width + cbMore.spacing : 0
        rightPadding: cbMore.indicator.width + cbMore.spacing

        text: cbMore.displayText
        font: cbMore.font
        color: "white"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        opacity: enabled ? 1 : 0.3
    }

    background: Rectangle {
        color: "black"
        border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : cbMore.activeFocus ? "white" : BronColors.bronBlue
        border.width: DisplayCtrl.dp(4)
    }

    popup: Popup {
        id: cbMorePopup
        y: cbMore.height - (cbMore.visualFocus ? 0 : 1)
        width: cbMore.width
        implicitHeight: listview.contentHeight
        topMargin: 6
        bottomMargin: 6
        padding: 1

        onOpened: cbMore.isOpen = true
        onClosed: cbMore.isOpen = false

        onVisibleChanged: canvas.requestPaint()

        contentItem: ListView {
            id: listview
            clip: true
            model: cbMore.popup.visible ? cbMore.delegateModel : null
            currentIndex: cbMore.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            color: "black"
            border.width: DisplayCtrl.dp(4)
            border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : cbMore.activeFocus ? "white" : BronColors.bronBlue
        }
    }

    onHighlighted: {
        forceActiveFocus()
        currentIndex = index
    }
}
