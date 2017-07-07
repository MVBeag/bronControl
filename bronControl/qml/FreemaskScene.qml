import QtQuick 2.7
import QtQml.Models 2.2
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import ch.bron.BronModels 1.0
import "."


Page {
    id: root
    property var studio
    property Item focusItem: freemaskBt
    readonly property string main: "MAIN"
    readonly property color mainColor: BronColors.bronBlue
    readonly property string mask: "MASK"
    readonly property color maskColor: "grey"
    property string toolTitle: "FREEMASK"

    property bool validSetting: freemaskBt.leftDevices > 0 && freemaskBt.rightDevices > 0
    property bool resetSelection: false

    background: Item{id:bg}

    function rollBack(validData){
        if(!validData){
            // signal delegates to deselect
            resetSelection = true
            // reset Freemask
            freemaskBt.reset()
            setFreemaskAlternate(true)
        }
        //        else{
        //            setFreemaskAlternate(false)
        //        }
    }

    function setFreemaskAlternate(reset){
        var settings = {}
        settings.isReset = reset
        settings.application = "freemask";
        settings.deviceValues = []
        var i
        var length = repFilter.items.count
        devicesGrid.highlightFollowsCurrentItem = false
        for(i = 0; i < length; i++){
            var entry = repFilter.items.get(i)
            if(entry.inShow){
                var entryIndex = entry.showIndex
                devicesGrid.currentIndex = entryIndex
                var group = devicesGrid.currentItem.selectionText
                settings.deviceValues.push({ "index": i,"group": group})
            }
        }
        devicesGrid.highlightFollowsCurrentItem = true
        studio.application = settings
    }

    ColumnLayout{
        id: wrapper
        anchors.fill: parent

        Rectangle{
            id: seqControl
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: childrenRect.height + DisplayCtrl.gridMargin//seqColumns.height//  parent.height/3.5
            color: "black"
            ColumnLayout{
                id: seqColumns
                width: parent.width * 0.9
                anchors.centerIn: parent

                Text{
                    id: categoriesText
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    font.family: bronFontBold.name
                    font.pointSize: DisplayCtrl.point20Size
                    color: "white"
                    text: "CATEGORIES"
                }

                Button2State{
                    id: freemaskBt
                    Layout.topMargin: DisplayCtrl.gridMargin
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: parent.width / 2
                    Layout.preferredHeight: DisplayCtrl.dp(60)

                    function reset(){
                        isLeftActive = true
                        leftDevices = 0
                        rightDevices = 0
                    }

                    rightColor: root.maskColor
                    textleft: root.main
                    textright: root.mask

                    property int leftDevices: 0
                    property int rightDevices: 0
                    property string activeText: isLeftActive ? textleft : textright
                    property color activeColor: isLeftActive ? leftColor : rightColor
                    property int selected: leftDevices + rightDevices
                }

            }
            Rectangle{
                id: masterBottenLine
                anchors.topMargin: DisplayCtrl.gridMargin
                anchors.top: seqColumns.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                height: DisplayCtrl.dp(4)
                color: "white"
            }
        }


        Repeater{
            model: root.studio.devices
            Item{
                property var count: model.count
                onCountChanged: {
                    var rowCount = model.count
                    for( var i = 0;i < rowCount;i++ ) {
                        var entry = repFilter.items.get(i)
                        var name = entry.name
                        var hide = false
                        if(name === "Scoro 1600 E" || name === "Scoro 3200 E"){
                            hide = true
                        }
                        if(((entry.type !== 2) && (hide === false)) ) {
                            items.get(i).inShow = true
                        }
                    }
                }
            }
        }




        GridView{
            id: devicesGrid
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: DisplayCtrl.dp(10)
            Layout.preferredWidth: Math.ceil(background.width * 0.9 + DisplayCtrl.gridMargin)
            Layout.fillHeight: true
            clip: true
            interactive: true
            focus: true
            cellHeight: (DisplayCtrl.itemSize + DisplayCtrl.gridMargin) * DisplayCtrl.userScale
            cellWidth: (DisplayCtrl.itemSize + DisplayCtrl.gridMargin) * DisplayCtrl.userScale

            header: Text{
                id: defineMaster
                font.family: bronFontBold.name
                font.pixelSize: DisplayCtrl.dp(50)
                color: "white"
                text: "DEFINE MASTER"
            }
            model:         DelegateModel{
                id: repFilter

                groups:[
                    DelegateModelGroup{
                        includeByDefault: false
                        name: "show"
                    }
                ]

                filterOnGroup: "show"

                Component.onCompleted: {
                    var rowCount = model.count
                    for( var i = 0;i < rowCount;i++ ) {
                        var entry = model.get(i)
                        var name = entry.name
                        var hide = false
                        if(name === "Scoro 1600 E" || name === "Scoro 3200 E"){
                            hide = true
                        }
                        if(((entry.type !== 2) && (hide === false)) ) {
                            items.get(i).inShow = true
                        }
                    }
                }

                model: root.studio.devices

                delegate: Item{
                    id: dlgItem
                    width: devicesGrid.cellWidth
                    height: devicesGrid.cellHeight
                    transform: DisplayCtrl.userZoom

                    property alias cogniCol: aDevice.cogniColor
                    property var address: aDevice.device.address
                    // switch back all selections
                    property bool resetSelection: root.resetSelection
                    property alias selectionText: aDevice.selectionText
                    onResetSelectionChanged: {
                        if(resetSelection === true){
                            aDevice.selected = false
                        }
                    }

                    Lamp{
                        id: aDevice
                        function setSelect(){
                            if(aDevice.selected){
                                switchBt(false)
                                aDevice.selected = false
                            }
                            else{
                                switchBt(true)
                                aDevice.selected = true
                            }
                        }
                        function switchBt(active) {
                            if(active){
                                if(freemaskBt.isLeftActive){
                                    freemaskBt.leftDevices++
                                    selectionText = freemaskBt.textleft
                                    selectionColor = freemaskBt.leftColor
                                }
                                else{
                                    freemaskBt.rightDevices++
                                    selectionText = freemaskBt.textright
                                    selectionColor = freemaskBt.rightColor
                                }
                            }
                            else{
                                selectionText = ""
                                selectionColor = "black"
                                if(freemaskBt.isLeftActive){
                                    if(freemaskBt.leftDevices > 0){
                                        freemaskBt.leftDevices--
                                    }
                                }
                                else{
                                    if(freemaskBt.rightDevices > 0){
                                        freemaskBt.rightDevices--
                                    }
                                }
                            }
                        }
                        Component.onCompleted: {
                            switch(model.maskgroup){
                            case ParaSelectors.DISABLED:
                                selected = false
                                break
                            case ParaSelectors.ACTIVE:
                                freemaskBt.isLeftActive = true
                                freemaskBt.leftDevices++
                                selectionColor = freemaskBt.leftColor
                                selectionText = freemaskBt.textleft
                                selected = true
                                break
                            case ParaSelectors.WAIT1:
                                freemaskBt.rightDevices++
                                freemaskBt.isLeftActive = false
                                selectionColor = freemaskBt.rightColor
                                selectionText = freemaskBt.textright
                                selected = true
                                break
                            case ParaSelectors.WAIT2:
                            case ParaSelectors.WAIT3:
                                break
                            }
                        }

                        device: model
                        selectionMode: true
                        //                    anchors.centerIn: parent
                        cogniColor: BronColors.intToColer(model.cogniColor)


                        onMoreClicked: setSelect()
                        onLampClicked: setSelect()
                    }
                }
            }
        }
    }


    header: ToolBar{
        id: toolbar
        background: Rectangle {
            anchors.fill: parent
            color: BronColors.bronBlack
        }
        RowLayout {
            anchors.fill: parent
            spacing: DisplayCtrl.gridMargin
            BronToolButton{
                id: btback

                Layout.margins: DisplayCtrl.dp(10)
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                font.family:  googleMaterial.name
                text: DisplayCtrl.icBack

                onClicked: {
                    root.rollBack(validSetting)
                    stack.pop()
                }
            }
            Text{
                id: title
                font.family: bronFontBold.name
                font.pointSize: DisplayCtrl.point30Size
                font.bold: true
                Layout.fillWidth: true
                Layout.fillHeight: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
                minimumPointSize: Math.max(DisplayCtrl.point5Size,1)
                color: "white"
                text: root.toolTitle
            }

            BronToolButton{
                id: btOk
                font.family:  googleMaterial.name
                text: DisplayCtrl.icDone
                enabled: (freemaskBt.leftDevices > 0) && (freemaskBt.rightDevices > 0)
                visible: enabled
                unselectColor: "lightgreen"
                onClicked: {
                    root.setFreemaskAlternate(false);
                    stack.pop();
                }

            }
            BronToolButton{
                id: btReset
                font.family:  googleMaterial.name
                text: DisplayCtrl.icReset
                onClicked: {
                    root.rollBack(false)
                }

            }


            BronToolButton{
                id: btHelp
                Layout.rightMargin: DisplayCtrl.dp(10)
                font.family: googleMaterial.name
                text: DisplayCtrl.icHelp
                onClicked: help.show = !help.show
            }
        }
    }


    footer: Rectangle{
        width: parent.width
        height: parent.height/30//Window.height/30
        color: "black"

        Image{
            id: conImage
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: DisplayCtrl.dp(20)
            height: parent.height*0.9
            fillMode: Image.PreserveAspectFit
            source: root.studio.name === "Bron-Demo0" ? "qrc:///res/reconnect_icon.png" :
                                                        root.studio.online ? "qrc:///res/reconnect_icon.png" : "qrc:///res/reconnect_failed_icon.png"
            Text{
                id: connectText
                anchors.left: parent.right
                anchors.leftMargin: DisplayCtrl.dp(15)
                font.family: bronFontBold.name
                color: "white"
                //                    font.pixelSize: parent.height
                text: root.studio.name
            }
        }

        Image{
            id: bronLogo
            anchors.centerIn: parent

            visible: x <= (connectText.x + connectText.contentWidth + 10) ? false : true
            height: parent.height * 0.6
            fillMode: Image.PreserveAspectFit
            source:"qrc:///res/bron_logo.png"
            mipmap: true
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
            id: helpSeqControl
            hidden: seqControl
            text: qsTr("select first the category, and than device(s) belonging to this category")
            onClicked: help.show = false
        }
        HelpText{
            id: helpDevicesGrid
            hidden: devicesGrid
            text: qsTr("device area. Only devices supporting this function are shown here. All other devices are disabled for this function. Select at least on device for main and mask")
            onClicked: help.show = false
        }
    }


}
