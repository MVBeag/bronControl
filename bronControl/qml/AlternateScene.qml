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
    property Item focusItem: btActive
    // constant definitions for coloring and text
    readonly property string active: "ACTIVE"
    readonly property color activeColor: BronColors.bronBlue
    readonly property string wait1: "WAIT 1"
    readonly property color wait1Color: Qt.darker(BronColors.bronBlue, 2.0)
    readonly property string wait2: "WAIT 2"
    readonly property color wait2Color: "grey"
    readonly property string wait3: "WAIT 3"
    readonly property color wait3Color: "darkgrey"
    readonly property string main: "MAIN"
    readonly property color mainColor: BronColors.bronBlue
    readonly property string mask: "MASK"
    readonly property color maskColor: "grey"
    property string toolTitle: "ALTERNATE"

    property bool resetSelection: false
    property bool validData: btActive.activeDevices > 0 && btWait1.activeDevices > 0

    background: Item{id:bg}

    function rollBack(validData){
        if(!validData){
            // signal delegates to deselect
            resetSelection = true
            // reset alternate
            alternateRow.reset()
            // reset Freemask
            setFreemaskAlternate(true)
        }
//        else{
//            setFreemaskAlternate(false)
//        }
    }

    function setFreemaskAlternate(reset){
        var settings = {}
        settings.isReset = reset
        settings.application = "alternate";
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
            Layout.preferredHeight: childrenRect.height + DisplayCtrl.gridMargin
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

                Row{
                    id: alternateRow
                    Layout.topMargin: DisplayCtrl.dp(10)
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    Layout.preferredHeight: DisplayCtrl.dp(60)
                    function reset(){
                        btWait3.isActive = false
                        btWait3.activeDevices = 0
                        btWait3.enabled = false

                        btWait2.isActive = false
                        btWait2.activeDevices = 0
                        btWait2.enabled = false

                        btWait1.activeDevices = 0
                        btWait1.enabled = false
                        btWait1.isActive = false

                        btActive.activeDevices = 0
                        alternateRow.activeBt = btActive
                    }


                    property var activeBt: btActive
                    property int selected: btActive.activeDevices + btWait1.activeDevices + btWait2.activeDevices + btWait3.activeDevices
                    Button{
                        id: btActive
                        property int activeDevices: 0
                        text: root.active
                        isActive: true
                        backgroundColor: root.activeColor
                        onClicked:{
                            if(activeDevices > 0){
                                isActive = true
                            }else if(isActive) {
                                parent.activeBt = btActive
                            }
                        }
                    }
                    Button{
                        id: btWait1
                        property int activeDevices: 0
                        text: root.wait1
                        enabled: false
                        isActive: false
                        backgroundColor: root.wait1Color
                        onClicked:{
                            if(activeDevices > 0){
                                isActive = true
                            }else if(isActive) {
                                parent.activeBt = btWait1
                            }
                        }
                        onIsActiveChanged: {
                            if(isActive === false){
                                if(activeDevices === 0){
                                    parent.activeBt = btActive
                                }
                            }
                        }
                    }
                    Button{
                        id: btWait2
                        property int activeDevices: 0
                        text: root.wait2
                        enabled: false
                        isActive: false
                        backgroundColor: root.wait2Color
                        onClicked:{
                            if(activeDevices > 0){
                                isActive = true

                            }else if(isActive) {
                                parent.activeBt = btWait2
                            }
                        }
                        onIsActiveChanged: {
                            if(isActive === false){
                                if(activeDevices === 0){
                                    parent.activeBt = btWait1
                                }
                            }
                        }
                    }
                    Button{
                        id: btWait3
                        isActive: false
                        property int activeDevices: 0
                        enabled: false
                        text: root.wait3
                        backgroundColor: root.wait3Color
                        onClicked:{
                            if(activeDevices > 0){
                                isActive = true
                            }else if(isActive) {
                                parent.activeBt = btWait3
                            }
                        }
                        onIsActiveChanged: {
                            if(isActive === false){
                                if(activeDevices === 0){
                                    parent.activeBt = btWait2
                                }
                            }
                        }
                    }
                }
            }
            Rectangle{
                id: masterBottenLine
                anchors.top: seqColumns.bottom
                anchors.topMargin: DisplayCtrl.gridMargin
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

        DelegateModel{
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
                                selectionText = alternateRow.activeBt.text
                                selectionColor = alternateRow.activeBt.backgroundColor
                                if(alternateRow.activeBt.text === root.active){
                                    btWait1.enabled = true
                                    btActive.activeDevices++
                                }
                                else if(alternateRow.activeBt.text === "WAIT 1"){
                                    btWait2.enabled = true
                                    btWait1.activeDevices++
                                }
                                else if(alternateRow.activeBt.text === "WAIT 2"){
                                    btWait3.enabled = true
                                    btWait2.activeDevices++
                                }
                                else if(alternateRow.activeBt.text === "WAIT 3"){
                                    btWait3.activeDevices++
                                }
                            }
                            else{
                                selectionText = ""
                                selectionColor = "black"

                                if(alternateRow.activeBt.text === "ACTIVE"){
                                    if(btActive.activeDevices > 0){
                                        btActive.activeDevices--
                                    }
                                    if(btActive.activeDevices === 0){
                                        btWait1.enabled = false
                                    }
                                }
                                else if(alternateRow.activeBt.text === "WAIT 1"){
                                    if(btWait1.activeDevices > 0){
                                        btWait1.activeDevices--
                                    }
                                    if(btWait1.activeDevices === 0){
                                        btWait1.isActive = false
                                        btWait2.enabled = false
                                    }
                                }
                                else if(alternateRow.activeBt.text === "WAIT 2"){
                                    if(btWait2.activeDevices > 0){
                                        btWait2.activeDevices--
                                    }
                                    if(btWait2.activeDevices === 0){
                                        btWait2.isActive = false
                                        btWait3.enabled = false
                                    }
                                }
                                else if(alternateRow.activeBt.text === "WAIT 3"){
                                    if(btWait3.activeDevices > 0){
                                        btWait3.activeDevices--
                                    }
                                    if(btWait3.activeDevices === 0){
                                        btWait3.isActive = false
                                    }
                                }
                            }
                    }
                    Component.onCompleted: {
//                        devicesGrid.loadedDevices ++;
//                        switch(model.application){
//                        case ParaSelectors.DISABLED: // eNone
//                            break;
//                        case ParaSelectors.FREEMASK: // eSequence not allowed here
//                            break
//                        case ParaSelectors.ALTERNATE_3_MASKS: // eFreemask
//                            break;
//                        case ParaSelectors.ALTERNATE_4_MASKS: // eAlternate
//                            break;
//                        }
                        switch(model.maskgroup){
                        case ParaSelectors.DISABLED:
                            selected = false
                            break
                        case ParaSelectors.ACTIVE:
                            selectionColor = btActive.backgroundColor
                            selectionText = btActive.text
                            selected = true
                            btActive.activeDevices++
                            btWait1.enabled = true
                            if(alternateRow.activeBt !== btWait3 &&
                                    alternateRow.activeBt !== btWait2 &&
                                    alternateRow.activeBt !== btWait1){
                                alternateRow.activeBt = btActive
                            }

                            break
                        case ParaSelectors.WAIT1:
                            selectionColor = btWait1.backgroundColor
                            selectionText = btWait1.text
                            btWait1.activeDevices++
                            btWait1.isActive = true
                            btWait2.enabled = true
                            selected = true
                            if(alternateRow.activeBt !== btWait3 &&
                                    alternateRow.activeBt !== btWait2){
                                alternateRow.activeBt = btWait1
                            }

                            break
                        case ParaSelectors.WAIT2:
                            selectionColor = btWait2.backgroundColor
                            selectionText = btWait2.text
                            selected = true
                            btWait2.activeDevices++
                            btWait2.isActive = true
                            btWait3.enabled = true
                            if(alternateRow.activeBt !== btWait3){
                                alternateRow.activeBt = btWait2
                            }

                            break
                        case ParaSelectors.WAIT3:
                            btWait3.activeDevices++
                            btWait3.isActive = true
                            alternateRow.activeBt = btWait3
                            selectionColor = alternateRow.activeBt.backgroundColor
                            selectionText = alternateRow.activeBt.text
                            selected = true
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
                text: "DEFINE CATEGORY"
            }
            model: repFilter

        }
    }


        header: ToolBar{
        id: toolbar
//        implicitWidth: parent.width
//        implicitHeight: DisplayCtrl.dp(70)
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
                    root.rollBack(validData)
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
                enabled: validData
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
            text: qsTr("device area. Only devices supporting this function are shown here. All other devices are disabled for this function. Select at least on device for a selected category, otherwise the following category isn't selectable")
            onClicked: help.show = false
        }
    }
}
