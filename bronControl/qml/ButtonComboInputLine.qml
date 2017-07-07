import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ch.bron.BronModels 1.0
import "."

FocusScope {
    id: root

    property string name
    property var selections
    property string selRole
    property var actValue
    property var actDValue

    focus: true

    onFocusChanged: {
        if(focus){
            bt.forceActiveFocus()
        }
    }


    function findValFromTag(sel, tag){
        if(Array.isArray(sel)){
            var i
            var length = sel.length
            for(i = 0; i < length; i++){
                if(tag === (sel[i])[selRole])
                    return sel[i].val
            }
        }
        else{
            return undefined
        }
    }


    signal value(var result)
    signal depSelector(var result)

    RowLayout{
            id: line
            anchors.fill: parent
            spacing: 0
            Button{
                id: bt
                Layout.preferredWidth: parent.width * 4/10
                Layout.preferredHeight: root.height

                focus: true

                KeyNavigation.tab: input
                KeyNavigation.backtab: input
                Keys.onReturnPressed: {
                    var res = {val: bt.isActive, tag: bt.isActive.toString()}
                    event.accepted = true
                    root.value(res)
                }

                text: name
                isActive: root.actValue
                onClicked:{
                    var res = {val: bt.isActive, tag: bt.isActive.toString()}
                    bt.forceActiveFocus()
                    root.value(res)
                }
            }
            Rectangle{
                id: conn
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.preferredHeight: DisplayCtrl.dp(2)
                color: BronColors.bronBlue
            }
            ComboBox{
                id: input

                currentIndex: root.actDValue

                Layout.preferredWidth: parent.width * 4/10
                Layout.preferredHeight: root.height
                focus: true

                KeyNavigation.tab: bt
                KeyNavigation.backtab: bt

                font.family: bronFontMedium.name
                font.bold: true
//                font.pointSize: DisplayCtrl.point15Size
                font.pixelSize: height
                textRole: selRole
                model: selections
                delegate: BronItemDelegate {
                    width: input.width
                    height: input.height
                    text: Array.isArray(selections) ? modelData[selRole] : modelData
                    font: input.font
                    highlighted: input.highlightedIndex == index
                }
                indicator: Canvas {
                     id: canvas
                     x: input.width - width - input.rightPadding
                     y: input.topPadding + (input.availableHeight - height) / 2
                     width: DisplayCtrl.dp(12)
                     height: DisplayCtrl.dp(8)
                     contextType: "2d"

                     Connections {
                         target: input
                         onPressedChanged: canvas.requestPaint()
                     }

                     onPaint: {
                         context.reset();
                         context.moveTo(0, 0);
                         context.lineTo(width, 0);
                         context.lineTo(width / 2, height);
                         context.closePath();
                         context.fillStyle = input.pressed ? Qt.darker(BronColors.bronBlue, 2.0) : BronColors.bronBlue;
                         context.fill();
                     }
                 }
                contentItem: Text {
                    leftPadding: 0
                    rightPadding: input.indicator.width + input.spacing

                    text: input.displayText
                    font: input.font
                    color: "white"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                background: Rectangle {
                    color: "black"
                    border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : input.activeFocus ? "white" : BronColors.bronBlue
                    border.width: DisplayCtrl.dp(4)
                }
                popup: Popup {
                    y: input.height - 1
                    width: input.width
                    implicitHeight: listview.contentHeight
                    padding: 1

                    contentItem: ListView {
                        id: listview
                        clip: true
                        model: input.popup.visible ? input.delegateModel : null
                        currentIndex: input.highlightedIndex

                        ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        color: "black"
                        border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : input.activeFocus ? "white" : BronColors.bronBlue
                        border.width: DisplayCtrl.dp(4)
                    }
                }

                onActivated:{
                    var res = {val: root.findValFromTag(selections, currentText), tag: currentText}
                    root.depSelector(res)
                }
            }
        }
}
