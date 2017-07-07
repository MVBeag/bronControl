import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import "../qml"

Item{
    id: settings
    property var studio
    property int apertureMode:  Selectables.fStopRange
    property int cognitionEnable: studio.cogniEnable

    signal back()


    Rectangle{
        id: cmpSettings
        color: BronColors.bronBlack
        width: parent.width * 2 / 3
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left

        // filter out mouse events
        MouseArea{
            anchors.fill: parent
            onClicked: {}
        }


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
            id: nwSettingsFrame

            anchors.top: settingsFrameEnd.bottom
            anchors.topMargin: DisplayCtrl.dp(10)
            anchors.left: parent.left

            width: parent.width
            height: txtNetworkSettings.contentHeight

            property alias txtColor: txtNetworkSettings.color

            color: BronColors.bronBlack
            Text{
                id: txtNetworkSettings
                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size
                color: "white"
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                fontSizeMode: Text.HorizontalFit
                maximumLineCount: 1

                text: "NETWORK SETTINGS"
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: nwSettingsFrame.color = Qt.darker(nwSettingsFrame.color, 2.0)
                onExited: nwSettingsFrame.color = Qt.lighter(nwSettingsFrame.color, 2.0)
                onPressed: nwSettingsFrame.txtColor = Qt.darker(nwSettingsFrame.txtColor, 2.0)
                onReleased: nwSettingsFrame.txtColor = Qt.lighter(nwSettingsFrame.txtColor, 2.0)
                onClicked:{
                    networkLoader.setSource("qrc:///qml/NetworkSettings.qml",
                                            {"address" : studio.address,
                                             "studio"  : studio})
                }
            }
            Rectangle{
                width: parent.width
                height: DisplayCtrl.dp(1)
                anchors.bottom: parent.bottom
                color: "white"
            }
        }
        Rectangle{
            id: cognitionLightSwitch

            property alias txtColor: cognitionLightModeTxt.color

            anchors.top: nwSettingsFrame.bottom
            anchors.left: parent.left

            width: parent.width
            height: txtCognitionLight.contentHeight

            color: BronColors.bronBlack
            Text{
                id: txtCognitionLight
                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size
                color: "white"
                width: parent.width/2
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                maximumLineCount: 2

                text: "COGNITION LIGHT"
            }
            Text{
                id: cognitionLightModeTxt

                anchors.right: parent.right

                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size

                rightPadding: parent.width * 0.05
                color: cognitionEnable === 1 ? BronColors.bronBlue : "white"
                text:  cognitionEnable === 0 ? "OFF" : "ON"
            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: cognitionLightSwitch.color = Qt.darker(cognitionLightSwitch.color, 2.0)
                onExited: cognitionLightSwitch.color = Qt.lighter(cognitionLightSwitch.color, 2.0)
                onPressed: cognitionLightSwitch.txtColor = Qt.darker(cognitionLightSwitch.txtColor, 2.0)
                onReleased: cognitionLightSwitch.txtColor = Qt.lighter(cognitionLightSwitch.txtColor, 2.0)
                onClicked:{
                    if(cognitionEnable === 0){
                        cognitionEnable = 1
                        studio.cogniEnable = 1
                    }
                    else{
                        cognitionEnable = 0
                        studio.cogniEnable = 0
                    }
                }
            }
            Rectangle{
                width: parent.width
                height: DisplayCtrl.dp(1)
                anchors.bottom: parent.bottom
                color: "white"
            }
        }
        Rectangle{
            id: apertureModeSwitch

            property alias txtColor: apertureModeTxt.color

            anchors.top: cognitionLightSwitch.bottom
            anchors.left: parent.left

            width: parent.width
            height: txtApertureMode.contentHeight

            color: BronColors.bronBlack
            Text{
                id: txtApertureMode
                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size
                color: "white"
                width: parent.width/2
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                maximumLineCount: 2

                text: "APERTURE DISPLAY MODE"
            }
            Text{
                id: apertureModeTxt

                anchors.right: parent.right

                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size

                rightPadding: parent.width * 0.05
                color: "white"
                text:  apertureMode === Selectables.fStopRange ? "f-stop" : "J"
            }

            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: apertureModeSwitch.color = Qt.darker(apertureModeSwitch.color, 2.0)
                onExited: apertureModeSwitch.color = Qt.lighter(apertureModeSwitch.color, 2.0)
                onPressed: apertureModeSwitch.txtColor = Qt.darker(apertureModeSwitch.txtColor, 2.0)
                onReleased: apertureModeSwitch.txtColor = Qt.lighter(apertureModeSwitch.txtColor, 2.0)
                onClicked:{
                    if(apertureMode === Selectables.fStopRange){
                        apertureMode = Selectables.energy
                    }
                    else{
                        apertureMode = Selectables.fStopRange
                    }
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
//            id: audioFrame

//            anchors.top: cogniLightFrame.bottom
//            anchors.left: parent.left

//            width: parent.width
//            height: txtAudio.contentHeight

//            property alias txtColor: txtAudio.color

//            color: BronColors.bronBlack
//            Text{
//                id: txtAudio
//                font.family: bronFontMedium.name
//                font.pointSize: DisplayCtrl.point20Size
//                color: "white"
//                width: parent.width/2
//                leftPadding: parent.width * 0.05
//                elide: Text.ElideRight
//                wrapMode: Text.WordWrap
//                maximumLineCount: 1

//                text: "AUDIO"
//            }
//            Text{
//                id: audioOnOffTxt

//                anchors.right: parent.right

//                font.family: bronFontMedium.name
//                font.pointSize: DisplayCtrl.point20Size

//                property var audio: studio.audio
//                rightPadding: parent.width * 0.05
//                color: audio === 0 ? "white" : (audio === 1 ? BronColors.bronBlue : "red")
//                text:  audio === 0 ? "OFF" : (audio === 1 ? "ON" : "DIFF")
//            }

//            MouseArea{
//                anchors.fill: parent
//                hoverEnabled: true
//                onEntered: audioFrame.color = Qt.darker(audioFrame.color, 2.0)
//                onExited: audioFrame.color = Qt.lighter(audioFrame.color, 2.0)
//                onPressed: audioFrame.txtColor = Qt.darker(audioFrame.txtColor, 2.0)
//                onReleased: audioFrame.txtColor = Qt.lighter(audioFrame.txtColor, 2.0)
//                onClicked:{
//                    switch(studio.audio){
//                    case 0:
//                        studio.audio = 1
//                        break
//                    default:
//                        studio.audio = 0
//                        break
//                    }
//                }
//            }
//            Rectangle{
//                width: parent.width
//                height: DisplayCtrl.dp(1)
//                anchors.bottom: parent.bottom
//                color: "white"
//            }
//        }

        Rectangle{
            id: generalInfo

            anchors.top: apertureModeSwitch.bottom
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

        Rectangle{
            id: userZoom

            anchors.top: generalInfo.bottom
            anchors.left: parent.left

            width: parent.width
            height: childrenRect.height//(txtZoom.contentHeight + zoomTxt.padding * 2) * 2

            property alias txtColor: txtZoom.color

            color: BronColors.bronBlack
            Text{
                id: txtZoom
                anchors.top: parent.top
                anchors.left: parent.left
                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size
                color: "white"
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                maximumLineCount: 1

                text: "ZOOM"
            }

            Text{
                id: zoomTxt
                anchors.top: parent.top
                anchors.right: parent.right

                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size

                rightPadding: parent.width * 0.05
                color: "white"
                text:  (zoomSlider.valueAt(zoomSlider.position) * 100).toFixed(0) + " %"//(DisplayCtrl.userScale * 100).toFixed(0) + "%"
            }

            Slider{
                id: zoomSlider
                anchors.top: zoomTxt.bottom
                anchors.topMargin: DisplayCtrl.gridMargin
                anchors.horizontalCenter: parent.horizontalCenter
                height: zoomTxt.contentHeight
                width: parent.width * 0.9
                rightPadding: parent.width * 0.05

                value: DisplayCtrl.userScale
                from: DisplayCtrl.minZoom
                to: DisplayCtrl.maxZoom
                onVisualPositionChanged: {
                    DisplayCtrl.targetScale = zoomSlider.valueAt(zoomSlider.position)
                }

                background: Rectangle {
                      x: zoomSlider.leftPadding
                      y: zoomSlider.topPadding + zoomSlider.availableHeight / 2 - height / 2
                      implicitWidth: zoomSlider.width
                      implicitHeight: 4
                      width: zoomSlider.availableWidth
                      height: implicitHeight
                      radius: 2
                      color: "lightgrey"

                      Rectangle {
                          width: zoomSlider.visualPosition * parent.width
                          height: parent.height
                          color: BronColors.bronBlue
                          radius: 2
                      }
                  }

                  handle: Rectangle {
                      x: zoomSlider.leftPadding + zoomSlider.visualPosition * (zoomSlider.availableWidth - width)
                      y: zoomSlider.topPadding + zoomSlider.availableHeight / 2 - height / 2
                      implicitWidth: 26
                      implicitHeight: 26
                      radius: 13
                      color: zoomSlider.pressed ? BronColors.bronBlue : BronColors.bronGray
                      border.color: BronColors.bronBlue
                      border.width: DisplayCtrl.dp(4)
                  }
            }

            Button{
                id: btTest
                anchors.top: zoomSlider.bottom
                anchors.topMargin: DisplayCtrl.gridMargin
                anchors.horizontalCenter: parent.horizontalCenter

                width: DisplayCtrl.itemSize
                height: DisplayCtrl.dp(60)
                text: "RESET"
                onClicked: {
                    isActive = false
                    DisplayCtrl.targetScale = 1.0
                }
            }
        }
        Rectangle{
            id: zoomBottomLine
            anchors.top: userZoom.bottom
            anchors.topMargin: DisplayCtrl.gridMargin
            width: parent.width
            height: DisplayCtrl.dp(1)
            color: "white"
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
