import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "."

Rectangle{
    id: settingsBG
    color: BronColors.bronBlack
    
    signal hide()
    signal accept()
    signal encryption(var encr)
    signal ssid(var ssid)
    signal pw(var pw)

    ColumnLayout{
        id: enterprizeSettingsLayout

        width: parent.width * 0.8
//        height: DisplayCtrl.dp(60) * 4

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: DisplayCtrl.dp(10)

//        Button2State{
//            id: encyBt

//            Layout.preferredWidth: parent.width
//            Layout.alignment: Qt.AlignHCenter
//            Layout.preferredHeight: DisplayCtrl.dp(60)

//            isLeftActive: true
//            textleft: "WPA"
//            textright: "WPA2"
//            onClicked: {
//                isLeftActive ? settingsBG.encryption("WPA") : settingsBG.encryption("WPA2")
//            }
//        }
        Text{
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: DisplayCtrl.dp(60)
            font.family: bronFont.name
            font.pointSize: DisplayCtrl.point15Size
            color: "white"
            text: "SELECT NETWORKs"

        }

        ComboBox{
            id: input

            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter

            currentIndex: 0

            focus: true

            font.family: bronFont.name
            font.pointSize: DisplayCtrl.point15Size

            displayText: "SELECT NETWORKss"
            textRole: "name"
            model: netScanner
            delegate: BronItemDelegate {
                width: input.width
                text: modelData
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
                border.color: BronColors.bronBlue
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
                    border.color: BronColors.bronBlue
                }
            }

            onActivated: {
                displayText = currentText
                settingsBG.ssid(currentText)
                pw.enabled = true
                pw.text = studioMgr.getPossiblePassword(currentText)
                settingsBG.pw(pw.text)
            }
//          Component.onCompleted: settingsBG.ssid(currentText)
        }
        TextField{
            id: pw
            background: Rectangle{
                color: "black"
                border.color: BronColors.bronBlue
            }

            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter

            enabled: false
            opacity: enabled ? 1.0 : 0.5

            color: "white"

            placeholderText: "Password"
            font.family:  bronFontMedium.name
            font.pointSize: DisplayCtrl.point15Size

            echoMode: TextInput.Password
            passwordCharacter: "*"
            passwordMaskDelay: 500

            onAccepted:{
                settingsBG.pw(text)
                accept()
            }

            Text{
                id: shwoPw
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: DisplayCtrl.gridMargin
                font.family: googleMaterial.name
                font.pixelSize: pw.height * 0.8
                color: "white"
                opacity: 0.7
                text: DisplayCtrl.icVisibility
                MouseArea{
                    anchors.fill: parent
                    onEntered: pw.echoMode = TextInput.Normal
                    onExited: pw.echoMode = TextInput.Password
                }
            }
        }
        Text{
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: DisplayCtrl.dp(20)
            font.family: bronFont.name
            font.pointSize: DisplayCtrl.point15Size
            color: "white"
            text: "take care that the UDP port 8185\nis open at the router"

        }


    }
    Rectangle{
        width: parent.width
        height: parent.height - enterprizeSettingsLayout.height
        anchors.top: enterprizeSettingsLayout.bottom
        anchors.left: parent.left
        color: "transparent"
        MouseArea{
            anchors.fill: parent
            onPressed: hide()
        }
    }
}
