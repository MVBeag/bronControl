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
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: DisplayCtrl.dp(20)

        Text{
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: DisplayCtrl.dp(60)
            font.family: bronFont.name
            font.pointSize: DisplayCtrl.point15Size
            color: "white"
            text: "SELECT NETWORKsss"

        }

        TextField{
            id: input

            background: Rectangle{
                color: "black"
                border.color: BronColors.bronBlue
            }

            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter

            color: "white"
            placeholderText: "NETWORK?"
            font.family: bronFontMedium.name
            font.pointSize: DisplayCtrl.point15Size

            onTextChanged: {
                pw.text = studioMgr.getPossiblePassword(text)
            }

            onEditingFinished: {
                settingsBG.ssid(text)
                pw.enabled = true
            }
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

            echoMode: showPw.checked ? TextInput.Normal : TextInput.Password
            passwordCharacter: "*"
            passwordMaskDelay: 500

            onEditingFinished: {
                settingsBG.pw(text)
//                accept()
            }

        }
        CheckBox{
            id: showPw
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: DisplayCtrl.dp(60)
            checked: false
            text: "show Password"
            font.family:  bronFontMedium.name
            font.pointSize: DisplayCtrl.point15Size

            background: Rectangle{
                anchors.fill: parent
                color: BronColors.bronBlack
            }

            indicator: Rectangle {
                implicitHeight: parent.height * 0.8
                implicitWidth: implicitHeight
                x: showPw.leftPadding
                y: parent.height / 2 - height / 2
                color: BronColors.bronBlue

                Rectangle {
                    width: parent.width * 2 /3
                    height: width
                    x: parent.width / 6
                    y: x
                    radius: 2
                    color: "black"
                    visible: showPw.checked
                }
            }

            contentItem: Text {
                text: showPw.text
                font: showPw.font
                opacity: enabled ? 1.0 : 0.3
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                leftPadding: showPw.indicator.width + showPw.spacing
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
