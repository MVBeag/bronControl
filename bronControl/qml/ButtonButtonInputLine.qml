import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ch.bron.BronModels 1.0
import "."

FocusScope {
    id: root

    property string leftName
    property string rightName
    property var actValue
    property var actDValue
    property bool isPush: false
    property var nextFocusItem: Item{}
    property var prevFocusItem: Item{}


    signal value(var result)
    signal depSelector(var result)


    onFocusChanged: {
        if(focus){
            left.forceActiveFocus()
        }
    }


    RowLayout{
            id: line

            Layout.alignment: Qt.AlignHCenter
            spacing: 0
            anchors.fill: parent

            Button{
                id: left
                Layout.preferredWidth: root.width * 4/10
                Layout.preferredHeight: root.height
                Layout.alignment: Qt.AlignLeft
                text: leftName


                KeyNavigation.tab: right
                KeyNavigation.backtab: prevFocusItem

                Keys.onReturnPressed: {
                    var res = {val: left.isActive, tag: left.isActive.toString()}
                    event.accepted = true
                    root.value(res)
                    if(isPush){
                        leftReset.start()
                    }

                }

                isActive: root.actValue
                onClicked:{
                    var res = {val: left.isActive, tag: left.isActive.toString()}
                    root.value(res)
                    if(isPush){
                        leftReset.start()
                    }
                }
                Timer{
                    id: leftReset
                    repeat: false
                    interval: 200
                    onTriggered: left.isActive = false

                }
            }
            Item{
                Layout.fillWidth: true
            }
            Button{
                id: right
                Layout.preferredWidth: root.width * 4/10
                Layout.preferredHeight: root.height
                Layout.alignment: Qt.AlignRight
                text: rightName

                KeyNavigation.tab: nextFocusItem
                KeyNavigation.backtab: left
                Keys.onReturnPressed: {
                    var res = {val: right.isActive, tag: right.isActive.toString()}
                    event.accepted = true
                    root.depSelector(res)
                    if(isPush){
                        rightReset.start()
                    }
                }

                isActive: root.actDValue
                onClicked:{
                    var res = {val: right.isActive, tag: right.isActive.toString()}
                    root.depSelector(res)
                    if(isPush){
                        rightReset.start()
                    }
                }
                Timer{
                    id: rightReset
                    repeat: false
                    interval: 200
                    onTriggered: right.isActive = false

                }
            }

        }
}
