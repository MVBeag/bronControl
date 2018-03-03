import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
//import QtQuick.Controls.Styles 1.4
import ch.bron.BronModels 1.0
import "."

Page {
    id: root

    property var studioMgr
    property bool wiggle: false
    property Item focusItem: studioGrid
    property string toolTitle: "CHOOSE STUDIO"

    focus: true

    signal switchStudioMgrView(var frontside)


    function changeStudio(ix){
        // close all stackview above the current
        var depth = stack.depth
        if(depth > 1){
            depth--;
        }
        for(var i = 0; i < depth; i++){
            var item = stack.currentItem
            if(item.objectName === "studioScene"){
                item.studio.online = false
            }
            stack.pop()
        }
        // start new studio
        if(ix < studioGrid.count){
            studioGrid.currentIndex = ix
            studioGrid.currentItem.delitem.switchWiggle()
        }
    }

    background: Item{id:bg}

    Component.onCompleted: {
        studioGrid.forceActiveFocus()
    }

    Connections{
        target: studioMgr
        onSwitchToStudio: changeStudio(ix)
    }

    PinchArea{
        id: zoom
        anchors.fill: parent

        pinch.minimumScale: DisplayCtrl.minZoom
        pinch.maximumScale: DisplayCtrl.maxZoom
        property real scale: 1.0
        onPinchStarted: {
            scale = DisplayCtrl.userScale
        }

        onPinchUpdated: {
            // adjust content pos due to drag
            var _scale = pinch.scale * scale
            if(_scale <= zoom.pinch.maximumScale && _scale >= zoom.pinch.minimumScale){
                DisplayCtrl.targetScale = _scale
            }
        }

        onPinchFinished: {
            var _scale = pinch.scale * scale
            if(_scale <= zoom.pinch.maximumScale && _scale >= zoom.pinch.minimumScale){
                DisplayCtrl.targetScale = _scale
            }
            else{
                if(pinch.scale < 1.0){
                    DisplayCtrl.targetScale = zoom.pinch.minimumScale
                }
                else{
                    DisplayCtrl.targetScale = zoom.pinch.maximumScale
                }
            }
        }



        ColumnLayout{
            id: cLayout
            anchors.fill: parent
            spacing: 0

            GridView{
                id: studioGrid
                Layout.topMargin: DisplayCtrl.gridMargin
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: parent.width * 0.05
                Layout.preferredWidth: parent.width * 0.9 + DisplayCtrl.gridMargin * 2
                Layout.preferredHeight:  parent.height*3/4

                KeyNavigation.tab:  studioGrid.count < 2 ? btHelp : btRemove
                KeyNavigation.backtab: btsettings

                cellHeight: (DisplayCtrl.itemSize + DisplayCtrl.gridMargin) * DisplayCtrl.userScale
                cellWidth: cellHeight
                clip: true
                model: root.studioMgr.studios
                snapMode: GridView.SnapToRow
                focus: true
                interactive: (contentHeight > height) || (contentWidth > width) ? true : false
                delegate: listDelegate
            }

            Component{
                id: listDelegate

                Item{
                    id: delitem

                    function switchWiggle(){
                        if(root.wiggle === false){
                            if(aStudio.netWorkState === StudioProxy.Active ||
                                    aStudio.netWorkState === StudioProxy.KnownAccessable ||
                                    aStudio.studioName === "Bron-Demo0"){
                                studioGrid.currentIndex = index
                                model.online = true
                                root.focusItem = studioGrid
                                stack.push(Qt.resolvedUrl("LoadingScene.qml"),
                                           {"studio" : model})
                            }
                            else{
                                networkProblem.visible = true
                            }
                        }
                    }

                    MessageDialog{
                        id: networkProblem
                        title: "Network problem!"
                        text: "The network of the selected studio is not accessible!"
                        icon: StandardIcon.Warning
                    }

                    width: studioGrid.cellWidth
                    height: studioGrid.cellHeight
                    transform: DisplayCtrl.userZoom
                    enabled: model.name === "Bron-Demo0" ? true : model.devices.count > 0 ? true : aStudio.wiggle
//                    enabled: model.ame === "Bron-Demo0" ? true : model.networkState === StudioProxy.Active ? true : aStudio.wiggle
                    opacity: aStudio.number === -1 ? 0.7 : enabled ? 1.0 : 0.7
                    focus: true

                    Studio{
                        id: aStudio
                        anchors.top: parent.top
                        anchors.left: parent.left
                        studioName: model.name
                        number: model.address
                        wiggle: root.wiggle
                        netWorkState: model.networkState
                        hasFocus: parent.activeFocus
                    }

                    Keys.onReturnPressed:{
                        if(root.state === ""){
                            event.accepted = true;
                            studioGrid.currentIndex = index
                            delitem.switchWiggle()
                        }
                    }

                    MouseArea{
                        id: mabronStudioserea
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        anchors.fill: aStudio
                        onClicked:{
                            if(root.state === ""){
                                delitem.forceActiveFocus()
                                studioGrid.currentIndex = index
                                if(mouse.button  === Qt.RightButton){
                                    model.wink = 1
                                }
                                if(mouse.button === Qt.LeftButton){
                                    delitem.switchWiggle()
                                }
                            }
                        }
                        onPressAndHold: model.wink = 1
                    }
                    Text{
                        id: textDelete

                        anchors.fill: aStudio
                        z: 0

                        enabled: aStudio.studioName === "Bron-Demo0" ? false : aStudio.wiggle
                        visible: aStudio.studioName === "Bron-Demo0" ? false : aStudio.wiggle

                        font.family: googleMaterial.name
                        font.pixelSize: parent.width * 0.7
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        minimumPixelSize: DisplayCtrl.dp(4)
                        fontSizeMode: Text.Fit

                        color: "white"
                        opacity: 0.5
                        text: DisplayCtrl.icDelete

                        MouseArea{
                            id: delStudio
                            anchors.fill: parent
                            onClicked: {
                                if(aStudio.studioName !== "Bron-Demo0"){
                                    //studioGrid.model.removeAt(index)
                                    studioMgr.deleteStudio(index);
                                }
                                if(studioGrid.count < 2){
                                    root.wiggle = false
                                    studioGrid.forceActiveFocus()
                                }
                            }
                        }
                    }
                }
            }

            Text{
                id: autoConnectText
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: root.width * 0.8
                visible: true//!root.studioMgr.autoConnect
                enabled: visible
                font.family: bronFontBold.name
                font.pixelSize: DisplayCtrl.dp(40)
                wrapMode: Text.Wrap
                verticalAlignment: Text.AlignHCenter
                text: "Please change the active network in the Operating System Network Settings"
                color: "white"
            }
            Item{
                Layout.fillHeight: true
            }

        }
    }

    Rectangle{
        id: help
        anchors.fill: parent
        property bool show: false
        property string oldTitle: ""
        onShowChanged: {
            if(show){
                oldTitle = root.toolTitle
                root.toolTitle = "HELP"
            }
            else{
                root.toolTitle = oldTitle
                oldTitle = ""
            }
        }


        color: Qt.rgba(0,0,0,0.7) // semiTransparent
        enabled: opacity > 0 ? true : false
        visible: true

        opacity: show ? 1 : 0

        Behavior on opacity {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }



        MouseArea{
            anchors.fill: parent
            onClicked: help.show = false
        }

        HelpText {
            id: helpStudioGrid
            hidden: studioGrid
            horizontalAlignment: Text.AlignLeft
            text: qsTr("1. The Flash units must be first set to a studio number\n2. WiFi must be turned on such that the Blue lights finishes flashing and is ON\n3. Select WiFi network on the PC to connect to bronControl, with correct studio number")
            onClicked: help.show = false
        }
    }

    header: ToolBar{
        id: toolbar
        background: Rectangle {
            anchors.fill: parent
            color: BronColors.bronBlack
        }
        RowLayout{
            anchors.fill: parent
            Item{
                Layout.fillWidth: true
            }
            Text{
                font.family: bronFontBold.name
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
                font.pointSize: DisplayCtrl.point20Size
                text: root.toolTitle;
                color: "white"
            }
            Item{
                Layout.fillWidth: true
            }

            BronToolButton{
                id: btRemove

                KeyNavigation.tab: btHelp
                KeyNavigation.backtab: studioGrid

                focus: true
                font.family: googleMaterial.name
                text: DisplayCtrl.icDelete

                enabled: studioGrid.count > 1
                opacity: studioGrid.count > 1 ? 1.0 : 0.5

                onClicked: {
                    focusItem = btRemove
                    root.wiggle = !root.wiggle
                }

                Keys.onReturnPressed:{
                    event.accepted = true;
                    btRemove.onClicked()
                }
            }
            BronToolButton{
                id: btHelp
                font.family: googleMaterial.name
                text: DisplayCtrl.icHelp
                focus: true
                KeyNavigation.tab: btsettings
                KeyNavigation.backtab: studioGrid.count < 2 ? studioGrid : btRemove
                Keys.onReturnPressed: {
                    event.accept = true
                    focusItem = btHelp
                    help.show = !help.show
                }

                onClicked:{
                    focusItem = btHelp
                    help.show = !help.show
                }
            }
            BronToolButton{
                id: btsettings
                Layout.rightMargin: DisplayCtrl.dp(10)

                font.family: googleMaterial.name
                text: DisplayCtrl.icSettings
                focus: true

                KeyNavigation.tab: studioGrid
                KeyNavigation.backtab: btRemove

                Keys.onReturnPressed: {
                    event.accepted = true
                    focusItem = studioGrid
                    settingsLoader.setSource("qrc:///qml/SettingsStudios.qml")
                }

                onClicked:{
                    focusItem = studioGrid
                    settingsLoader.setSource("qrc:///qml/SettingsStudios.qml")
                }
            }
        }
    }
    footer: Rectangle{
        width: parent.width
        height: toolbar.height*2/3
        color: "black"

        Image{
            id: bronLogo
            anchors.centerIn: parent
            height: parent.height * 0.6
            fillMode: Image.PreserveAspectFit
            source:"qrc:///res/bron_logo.png"
            mipmap: true
        }
    }

    Loader{
        id: settingsLoader
        width: parent.width
        height: parent.height
        x: -width
        function clearLoader(){
            settingsLoader.source = ""
            focusItem.forceActiveFocus()
        }
        onLoaded: root.state = "settings"
        focus: true
    }

    Connections{
        target: settingsLoader.item
        onBack:{
            root.state = ""
            focusItem = studioGrid
        }
        onSwitchStudioMgrView:{
            root.switchStudioMgrView(frontside)
        }
    }

    states:[
        State{
            name: "settings"
            PropertyChanges {
                target: settingsLoader
                x: 0
            }
        }

    ]

    transitions: [
        Transition {
            from: ""
            to: "settings"
            NumberAnimation{ target: settingsLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
        },
        Transition {
            from: "settings"
            to: ""
            SequentialAnimation{
                NumberAnimation{ target: settingsLoader; property: "x"; duration: 400; easing.type: Easing.InOutQuad }
                ScriptAction{ script: settingsLoader.clearLoader()}
            }
        }
    ]


}
