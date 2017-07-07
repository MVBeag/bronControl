import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ch.bron.BronModels 1.0
import "."

FocusScope {
    id: root

    property string name
    property var actValue: 0
    property bool isPush: false
    property var nextFocusItem: Item{}
    property var prevFocusItem: Item{}
    onActValueChanged: {
        bt.isActive = actValue != 0
    }

    signal value(var result)

    RowLayout{
        id: line
        anchors.fill: parent
        spacing: 0
        Item{
            Layout.preferredWidth: root.width * 4 / 10
        }

        Item{
            Layout.fillWidth: true
        }

        Button{
            id: bt
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: root.width * 4/10
            Layout.preferredHeight: root.height
            text: name
            KeyNavigation.tab: nextFocusItem
            KeyNavigation.backtab: prevFocusItem

            focus: true
            isActive: root.actValue !== 0

            Keys.onReturnPressed: {
                var res = {val: bt.isActive, tag: bt.isActive.toString()}
                event.accepted = true
                root.value(res)
                if(isPush){
                    treset.start()
                }
            }

            onClicked:{
                bt.forceActiveFocus()
                var res = {val: bt.isActive, tag: bt.isActive.toString()}
                root.value(res)
                if(isPush){
                    treset.start()
                }
            }
            Timer{
                id: treset
                repeat: false
                interval: 200
                onTriggered: bt.isActive = false

            }
        }
    }
}
