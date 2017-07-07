import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import "."


Item{
    id: settings
    signal back()
    signal switchStudioMgrView(var frontside)

    Rectangle{
        id: cmpSettings
        color: BronColors.bronBlack
        width: parent.width * 2 / 3
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left

        Rectangle{
            id: settingsFrame
            anchors.top: parent.top
            width: parent.width
            height: DisplayCtrl.dp(70)
            color: BronColors.bronBlue
            BronToolButton{
                id: btQuit
                anchors.verticalCenter: settingsFrame.verticalCenter
                anchors.right: parent.right
                font.family: googleMaterial.name
                font.pointSize: DisplayCtrl.point30Size
//                Keys.onTabPressed: standbyLine.forceActiveFocus()
//                Keys.onBacktabPressed: standbyLine.forceActiveFocus()
                Keys.onReturnPressed:{
                    event.accepted = true
                    settings.back()
                }
                text: DisplayCtrl.icClose
                onClicked: settings.back()
            }

        }
        Rectangle{
            id: settingsFrameEnd
            anchors.top: settingsFrame.bottom
            width: parent.width
            height: DisplayCtrl.dp(2)
            color: "white"
        }

        Rectangle{
            id: generalInfo

            anchors.top: settingsFrame.bottom
            anchors.topMargin: DisplayCtrl.dp(10)
            anchors.left: parent.left

            width: parent.width
            height: txtGeneral.contentHeight

            property alias txtColor: txtGeneral.color

            color: BronColors.bronBlack
            Text{
                id: txtGeneral
                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size
                color: "white"
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                maximumLineCount: 1

                text: "GENERAL INFORMATION"
            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: generalInfo.color = Qt.darker(generalInfo.color, 2.0)
                onExited: generalInfo.color = Qt.lighter(generalInfo.color, 2.0)
                onPressed: generalInfo.txtColor = Qt.darker(generalInfo.txtColor, 2.0)
                onReleased: generalInfo.txtColor = Qt.lighter(generalInfo.txtColor, 2.0)
                onClicked:{
                    generalLoader.setSource("qrc:///qml/GeneralInfo.qml")
                }
            }
            Rectangle{
                width: parent.width
                height: DisplayCtrl.dp(1)
                anchors.bottom: parent.bottom
                color: "white"
            }
        }
//        Rectangle{
//            id: showStudioMgr

//            anchors.top: generalInfo.bottom
//            anchors.topMargin: DisplayCtrl.dp(10)
//            anchors.left: parent.left

//            width: parent.width
//            height: txtStudioMgr.contentHeight

//            property alias txtColor: txtStudioMgr.color

//            color: BronColors.bronBlack
//            Text{
//                id: txtStudioMgr
//                font.family: bronFontMedium.name
//                font.pointSize: DisplayCtrl.point20Size
//                color: "white"
//                leftPadding: parent.width * 0.05
//                elide: Text.ElideRight
//                maximumLineCount: 1

//                text: "SHOW STUDIO MANAGER"
//            }

//            MouseArea{
//                anchors.fill: parent
//                hoverEnabled: true
//                onEntered: showStudioMgr.color = Qt.darker(showStudioMgr.color, 2.0)
//                onExited: showStudioMgr.color = Qt.lighter(showStudioMgr.color, 2.0)
//                onPressed: showStudioMgr.txtColor = Qt.darker(showStudioMgr.txtColor, 2.0)
//                onReleased: showStudioMgr.txtColor = Qt.lighter(showStudioMgr.txtColor, 2.0)
//                onClicked:{
//                    settings.switchStudioMgrView(false)
//                }
//            }
//            Rectangle{
//                width: parent.width
//                height: DisplayCtrl.dp(1)
//                anchors.bottom: parent.bottom
//                color: "white"
//            }
//        }

    }

    Rectangle{
        id: background
        width: parent.width - cmpSettings.width
        height: parent.height

        anchors.top: parent.top
        anchors.left: cmpSettings.right

        color: "transparent"
        MouseArea{
            anchors.fill: parent
            onClicked: settings.back()
        }
    }

    Loader{
        id: networkLoader
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        x: -width
        onLoaded: settings.state = "network"

        function clearLoader(){
            networkLoader.source = ""
        }
    }

    Connections{
        target: networkLoader.item
        onBack:{
            settings.state = ""
        }
    }

    Loader{
        id: generalLoader
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        x: -width

        onLoaded: settings.state = "general"
        function clearLoader(){
            generalLoader.source = ""
        }
    }
    Connections{
        target: generalLoader.item
        onBack:{
            settings.state = ""
        }
    }

    states:[
        State{
            name: "general"
            PropertyChanges {
                target: generalLoader
                x: 0
            }
        },
        State{
            name: "network"
            PropertyChanges {
                target: networkLoader
                x: 0
            }
        }

    ]

    transitions: [
        Transition{
            from: ""; to: "general"
            NumberAnimation { target: generalLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
        },
        Transition{
            from: "general"; to: ""
            SequentialAnimation{
                NumberAnimation { target: generalLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
                ScriptAction{ script: generalLoader.clearLoader()}
            }

        },
        Transition {
            from: ""; to: "network"
            NumberAnimation{ target: networkLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
        },
        Transition {
            from: "network"; to: ""
            SequentialAnimation{
                NumberAnimation{ target: networkLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
                ScriptAction{ script: networkLoader.clearLoader()}
            }
        }
    ]




}
