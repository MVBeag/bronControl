import QtQml 2.2
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import ch.bron.BronModels 1.0
import "."

FocusScope{
    id: root

    property string name
    property string unit: ""
    property var validate: RegExpValidator{}
    property bool isPW: false
    property bool readOnly: false
    property var actValue
    property int decPlaces: -1
    property var nextFocusItem: Item{}
    property var prevFocusItem: Item{}
    property alias hints: lineInput.inputMethodHints

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

            Layout.preferredWidth: parent.width * 4/10
            focus: false
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
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
        }
        TextField{
            id: lineInput
            background: Rectangle{
                color: BronColors.bronBlack//"black"
                //                    border.color: lineInput.activeFocus ? "white" : BronColors.bronBlue
            }

            placeholderText: decPlaces === -1 ? root.actValue : root.actValue.toFixed(decPlaces)
            readOnly: root.readOnly

            Layout.preferredWidth: parent.width *4 / 10
            KeyNavigation.tab: nextFocusItem
            KeyNavigation.backtab: prevFocusItem

//            Keys.onReturnPressed: {
//                console.log("enter pressed")
//                Qt.inputMethod.hide()
//                if(acceptableInput){
//                    console.log("value " + lineInput.text)
//                    var res = {val: lineInput.text, tag: lineInput.text}
//                    root.value(res)
//                    nextFocusItem.forceActiveFocus()
//                }
//            }

            font.family: bronFontMedium.name
            font.bold: true
            font.pointSize: DisplayCtrl.point15Size
            focus: !readOnly
            echoMode: isPW ? TextInput.PasswordEchoOnEdit : TextInput.Normal
            passwordMaskDelay: 1
            passwordCharacter: "*"

            validator: root.validate
            color: lineInput.activeFocus ? BronColors.bronBlue : "white"

            property bool isInEdit:false
            onActiveFocusChanged: {
                if(activeFocus){
                    isInEdit = true
                }
                else{
                    Qt.inputMethod.hide()
                }
            }

//            onAccepted: {
//                var loc = Qt.locale()
//                var res = {val: Number.fromLocaleString(loc,lineInput.text), tag: lineInput.text}
//                root.value(res)
//                nextFocusItem.forceActiveFocus()
//                Qt.inputMethod.hide()
//            }
            onEditingFinished: {
                if(isInEdit){
                    isInEdit = false
                    var loc = Qt.locale()
                    var res = {val: Number.fromLocaleString(loc,lineInput.text), tag: lineInput.text}
                    root.value(res)
                    nextFocusItem.forceActiveFocus()
                    Qt.inputMethod.hide()
                }
            }

            TextField{
                id: unitName

                anchors.right: parent.right
                background: Rectangle{
                    color: "transparent"//BronColors.bronBlack
                }

                visible: root.unit === "" ? false : true

                readOnly: true
                placeholderText: root.unit
                text: root.unit

                font.family: bronFontMedium.name
                font.bold: true
                font.pointSize: DisplayCtrl.point15Size

                color: "white"
            }

        }
        //            TextField{
        //                id: unitName
        //                background: Rectangle{
        //                    color: "transparent"//BronColors.bronBlack
        //                }

        //                visible: root.unit === "" ? false : true
        //                Layout.preferredWidth: root.width/10

        //                readOnly: true
        //                placeholderText: root.unit
        //                text: root.unit

        //                font.family: bronFont.name
        //                font.pointSize: DisplayCtrl.point20Size

        //                color: "white"
        //            }
    }
    //    }
}
