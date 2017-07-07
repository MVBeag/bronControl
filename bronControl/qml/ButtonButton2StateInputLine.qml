import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ch.bron.BronModels 1.0
import "."

Item {
    id: root

    property string name
    property var selections: [{val: 0, tag: "left"},{val: 1, tag: "right"}]
    property var actValue
    property var actDValue

    signal value(var result)
    signal depSelector(var result)

    onFocusChanged: {
        if(focus){
            bt.forceActiveFocus()
        }
    }


    RowLayout{
            id: line
            anchors.fill: parent
            spacing: 0
            Button{
                id: bt
                Layout.preferredWidth: root.width * 4/10
                Layout.preferredHeight: root.height
                text: name
                isActive: root.actValue

                KeyNavigation.tab: bt2State
                KeyNavigation.backtab: bt2State
                Keys.onReturnPressed: {
                    var res = {val: bt.isActive, tag: bt.isActive.toString()}
                    event.accepted = true
                    root.value(res)
                }

                onClicked:{
                    var res = {val: bt.isActive, tag: bt.isActive.toString()}
                    root.value(res)
                }
            }
            Rectangle{
                id: conn
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: root.width * 2/10
                Layout.preferredHeight: DisplayCtrl.dp(2)
                color: BronColors.bronBlue
            }
            Button2State{
                id: bt2State
                Layout.preferredWidth: root.width * 4/10
                Layout.preferredHeight: root.height
                textleft: root.selections[0].tag
                textright: root.selections[1].tag
                isLeftActive: root.actDValue
                focus: true
                KeyNavigation.tab: bt
                KeyNavigation.backtab: bt
                Keys.onReturnPressed: {
                    var index
                    if(bt2State.isLeftActive){
                        index = 0
                    }
                    else{
                        index = 1
                    }
                    event.accepted = true
                    var res = {val: root.selections[index].val, tag: root.selections[index].tag}
                    root.depSelector(res)
                }

                onClicked: {
                    var index
                    if(bt2State.isLeftActive){
                        index = 0
                    }
                    else{
                        index = 1
                    }

                    var res = {val: root.selections[index].val, tag: root.selections[index].tag}
                    root.depSelector(res)
                }
            }
        }

}
