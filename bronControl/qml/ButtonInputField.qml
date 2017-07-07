import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ch.bron.BronModels 1.0
import "."

FocusScope {
    id: root

    property string name
    property string unit
    property var actValue
    property var actDValue
    property var validate: RegExpValidator{}
    property var nextFocusItem: Item{}
    property var prevFocusItem: Item{}

    onActValueChanged:{
        bt.isActive = actValue
    }

    signal value(var result)
    signal depSelector(var result)

    focus: true
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
            Layout.preferredWidth: line.width * 4/10
            Layout.preferredHeight: line.height
            KeyNavigation.tab: lineInput
            KeyNavigation.backtab: prevFocusItem
            Keys.onReturnPressed:{
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
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: DisplayCtrl.dp(4)
            color: BronColors.bronBlue
        }
        TextField{
            id: lineInput
            background: Rectangle{
                color: "black"
                border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : lineInput.activeFocus ? "white" : BronColors.bronBlue
                border.width: DisplayCtrl.dp(4)
            }

            placeholderText: actDValue
            Layout.preferredWidth: line.width * 4/10

            KeyNavigation.tab: nextFocusItem
            KeyNavigation.backtab: bt

            font.family: bronFontMedium.name
            font.pointSize: DisplayCtrl.point15Size

            validator: root.validate
            color: "white"

            onAccepted: {
                var res = {val: text, tag: text}
                root.depSelector(res)
            }
        }
    }
}
