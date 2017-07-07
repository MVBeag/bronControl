import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ch.bron.BronModels 1.0
import "."

FocusScope {
    id: root

    property string name
    property var validate: RegExpValidator{}

    property var selections
    property string selRole
    property var actValue
    property bool readonly: true

    property var nextFocusItem: Item{}
    property var prevFocusItem: Item{}

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
    function findTagFromValue(val,sel){
        if(Array.isArray(sel)){
            var i
            var length = sel.length
            for(i =  0; i < length; i++){
                if(val === sel[i].val){
                    return i
                }
            }

        }
        else{
            return val
        }
    }
    signal value(var result)

    RowLayout{
        id: line
        anchors.fill: parent
        spacing: 0
        TextField{
            id: lineName
            background: Rectangle{
                color: BronColors.bronBlack
            }

            Layout.preferredWidth: parent.width *4/10

            readOnly: true
            placeholderText: name + ":"
            text: name + ":"

            font.family: bronFontMedium.name
            font.bold: true
            font.pointSize: DisplayCtrl.point15Size

            color: "white"
        }
        Item{
            id: conn
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.preferredHeight: DisplayCtrl.dp(4)
        }
        ComboBox{
            id: input

            currentIndex: findTagFromValue(root.actValue, root.selections)

            Layout.preferredWidth: parent.width * 4/10
            focus: true

            KeyNavigation.tab: nextFocusItem
            KeyNavigation.backtab: prevFocusItem

            font.family: bronFontMedium.name
            font.bold: true
            font.pointSize: DisplayCtrl.point15Size

            textRole: selRole
            model: selections
            delegate: BronItemDelegate {
                width: input.width
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

//            contentItem: Text {
//                leftPadding: 0
//                rightPadding: input.indicator.width + input.spacing


//                text: input.displayText
//                font: input.font
//                color: "white"
//                horizontalAlignment: Text.AlignLeft
//                verticalAlignment: Text.AlignVCenter
//                elide: Text.ElideRight
//            }
            contentItem: TextField {
                leftPadding: 0
                rightPadding: input.indicator.width + input.spacing
                readOnly: readonly

                text: input.displayText
                font: input.font
                color: "white"
                background: Rectangle{
                    color: bg.color
                    border.color: "transparent"
                }

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
//                elide: Text.ElideRight
                onAccepted: {
                    var length = selections.count
                    var found = false
                    for(var i = 0; i < length; i++){
                        if(selections.get(i) === text){
                            found = true
                        }
                    }
                    if(false === found){
                        selections.append(name);
                    }
                }
            }


            background: Rectangle {
                id: bg
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
                    border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : input.activeFocus ? "white" : BronColors.bronBlue
                    border.width: DisplayCtrl.dp(4)
                    color: "black"
                }
            }

            onActivated:{
                var res = {val: root.findValFromTag(selections, currentText), tag: currentText}
                root.value(res)
            }

        }
    }

}
