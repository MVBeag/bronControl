import QtQuick 2.7
import QtQml.Models 2.2
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import TextFieldDoubleValidator 1.0
import "."


Page {
    id: root
    property var studio
    property Item focusItem: sequFrame
    property string toolTitle: "SEQUENCE"

    // bei sequence ohne master pause und master, keine veränderungen an remote settings (cell, rfs/ir)
    // in prinzip die sequence aus more
    property bool correctSetup: (sequFrame.value > 0 && masterPause.value === 0 && masterDelay.value === 0 && selectedMaster === -1) ||
                                (sequFrame.value > 0 && (masterPause.value > 0 || masterDelay.value > 0) && selectedMaster !== -1)

    background: Item{id:bg}


    function rollBack(reset){
        if(reset){
            sequFrame.reset()
            masterPause.reset()
            masterDelay.reset()
            resetSelection = true
            selectedMaster = -1
            setSequence(true)
        }
    }

    // parse the settings of the dlgItems and build an command for the underlying driver framework
    function setSequence(reset){
        var settings = {}
        settings.isReset = reset
        settings.application = "sequence"
        settings.deviceValues = []
        var length = repFilter.items.count
        for(var i = 0; i < length; i++){
            var entry = repFilter.items.get(i)
            if(entry.inShow){
//                var entryIndex = entry.showIndex
                // since we loop over items, we have to use i and not entryIndex
                var isMaster = root.selectedMaster === i ? true : false
                var delay = masterDelay.value
                var interval = masterPause.value
                var sequence = sequFrame.value
                settings.deviceValues.push({ "index": i,"isMaster":isMaster, "delay":delay, "interval": interval, "sequence": sequence})
            }
        }
        studio.application = settings
    }

    property int selectedMaster: -1
    property bool resetSelection: false

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
                RowLayout{
                    Layout.preferredHeight: sequenceValue.height
                    spacing: 0
                    Text{
                        id: sequenceText
//                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: sequFrame.isActive ? "SEQUENCE: " /*+ (sequFrame.visualPosition * sequFrame.higherLimit).toFixed(0)*/
                                                 : "SEQUENCE"
                    }
                    TextField{
                        id: sequenceValue
                        background: Rectangle{
                            color: BronColors.bronBlack
                        }
                        visible: sequFrame.value > 0
                        enabled: visible
                        validator: IntValidator{bottom: 0; top: 50}
                        inputMethodHints: Qt.ImhDigitsOnly
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: (sequFrame.visualPosition * sequFrame.higherLimit).toFixed(0)
                        onEditingFinished: sequFrame.value = text
                    }
                }
                BronSlider {
                    id: sequFrame
                    Layout.topMargin: DisplayCtrl.gridMargin
                    Layout.preferredHeight: DisplayCtrl.dp(150)
                    Layout.fillWidth: true
                    lowerLimit: 0.0
                    higherLimit: 50.0
                    stepSize: 1.0
                    comma: 0
                }


                RowLayout{
                    Layout.preferredHeight: delayValue.height
                    spacing: 0
                    Text{
                        id: masterDelayText
//                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: masterDelay.isActive ? "MASTER DELAY: "
                                                   : "MASTER DELAY"
                    }
                    TextField{
                        id: delayValue
                        background: Rectangle{
                            color: BronColors.bronBlack
                        }
                        visible: masterDelay.value > 0
                        enabled: visible
                        validator: TextFieldDoubleValidator{bottom: 0; decimals: 2; top: 10; notation: DoubleValidator.StandardNotation}
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: (masterDelay.visualPosition * masterDelay.higherLimit).toFixed(2)
                        onEditingFinished: masterDelay.value = text
                    }
                    Text{
                        id: masterDelayUnit
                        visible: masterDelay.value > 0
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: "s"
                    }
                    Item{
                        Layout.fillWidth: true
                    }
                }
                BronSlider{
                    id: masterDelay
                    Layout.topMargin: DisplayCtrl.gridMargin
                    Layout.preferredHeight: DisplayCtrl.dp(150)
                    Layout.fillWidth: true
                    lowerLimit: 0.0
                    higherLimit: 10.0
                    stepSize: 0.05
                    comma: 0
                    unit: "s"
                }

//                Text{
//                    id: masterPauseText
//                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
//                    font.family: bronFontBold.name
//                    font.pointSize: DisplayCtrl.point20Size
//                    color: "white"
//                    text: masterPause.isActive ? "MASTER INTERVAL - " + (masterPause.visualPosition * masterPause.higherLimit).toFixed(2) + "s"
//                                               : "MASTER INTERVAL"
//                }

                RowLayout{
                    Layout.preferredHeight: delayValue.height
                    spacing: 0
                    Text{
                        id: masterPauseText
//                        Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: masterPause.isActive ? "MASTER INTERVAL: "
                                                   : "MASTER INTERVAL"
                    }
                    TextField{
                        id: pauseValue
                        background: Rectangle{
                            color: BronColors.bronBlack
                        }
                        visible: masterPause.value > 0
                        enabled: visible
                        validator: TextFieldDoubleValidator{bottom: 0; decimals: 2; top: 10; notation: DoubleValidator.StandardNotation}
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: (masterPause.visualPosition * masterPause.higherLimit).toFixed(2)
                        onEditingFinished: masterPause.value = text
                    }
                    Text{
                        id: masterPauseUnit
                        visible: masterPause.value > 0
                        font.family: bronFontBold.name
                        font.pointSize: DisplayCtrl.point20Size
                        color: "white"
                        text: "s"
                    }
                    Item{
                        Layout.fillWidth: true
                    }

                }
                BronSlider{
                    id: masterPause
                    Layout.topMargin: DisplayCtrl.gridMargin
                    Layout.preferredHeight: DisplayCtrl.dp(150)
                    Layout.fillWidth: true
                    lowerLimit: 0.0
                    higherLimit: 10.0
                    stepSize: 0.05
                    comma: 0
                    unit: "s"
                }
            }
            Rectangle{
                id: masterBottenLine
                anchors.top: seqColumns.bottom
                anchors.topMargin: DisplayCtrl.gridMargin
                width: parent.width
                height: DisplayCtrl.dp(4)
                color: "white"//BronColors.disabledGrey
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
                property int selectedMaster: root.selectedMaster
                property bool resetSelection: root.resetSelection

                Component.onCompleted: {
                    if(root.selectedMaster === -1){
                        if(model.sequence > 1){
                            root.selectedMaster = index
                            aDevice.selected = true
                            sequFrame.isActive = true
                            sequFrame.value = model.sequence
                            if(model.pauseTime !== 0){
                                masterPause.isActive = true
                                masterPause.value = model.pauseTime
                            }
                            if(model.delay != 0){
                                masterDelay.isActive = true
                                masterDelay.value = model.delay
                            }
                        }
                    }
                }

                onResetSelectionChanged: {
                    if(resetSelection === true){
                        root.selectedMaster = -1
                        aDevice.selected = false
                    }
                }

                onSelectedMasterChanged: {
                    if(selectedMaster === -1){
                        aDevice.selected = false
                    }
                    else if(selectedMaster !== index){
                        aDevice.selected = false
                    }
                }

                Lamp{
                    id: aDevice
                    device: model
                    selectionMode: true
//                    anchors.centerIn: parent
                    selectionColor: BronColors.bronBlue
                    selectionText: "MASTER"
                    cogniColor: BronColors.intToColer(model.cogniColor)
                    onMoreClicked: {
                        if(aDevice.selected){
                            root.selectedMaster = -1
                            aDevice.selected = false
                        }
                        else{
                            root.selectedMaster = index
                            aDevice.selected = true
                        }
                    }
                    onLampClicked: {
                        if(aDevice.selected){
                            root.selectedMaster = -1
                            aDevice.selected = false
//                            btOk.enabled = false
                        }
                        else{
                            root.selectedMaster = index
                            aDevice.selected = true
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
            model: repFilter

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
                    root.rollBack(!correctSetup)
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
                enabled: correctSetup
                visible: enabled
                text: DisplayCtrl.icDone
                unselectColor: "lightgreen"
                onClicked: {
                    root.setSequence(false);
                    stack.pop()
                }

            }

            BronToolButton{
                id: btReset
                font.family:  googleMaterial.name
                text: DisplayCtrl.icReset
                onClicked: {
                    root.rollBack(true)
                }

            }

            BronToolButton{
                id: btHelp
                Layout.rightMargin: DisplayCtrl.dp(10)
                font.family: googleMaterial.name
                text: DisplayCtrl.icHelp
                onClicked:{
                    root.focusItem = btHelp
                    help.show = !help.show
                }
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
            id: helpSeqFrame
            hidden: sequFrame
            text: qsTr("set the number of flashes this sequence has")
            onClicked: help.show = false
        }
        HelpText{
            id: helpMasterDelay
            hidden: masterDelay
            text: qsTr("set the delay time (trigger to first flash)")
            onClicked: help.show = false
        }

        HelpText{
            id: helpMasterPause
            hidden: masterPause
            text: qsTr("set the interval time (time between flashes)")
            onClicked: help.show = false
        }

        HelpText{
            id: helpDevicesGrid
            hidden: devicesGrid
            text: qsTr("device area. Only devices supporting this function are shown here. All other devices are disabled for this function. Select a master to drive the sequence.")
            onClicked: help.show = false
        }
    }
}
