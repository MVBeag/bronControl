import QtQuick 2.7
import QtQuick.Controls 2.0
import "."

Item {
    id: root

    signal back()

    property string address
    property var studio

    Item{
        id: enterprizeSettings
        function clearSettings(){
            ssid = ""
            pw = ""
            encr = 1
        }

        property string ssid: ""
        property string pw: ""
        property int encr: 1 // WPA is active on load
        property bool isValid: ssid !== "" //&& pw !== "" allowed in case of open network
    }

    Item{
        id: sapSettings
        property string ssid: "Bron-Studio" + root.address
        property string pw: "bronControl"
        property int encr: 2 // WPA2
        property bool isValid: ssid !== ""
    }


    Rectangle{
        id: visualSide

        width: parent.width * 2 / 3
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left

        color: BronColors.bronBlack

        property bool sapActive: true



        function clicked(){
            if(sapActive){
                sapActive = false
                if(DisplayCtrl.isMobilePlattform){
                    nwLoader.setSource("qrc:///qml/SettingsNetworkIOSAndroid.qml")
                }
                else{
                    nwLoader.setSource("qrc:///qml/SettingsNetwork.qml")
                }
            }
            else{
                sapActive = true
                root.state = ""
                enterprizeSettings.clearSettings()
            }

        }

        Rectangle{
            id: settingsFrame
            anchors.top: parent.top
            width: parent.width
            height: DisplayCtrl.dp(70)
            color: BronColors.bronBlue
            BronToolButton{
                anchors.right: parent.right
                anchors.top: parent.top

                font.family: googleMaterial.name
                font.pointSize: DisplayCtrl.isMobilePlattform ? DisplayCtrl.point30Size :  DisplayCtrl.point20Size
                text: DisplayCtrl.icDone
                enabled: visualSide.sapActive ? true : enterprizeSettings.isValid
                visible: enabled
                onClicked: {
                    if(visualSide.sapActive){
                        studioMgr.setNetworkMode(studio.name, root.address, false, sapSettings.ssid, sapSettings.pw, sapSettings.encr)
                    }
                    else{
                        studioMgr.setNetworkMode(studio.name, root.address, true, enterprizeSettings.ssid, enterprizeSettings.pw, enterprizeSettings.encr)

                    }
                    root.back()
                }

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
            id: privateMode

            anchors.top: settingsFrameEnd.top
            anchors.topMargin: DisplayCtrl.dp(10)
            anchors.left: parent.left

            width: parent.width
            height: privateModeTxt.contentHeight + privateModeHelpText.contentHeight

            property alias txtColor: privateModeTxt.color

            color: visualSide.sapActive ? BronColors.bronBlue : BronColors.bronBlack
            Text{
                id: privateModeTxt
                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size
                color: "white"
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                fontSizeMode: Text.HorizontalFit
                maximumLineCount: 1

                text: "PRIVATE MODE"
            }
            Text{
                id: privateModeHelpText
                anchors.top: privateModeTxt.bottom
                anchors.left: privateModeTxt.left

                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point15Size
                width: parent.width * 0.9
                color: "white"
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                maximumLineCount: 3
                wrapMode: Text.WordWrap
                text: "This automatically connects the devices with the same studio address. The devices are creating a own \"private\" network"
            }

            MouseArea{
                id: maSAP

                anchors.fill: parent
                onClicked: {
                    if(visualSide.sapActive === false){
                        visualSide.clicked()
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
            id: enterpriseMode

            anchors.top: privateMode.bottom
            anchors.left: parent.left

            width: parent.width
            height: enterpriseModeTxt.contentHeight + enterpriseModeHelpText.contentHeight

            property alias txtColor: enterpriseModeTxt.color

            color: visualSide.sapActive ? BronColors.bronBlack : BronColors.bronBlue
            Text{
                id: enterpriseModeTxt
                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point20Size
                color: "white"
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                fontSizeMode: Text.HorizontalFit
                maximumLineCount: 1

                text: "ENTERPRISE MODE"
            }
            Text{
                id: enterpriseModeHelpText
                anchors.top: enterpriseModeTxt.bottom
                anchors.left: enterpriseModeTxt.left

                font.family: bronFontMedium.name
                font.pointSize: DisplayCtrl.point15Size
                width: parent.width * 0.9
                color: "white"
                leftPadding: parent.width * 0.05
                elide: Text.ElideRight
                maximumLineCount: 3
                wrapMode: Text.WordWrap
                text: "Choose this mode to assign this studio to an existing Network"
            }

            MouseArea{
                id: maEnterprize

                anchors.fill: parent
                onClicked: {
                    if(visualSide.sapActive === true){
                        visualSide.clicked()
                    }
                }
            }


            Rectangle{
                width: parent.width
                height: DisplayCtrl.dp(1)
                anchors.top: parent.bottom
                color: "white"
            }
        }

        Loader{
            id: nwLoader
            width: parent.width
            height: parent.height
            anchors.top: enterpriseMode.bottom
            anchors.topMargin: DisplayCtrl.dp(20)
            x: -width
            focus: true
            onLoaded: {
                root.state = "shown"
            }
            function clearLoader(){
                nwLoader.source = ""
            }

            Connections{
                target: nwLoader.item
                onSsid:{
                    enterprizeSettings.ssid = ssid
                }
                onEncryption:{
                    enterprizeSettings.encr = SirosSelects.findValFromTag(SirosSelects.wlanEncMode,  encr)
                }
                onPw:{
                    enterprizeSettings.pw = pw
                }
                onHide:{
                    root.state = "";
                    visualSide.sapActive = true
                }
                onAccept:{
                    root.state = ""
                    if(enterprizeSettings.isValid){
                        studioMgr.setNetworkMode(studio.name, root.address, true, enterprizeSettings.ssid, enterprizeSettings.pw, enterprizeSettings.encr)
                        root.back()
                    }
                }
            }
        }
    }

    Rectangle{
        width: parent.width - visualSide.width
        height: parent.height
        anchors.top: parent.top
        anchors.left: visualSide.right
        color: "transparent"
        MouseArea{
            anchors.fill: parent
            onPressed: back()
        }
    }

    states:[
        State{
            name: "shown"
            PropertyChanges {
                target: nwLoader
                x: 0
            }
        }
    ]
    transitions: [
        Transition{
            from: ""; to: "shown"
            NumberAnimation { target: nwLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
        },
        Transition{
            from: "shown"; to: ""
            SequentialAnimation{
                NumberAnimation { target: nwLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
                ScriptAction{ script: nwLoader.clearLoader()}
            }

        }
    ]



}
