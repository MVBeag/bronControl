import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
//import QtQuick.Controls.Styles 1.4
import QtQml 2.2
import QtQml.Models 2.2
import ch.bron.BronModels 1.0
import "."

Page {
    id: root

    property var studio
    property Item focusItem: mainEnergyEnergy
    property int apertureMode: Selectables.fStopRange
    property string toolTitle: ""//studio.name

    focus: true
    title: studio.name

    objectName: "studioScene"

    property bool onLineState: root.studio.online
    onOnLineStateChanged:{
        if(!onLineState) {
            stack.pop()
        }
    }


    property real viewHeight: Qt.platform.os === "osx" ?  height - header.height - footer.height :
                              Qt.platform.os === "windows" ? height - header.height - footer.height : height - header.height - footer.height

    onHeightChanged: {
        if(popUpLoader.status === Loader.Ready){
            popUpLoader.item.height = viewHeight
        }
    }

    property bool showGroupsRight: width - DisplayCtrl.portraitWidth > groupsGrid.cellWidth + DisplayCtrl.portraitWidth * 0.1

    Component.onCompleted: {
        focusItem.forceActiveFocus()
    }

    background: Rectangle{
        id: background
        color: BronColors.bronBlack
    }

    contentData: ColumnLayout{
        id: wrapper
        anchors.fill: parent
        spacing: 0
        GridLayout{
            id: gridLayout

            flow: root.showGroupsRight ? GridLayout.LeftToRight : GridLayout.TopToBottom
            columnSpacing: 0
            rowSpacing: 0

            Rectangle{
                id: studiosCtrl
                Layout.preferredWidth: DisplayCtrl.portraitWidth
                Layout.preferredHeight: DisplayCtrl.portraitHeight/3
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                color: "black"

                EnergySetter{
                    id: mainEnergyEnergy

                    color: studiosCtrl.color
                    focus: true

                    onSetFocus: btLine.forceActiveFocus()
                    onSetBackFocus: btLine.forceActiveFocus()

                    width: DisplayCtrl.energySetterWidth
                    height: parent.height /*- pageInd.height*/ - btLine.height - (2 * DisplayCtrl.gridMargin)
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    property var count: studio.devices.count

                    min: 0.0
                    max: 10.0
                    currentMin: root.studio.maxMinPossibleEnergy
                    currentMax: root.studio.minMaxPossibleEnergy
                    currentRangeMax: root.studio.maxPower
                    currentRangeMin: root.studio.minPower

                    onIncPower: root.studio.increasePower = increment
                    onDecPower: root.studio.decreasePower = decrement
                }
                Component{
                    id: modlightBtComponent
                    Button{
                        id: btModLight

                        signal selected()

                        anchors.fill: parent
                        zoomAllowed: true

                        Keys.onBacktabPressed:{
                            event.accepted = true
                            mainEnergyEnergy.forceActiveFocus()
                        }
                        focus: true
                        areDifferent: root.studio.switchModLight === 2
                        isActive: root.studio.switchModLight

                        text: "MOD. LIGHT"
                        onClicked: {
                            root.studio.switchModLight = btModLight.isActive
                            forceActiveFocus()
                            selected()
                        }
                    }
                }
                Component{
                    id: standbyBtComponent
                    Button{
                        id: btStandby

                        signal selected()
                        anchors.fill: parent
                        zoomAllowed: true
                        focus: true
                        areDifferent: root.studio.switchStandby === 2
                        isActive: root.studio.switchStandby


                        text: "STANDBY"
                        onClicked: {
                            root.studio.switchStandby = btStandby.isActive
                            forceActiveFocus()
                            selected()
                        }
                    }
                }
                Component{
                    id: testBtComponent
                    Button{
                        id: btTest

                        signal selected()
                        anchors.fill: parent
                        zoomAllowed: true

                        focus: true

                        function resetState(){ btTest.state = "" }
                        text: "TEST"
                        function doAction(){
                            btTest.isActive = false;
                            btTest.state = "flash"
                            root.studio.testFlash = true
                            root.studio.showCogni = true
                            selected()
                        }

                        onClicked: {
                            doAction()
                            forceActiveFocus()
                        }
                        states: State{
                            name: "flash"
                            PropertyChanges { target: flashEffect}
                        }

                        transitions: Transition{
                            from: ""; to: "flash"
                            SequentialAnimation{
                                PropertyAnimation{target: flashEffect; property: "opacity"; to: 1.0; duration: 1}
                                PropertyAnimation{target: flashEffect; property: "opacity"; to: 0.0; duration: 400}
                                ScriptAction{ script: btTest.resetState() }
                            }

                        }
                        Rectangle{
                            id: flashEffect
                            anchors.fill: parent
                            opacity: 0.0
                            color: "white"
                        }
                    }
                }
                Component{
                    id: applicComboComponent

                    BronComboBox{
                        id: cbMore
                        anchors.fill: parent

                        property var fa: studio.application
                        onFaChanged: {
                            switch(fa){
                            case ParaSelectors.None:
                                comboSel = ["SEQUENCE", "FREEMASK", "ALTERNATE"]
                                displayText = "MORE"
                                currentIndex = 0;
                                break
                            case ParaSelectors.Sequence:
                                comboSel = ["RESET","SEQUENCE"]
                                currentIndex = 1
                                break
                            case ParaSelectors.Freemask:
                                comboSel = ["RESET", "FREEMASK"]
                                currentIndex = 1
                                break
                            case ParaSelectors.Alternate:
                                comboSel = ["RESET","ALTERNATE"]
                                currentIndex = 1
                                break
                            }
                        }
                        property var comboSel: ["SEQUENCE", "FREEMASK", "ALTERNATE"]
                        focus: true
                        function setReset(doDefault){
                            var settings = {}
                            settings.isReset = doDefault ? false : true
                            studio.application = settings
                        }
                        function doSelection(index, text){
                            switch(text){
                            case "RESET":
                                displayText = "MORE"
                                focusItem = cbMore
                                setReset(false)
                                break
                            case "SEQUENCE":
                                displayText = text
                                focusItem = cbMore
                                stack.push("qrc:///qml/SequenceScene.qml", {"studio" : root.studio})
                                break
                            case "FREEMASK":
                                displayText = text
                                focusItem = cbMore
                                stack.push("qrc:///qml/FreemaskScene.qml", {"studio" : root.studio})
                                break
                            case "ALTERNATE":
                                displayText = text
                                focusItem = cbMore
                                stack.push("qrc:///qml/AlternateScene.qml", {"studio" : root.studio})
                                break
                                //                        case "DEFAULT":
                                //                            displayText = text
                                //                            focusItem = cbMore
                                //                            setReset(true)
                            }
                            selected()
                        }

                        font.family: bronFontMedium.name
                        font.bold: true
                        font.pointSize: DisplayCtrl.point15Size * DisplayCtrl.userScale
                        displayText: "MORE"
                        Keys.onPressed: {
                            if(event.key === Qt.Key_Return){
                                if(false === isOpen){
                                    doSelection(currentIndex, currentText)
                                    event.accepted = true
                                }
                            }
                        }

                        // only enabled if more than one element (whitch can take part) is active
                        enabled: geniGrid.model.count + devicesGrid.model.count > 1 ? true : false
                        model: comboSel

                        onActivated: {
                            forceActiveFocus()
                            currentIndex = index
                            if(isOpen){
                                doSelection(index, currentText)
                            }
                        }
                    }
                }
                Component{
                    id: speedBtComponent
                    Button{
                        id: btSpeed

                        signal selected()

                        anchors.fill: parent
                        zoomAllowed: true
                        focus: true
                        areDifferent: root.studio.speed === 2
                        isActive: root.studio.speed

                        text: "SPEED"
                        onClicked: {
                            root.studio.speed = btSpeed.isActive
                            forceActiveFocus()
                            selected()
                        }
                    }
                }
                Component{
                    id: audioBtComponent
                    Button{
                        id: btAudio

                        signal selected()

                        anchors.fill: parent
                        zoomAllowed: true
                        focus: true
                        areDifferent: root.studio.audio === 2
                        isActive: root.studio.audio

                        text: "AUDIO"
                        onClicked: {
                            root.studio.audio = btAudio.isActive
                            forceActiveFocus()
                            selected()
                        }
                    }
                }
                Component{
                    id: cogniBtComponent
                    Button{
                        id: btCogni

                        signal selected()

                        anchors.fill: parent
                        zoomAllowed: true

                        focus: true
                        areDifferent: root.studio.switchCogniLight === 2
                        isActive: root.studio.switchCogniLight

                        text: "COGN.LIGHT"
                        onClicked: {
                            root.studio.switchCogniLight = btCogni.isActive
                            forceActiveFocus()
                            selected()
                        }
                    }
                }
                Component{
                    id: rfsBtComponent
                    Button{
                        id: rfsBt

                        signal selected()

                        anchors.fill: parent
                        zoomAllowed: true
                        focus: true
                        areDifferent: root.studio.switchRemote === (ParaSelectors.RcCellIrRfs + 1)
                        isActive: root.studio.switchRemote === ParaSelectors.RcRfs

                        text: "RFS"
                        onClicked: {
                            root.studio.switchRemote = rfsBt.isActive ? ParaSelectors.RcRfs : ParaSelectors.RcOff
                            forceActiveFocus()
                            selected()
                        }
                    }

                }

                ListModel{
                    id: btListModel
                    ListElement{
                        name: "modLightButton"
                    }
                    ListElement{
                        name: "standbyButton"
                    }
                    ListElement{
                        name: "testButton"
                    }
                    ListElement{
                        name: "appliButton"
                    }
                    ListElement{
                        name: "speedButton"
                    }
                    ListElement{
                        name: "audioButton"
                    }
                    ListElement{
                        name: "congiButton"
                    }
                    ListElement{
                        name: "rfsButton"
                    }
                }
                ListView{
                    id: btLine

                    property bool isIn: false

                    anchors.top: mainEnergyEnergy.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width * 0.05
                    width: parent.width * 0.9// * DisplayCtrl.userScale
                    height: DisplayCtrl.dp(60) * DisplayCtrl.userScale
                    interactive: true
                    clip: true
                    focus: true
                    snapMode: ListView.SnapOneItem
                    highlightFollowsCurrentItem: true

                    ScrollBar.horizontal: BronScrollBar{
                        id: controlsSB
                        parent: btLine.parent
                        anchors.top: btLine.bottom
                        anchors.topMargin: DisplayCtrl.gridMargin/2
                        anchors.left: btLine.left
                        anchors.right: btLine.right
                        show: btLine.isIn
                    }

                    orientation: ListView.Horizontal
                    spacing: DisplayCtrl.gridMargin * DisplayCtrl.userScale

                    model: btListModel
                    delegate: FocusScope{
                        id: delegateItem
                        width: DisplayCtrl.itemSize * DisplayCtrl.userScale
                        height: DisplayCtrl.dp(60) * DisplayCtrl.userScale
                        focus: true
                        property string name: model.name
                        Loader{
                            id: componentLoader
                            focus: true
                            anchors.fill: parent
                        }
                        Connections{
                            target: componentLoader.item
                            onSelected: {
                                btLine.currentIndex = index
                            }
                        }

                        Component.onCompleted: {
                            if(name === "modLightButton"){
                                componentLoader.sourceComponent = modlightBtComponent;
                            }
                            else if(name === "standbyButton"){
                                componentLoader.sourceComponent = standbyBtComponent
                            }
                            else if(name === "testButton"){
                                componentLoader.sourceComponent = testBtComponent
                            }
                            else if(name === "appliButton"){
                                componentLoader.sourceComponent = applicComboComponent
                            }
                            else if(name === "speedButton"){
                                componentLoader.sourceComponent = speedBtComponent
                            }
                            else if(name === "audioButton"){
                                componentLoader.sourceComponent = audioBtComponent
                            }
                            else if(name === "congiButton"){
                                componentLoader.sourceComponent = cogniBtComponent
                            }
                            else if(name === "rfsButton"){
                                componentLoader.sourceComponent = rfsBtComponent
                            }
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        propagateComposedEvents: true

                        onEntered: btLine.isIn = true
                        onExited: btLine.isIn = false
                    }

                }

                Rectangle{
                    id: mainSettingBottenLine
                    anchors.top: studiosCtrl.bottom
                    width: parent.width
                    height: DisplayCtrl.dp(2)
                    color: "white"
                }
                Rectangle{
                    id: mainSettingRightLine
                    anchors.left: parent.right
                    anchors.top: parent.top
                    width: DisplayCtrl.dp(2)
                    height: parent.height
                    color: "white"
                }
            }

            ///////////////////////////////////////////////////////////////////////////////////////////////////
            // Groups
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            DelegateModel {
                id: groupsDisplayDelegateModel
                model: root.studio.groups
                property var element: root.studio.groups.element
                onElementChanged:{
                    var groups = groupsDisplayDelegateModel.items.count
                    for(var ix = 0; ix < groups; ix++){
                        var entry = groupsDisplayDelegateModel.items.get(ix)
                        var devs = entry.model.devices.count
                        if(devs > 1){
                            entry.inMoreThanOne = true
                        }
                        else if(devs < 2){
                            entry.inMoreThanOne = false
                        }
                    }
                    groupsGrid.adjustHeight()
                }

                groups: [
                    DelegateModelGroup {
                        id: groupDefault
                        includeByDefault: true
                        name: "groupDefault"
                        onChanged:{
                            for(var i = 0; i < count; i++){
                                var entry = get(i)
                                var devs = entry.model.devices.count
                                if(devs > 1){
                                    entry.inMoreThanOne = true
                                }
                                else if(devs < 2){
                                    entry.inMoreThanOne = false
                                }
                            }
                            groupsGrid.adjustHeight()
                        }

                    },
                    DelegateModelGroup {
                        id: moreDelegateGroup
                        includeByDefault: false
                        name: "moreThanOne"
                    }
                ]
                filterOnGroup: "moreThanOne"

                Component.onCompleted: {
                    for(var ix = 0; ix < model.count; ix++ ){
                        var entry = items.get(ix)
                        var devs = entry.model.devices.count
                        if(devs > 1){
                            entry.inMoreThanOne = true
                        }
                        else if(devs < 2){
                            entry.inMoreThanOne = false
                        }
                    }
                    groupsGrid.adjustHeight()
                }


                delegate: Item{
                    id: delitem

                    property alias text: groupText.text
                    property alias cogniCol: cogniColor.color

                    width: groupsGrid.cellWidth
                    height: groupsGrid.cellHeight

                    focus: true

                    Rectangle{
                        id: group
                        anchors.left: parent.left
                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(80)
                        color: BronColors.bronBlue
                        border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue :delitem.activeFocus ? "white" : BronColors.bronBlue
                        border.width: DisplayCtrl.dp(4)
                        Text{
                            id: groupText
                            anchors.centerIn: parent
                            font.family: bronFontBold.name
                            font.bold: true
                            font.pixelSize: parent.height/3
                            text: "GROUP "+ model.address
                        }
                        Rectangle {
                            id: cogniBack
                            width: parent.width / 7
                            height: cogniBack.width
                            anchors.top: parent.top
                            anchors.right: parent.right
                            color: BronColors.bronBlack
                            Rectangle{
                                id: cogniColor
                                width: parent.width * 0.85
                                height: cogniColor.width
                                anchors.top: parent.top
                                anchors.right: parent.right
                                color: BronColors.intToColer(model.cogniColor)
                                Text{
                                    id: groupAddress
                                    anchors.fill: parent
                                    font.family: bronFont.name
                                    font.pixelSize: DisplayCtrl.sp(parent.height / 1.5)
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    text: model.address
                                    color: "black"
                                }
                            }
                        }
                    }

                    MouseArea{
                        id: maerea
                        anchors.fill: group
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onClicked:{
                            groupsGrid.currentIndex = delitem.DelegateModel.moreThanOneIndex
                            focusItem = delitem

                            if(mouse.button  === Qt.RightButton){
                                model.wink = 1
                            }
                            if(mouse.button === Qt.LeftButton){
                                toolTitle = "ADJUST GROUP"
                                if(setterLoader.status === Loader.Ready){
                                    if(setterLoader.item.group !== model){
                                        setterLoader.item.back()
                                        setterLoader.nextSource = "qrc:///qml/GroupSelection.qml"
                                        setterLoader.nextGroup = model
                                        setterLoader.nextCogniCol = groupsGrid.currentItem.cogniCol
                                        setterLoader.nextSelGroup = groupsGrid.currentItem.text
                                    }
                                }
                                else{
                                    setterLoader.setSource("qrc:///qml/GroupSelection.qml",
                                                           {"group": model,
                                                               "cogniCol": groupsGrid.currentItem.cogniCol,
                                                               "selGroup": groupsGrid.currentItem.text})
                                }
                            }
                        }
                        onPressAndHold: {
                            groupsGrid.currentIndex = delitem.DelegateModel.moreThanOneIndex
                            focusItem = delitem
                            model.standby = model.standby == 0 ? 1 :0
                        }
//                        onExited: {
//                            console.log("ma exited")
//                            var enabled = model.congiEnable
//                            // disable cogni color shortly to clear the cogni lights
//                            model.cogniEnable = false;
//                            if(enabled !== false){
//                                model.cogniEnable = enabled
//                            }

//                        }
                    }
                    Keys.onReturnPressed:{
                        groupsGrid.currentIndex = delitem.DelegateModel.moreThanOneIndex
                        focusItem = delitem
                        event.accepted = true
                        toolTitle = "ADJUST GROUP"
                        if(setterLoader.status === Loader.Ready){
                            if(setterLoader.item.group !== model){
                                setterLoader.item.back()
                                setterLoader.nextSource = "qrc:///qml/GroupSelection.qml"
                                setterLoader.nextGroup = model
                                setterLoader.nextCogniCol = groupsGrid.currentItem.cogniCol
                                setterLoader.nextSelGroup = groupsGrid.currentItem.text
                            }
                        }
                        else{
                            setterLoader.setSource("qrc:///qml/GroupSelection.qml",
                                                   {"group": model,
                                                       "cogniCol": groupsGrid.currentItem.cogniCol,
                                                       "selGroup": groupsGrid.currentItem.text})
                        }
                    }
                }
            }


            GridView{
                id: groupsGrid
                property bool rbGActive: rbG.active
                property bool changeLayout: root.showGroupsRight
                property bool isIn: false

                onRbGActiveChanged: {
                    // if active, calculate new height!
                    if(rbGActive){
                        groupsGrid.adjustHeight()
                    }
                }

                onChangeLayoutChanged: groupsGrid.adjustHeight()

                function getHeight(){
                    if(root.showGroupsRight){
                        return groupsGrid.cellHeight * 4 //gridLayout.height
                    }
                    else{
                        if(groupsGrid.model.count > 0 && rbGActive){
                            var rowElements = groupsGrid.width / groupsGrid.cellWidth
                            return Math.ceil(groupsGrid.model.count / rowElements) * groupsGrid.cellHeight// + groupsGrid.headerItem.height
                        }
                        else{
                            return 0;
                        }
                    }
                }

                function adjustHeight(){
                    Layout.preferredHeight = getHeight()
                }

                onWidthChanged: {
                    Layout.preferredHeight = getHeight()
                }

                visible: root.showGroupsRight ? true : (groupsGrid.model.count > 0 && rbG.active) ? true : false

                Layout.topMargin: visible ? DisplayCtrl.gridMargin : 0
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.leftMargin: DisplayCtrl.portraitWidth * 0.05
                //                Layout.rightMargin: DisplayCtrl.portraitWidth * 0.05
                Layout.preferredWidth: root.showGroupsRight ? root.width - studiosCtrl.width - DisplayCtrl.portraitHeight * 0.05
                                                            : root.width - DisplayCtrl.portraitWidth * 0.05
                Layout.preferredHeight: 0

                //                property real ww: Layout.preferredWidth

                clip: interactive
                interactive: root.showGroupsRight ? (groupsGrid.count > 4 ? true : false )
                                                  : false

                flow: root.showGroupsRight ? GridView.TopToBottom : GridView.LeftToRight
                onFlowChanged: {
                    if(flow === GridView.TopToBottom){
                        groupsGrid.adjustHeight()
                    }
                }

                snapMode: GridView.SnapToRow
                focus: true
                KeyNavigation.tab: geniGrid
                Keys.onBackPressed: btListModel.forceActiveFocus()

                cellHeight: DisplayCtrl.dp(80) + DisplayCtrl.gridMargin
                cellWidth: DisplayCtrl.itemSize + DisplayCtrl.gridMargin
                model: groupsDisplayDelegateModel

                ScrollBar.horizontal: BronScrollBar {
                    show: groupsGrid.isIn
                }

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    propagateComposedEvents: true

                    onEntered: groupsGrid.isIn = true
                    onExited: groupsGrid.isIn = false
                }

            }
        }
        Rectangle{
            id: groupsBottenLine
            visible: true//groupsGrid.visible
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: DisplayCtrl.dp(2)
            color: "white"
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Groups end
        ///////////////////////////////////////////////////////////////////////////////////////////////////


        Flickable{
            id: flicker
            property bool isIn: false

            focus: true
            Layout.topMargin: DisplayCtrl.gridMargin
            Layout.preferredWidth: root.width
            Layout.preferredHeight: root.height - gridLayout.height - groupsBottenLine.height
                                    - 2 * DisplayCtrl.gridMargin
            contentWidth: root.width//(DisplayCtrl.itemSize * 4 + DisplayCtrl.gridMargin * 3)/0.9  //root.width
            contentHeight: /*groupsGrid.height +*/ geniGrid.height + devicesGrid.height + DisplayCtrl.itemSize//flickerColumn.height


            clip: true

            ScrollBar.vertical: BronScrollBar {
                show: flicker.isIn
            }

            ColumnLayout{
                id: flickerColumn
                width: parent.width
                spacing: 0

                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // Generators
                ///////////////////////////////////////////////////////////////////////////////////////////////////
                DelegateModel {
                    id: geniDisplayDelegateModel

                    model: root.studio.devices
                    groups: [
                        DelegateModelGroup{
                            id: geniDefault
                            includeByDefault: true
                            name: "geniDefault"
                            onChanged:{
                                for(var i = 0; i < count; i++){
                                    var entry = get(i)
                                    if(entry.model.type === 1){
                                        if(entry.inGenis === false){
                                            entry.inGenis = true
                                        }
                                    }
                                    else{
                                        entry.inGenis = false
                                    }
                                }
                                geniGrid.adjustHeight()
                            }
                        },
                        DelegateModelGroup {
                            id: generatorDevices
                            includeByDefault: false
                            name: "genis"
                        }
                    ]
                    filterOnGroup: "genis"
                    delegate: FocusScope{
                        id: geniDlg
                        width: scalingItem.width//geniGrid.cellWidth//aGeni.width + lampList.contentWidth
                        height: scalingItem.height
                        x: scalingItem.x
                        y: scalingItem.y
                        property var geni: model
                        Item{
                            id: scalingItem
                            width:  geniGrid.cellWidth
                            height: geniGrid.cellHeight
                            transform: DisplayCtrl.userZoom

//                            property var geni: model

                            Generator{
                                id: aGeni
                                focus: true
                                generator: geni

                                onFocusItemChanged: root.focusItem = focusItem
                                apertureMode: root.apertureMode

                                onLongClicked: {
                                    geniGrid.currentIndex = geniDlg.DelegateModel.genisIndex
                                    focusItem = parent
                                    geni.test = 1
                                }
                                onMoreLongClicked: {
                                    geniGrid.currentIndex = geniDlg.DelegateModel.genisIndex
                                    focusItem = parent
                                    geni.standby = !geni.standby
                                }
                                onMoreClicked: {
                                    geniGrid.currentIndex = geniDlg.DelegateModel.genisIndex
                                    focusItem = parent
                                    toolTitle = "SETTINGS"
                                    var name = geniDlg.geni.fullModelName
                                    if(name === "Scoro 1600 E" || name === "Scoro 3200 E"){
                                        if(popUpLoader.status === Loader.Ready){
                                            if(popUpLoader.item.device !== geniDlg.geni){
                                                popUpLoader.item.back()
                                                popUpLoader.nextSource = "qrc:///qml/ScoroEMore.qml"
                                                popUpLoader.nextDevice = geniDlg.geni
                                                popUpLoader.nextHeight = root.viewHeight
                                            }
                                        }
                                        else{
                                            popUpLoader.setSource("qrc:///qml/ScoroEMore.qml", {"device": geniDlg.geni, "height" : root.viewHeight})
                                        }
                                    }
                                    else if(name === "Scoro 3200 S" || name === "Scoro 1600 S"){
                                        if(popUpLoader.status === Loader.Ready){
                                            if(popUpLoader.item.device !== geniDlg.geni){
                                                popUpLoader.item.back()
                                                popUpLoader.nextSource = "qrc:///qml/ScoroSMore.qml"
                                                popUpLoader.nextDevice = geniDlg.geni
                                                popUpLoader.nextHeight = root.viewHeight
                                            }
                                        }
                                        else{
                                            popUpLoader.setSource("qrc:///qml/ScoroSMore.qml", {"device": geniDlg.geni, "height" : root.viewHeight})
                                        }
                                    }
                                }
                                onGeneratorClicked: {
                                    geniGrid.currentIndex = geniDlg.DelegateModel.genisIndex
                                    focusItem = parent
                                    toolTitle = "ADJUST UNIT"
                                    if(setterLoader.status === Loader.Ready){
                                        if(setterLoader.item.device !== geniDlg.geni){
                                            setterLoader.item.back()
                                            setterLoader.nextSource = "qrc:///qml/DeviceSelection.qml"
                                            setterLoader.nextDevice = geniDlg.geni
                                            setterLoader.nextSelDevice = "GENERATOR " + (geniDlg.DelegateModel.genisIndex + 1)
                                            setterLoader.nextCogniCol = "transparent"
                                        }
                                    }
                                    else{
                                        setterLoader.setSource("qrc:///qml/DeviceSelection.qml",
                                                               {"device": geniDlg.geni,
                                                                   "selDevice": "GENERATOR " + (geniDlg.DelegateModel.genisIndex + 1),
                                                                   "cogniCol": "transparent"})
                                    }
                                }
                            }

                            ListView{
                                id: lampList
                                width: geniDlg.width - aGeni.width - DisplayCtrl.dp(20)
                                height: geniGrid.cellHeight
                                anchors.left: aGeni.right
                                anchors.top: aGeni.top
                                anchors.leftMargin: DisplayCtrl.dp(20)
                                orientation: ListView.Horizontal
                                spacing: DisplayCtrl.gridMargin
                                focus: true
                                interactive: true
                                clip: false
                                model: geni.lamps
                                delegate: Rectangle{
                                    id: lampDelegate
                                    focus: true
                                    height: DisplayCtrl.itemSize //* DisplayCtrl.userScale
                                    width: height
                                    Keys.onPressed: {
                                        if (event.key === Qt.Key_Return){
                                            if(event.modifiers & Qt.ShiftModifier){
                                                lampList.currentIndex = index
                                                event.accepted = true
                                                focusItem = parent
                                                lamp.moreClickedLamp(lamp)
                                            }
                                            else{
                                                lampList.currentIndex = index
                                                event.accepted = true
                                                focusItem = parent
                                                lamp.lampClickedLamp(lamp)
                                            }
                                        }
                                    }
                                    color: "yellow"
                                    Lamp{
                                        id: lamp
                                        anchors.left: parent.left
                                        anchors.top: parent.top
                                        useChannel: true
                                        channel: model.lampChannel + 1
                                        cogniColor: BronColors.intToColer(model.cogniColor)
                                        device: model
                                        focus: true
                                        apertureMode: root.apertureMode
                                        onMoreClicked:{
                                            lampList.currentIndex = index
                                            focusItem = parent
                                            toolTitle = "SETTINGS"
                                            if(popUpLoader.status === Loader.Ready){
                                                if(popUpLoader.item.device !== lamp.device){
                                                    popUpLoader.item.back()
                                                    popUpLoader.nextSource = "qrc:///qml/GenLampMore.qml"
                                                    popUpLoader.nextDevice = lamp.device
                                                    popUpLoader.nextHeight = root.viewHeight
                                                }
                                            }
                                            else{
                                                popUpLoader.setSource("qrc:///qml/GenLampMore.qml", {"device": lamp.device, "height" : root.viewHeight})
                                            }
                                        }
                                        onLampClicked: {
                                            lampList.currentIndex = index
                                            focusItem = parent
                                            toolTitle = "ADJUST UNIT"
                                            if(setterLoader.status === Loader.Ready){
                                                if(setterLoader.item.device !== lamp.device){
                                                    setterLoader.item.back()
                                                    setterLoader.nextSource = "qrc:///qml/DeviceSelection.qml"
                                                    setterLoader.nextDevice = lamp.device
                                                    setterLoader.nextSelDevice = "Lamp " + lamp.channel
                                                    setterLoader.nextCogniCol = lamp.cogniColor
                                                }
                                            }
                                            else{
                                                setterLoader.setSource("qrc:///qml/DeviceSelection.qml",
                                                                       {"device": lamp.device,
                                                                           "selDevice": "Lamp " + lamp.channel,
                                                                           "cogniCol": lamp.cogniColor})
                                            }
                                        }
                                        hasFocus: parent.activeFocus
                                    }
                                    Rectangle{
                                        id: connector
                                        anchors.right: lamp.left
                                        anchors.verticalCenter: lamp.verticalCenter
                                        color: BronColors.bronBlue
                                        width: lampList.spacing
                                        height: lamp.height / 15
                                    }
                                }
                            }
                        }
                    }
                }
                GridView{
                    id: geniGrid
                    property bool headerFinished: false

                    flow: GridView.FlowTopToBottom

                    function getHeight(){
                        if(geniGrid.model.count > 0){
                            // allways one scoro in one line
                            return geniGrid.model.count * geniGrid.cellHeight// + geniGrid.headerItem.height
                        }
                        else{
                            return 0
                        }
                    }
                    onWidthChanged: {
                        Layout.preferredHeight = getHeight()
                    }
                    function adjustHeight(){
                        Layout.preferredHeight = getHeight()
                    }

                    Layout.alignment: Qt.AlignLeft//Qt.AlignHCenter
                    Layout.leftMargin: DisplayCtrl.portraitWidth * 0.05
                    Layout.rightMargin: DisplayCtrl.portraitWidth * 0.05
                    Layout.preferredWidth: parent.width - DisplayCtrl.portraitWidth * 0.1
                    Layout.preferredHeight: 0
                    clip: true
                    interactive: true

                    KeyNavigation.tab: devicesGrid
                    KeyNavigation.backtab: groupsGrid
                    focus: true

                    cellHeight: (DisplayCtrl.itemSize + DisplayCtrl.gridMargin) * DisplayCtrl.userScale
                    cellWidth: (cellHeight - DisplayCtrl.gridMargin) * 4//width * DisplayCtrl.userScale//cellHeight
                    onCellHeightChanged: adjustHeight()

                    model: geniDisplayDelegateModel

                }

                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // Generators end
                ///////////////////////////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // Devices
                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // used to scale delegates

                DelegateModel {
                    id: devicesDisplayDelegateModel
                    model: root.studio.devices
                    groups: [
                        DelegateModelGroup{
                            id: deviceDefault
                            includeByDefault: true
                            name: "deviceDefault"
                            onChanged: {
                                for(var i = 0; i < count; i++){
                                    var entry = get(i)
                                    if(entry.model.type === 0){
                                        if(entry.inCompact === false){
                                            entry.inCompact = true
                                        }
                                    }
                                    else{
                                        entry.inCompact = false
                                    }
                                }
                                devicesGrid.adjustHeight()
                            }
                        },
                        DelegateModelGroup {
                            id: compactDevices
                            includeByDefault: false
                            name: "compact"
                        }
                    ]
                    filterOnGroup: "compact"
                    Component.onCompleted: {
                        var rowCount = model.count
                        for( var i = 0;i < rowCount;i++ ) {
                            var entry = model.get(i)
                            if(entry.type === 0) {
                                items.get(i).inCompact = true
                            }
                        }
                        devicesGrid.adjustHeight()
                    }
                    delegate: Item{
                        id: devicesDlgitem
                        width: devicesGrid.cellWidth
                        height: devicesGrid.cellHeight
                        focus: true
                        Keys.onPressed: {
                            if (event.key === Qt.Key_Return){
                                if(event.modifiers & Qt.ShiftModifier){
                                    devicesGrid.currentIndex = index
                                    event.accepted = true
                                    focusItem = devicesDlgitem
                                    toolTitle = "SETTINGS"
                                    var name = model.fullModelName
                                    if(name === "Siros 400 " || name === "Siros 800 "){
                                        if(popUpLoader.status === Loader.Ready){
                                            popUpLoader.item.back()
                                            popUpLoader.nextSource = "qrc:///qml/SirosMore.qml"
                                            popUpLoader.nextDevice = model
                                            popUpLoader.nextHeight = root.viewHeight
                                        }
                                        else{
                                            popUpLoader.setSource("qrc:///qml/SirosMore.qml", {"device": model, "height" : root.viewHeight})
                                        }

                                    }
                                    else if(name === "Siros 400 S" || name === "Siros 800 S"){
                                        if(popUpLoader.status === Loader.Ready){
                                            popUpLoader.item.back()
                                            popUpLoader.nextSource = "qrc:///qml/SirosSMore.qml"
                                            popUpLoader.nextDevice = model
                                            popUpLoader.nextHeight = root.viewHeight
                                        }
                                        else{
                                            popUpLoader.setSource("qrc:///qml/SirosSMore.qml", {"device": model, "height" : root.viewHeight})
                                        }
                                    }
                                    else if(name === "Siros 400 L" || name === "Siros 800 L"){
                                        if(popUpLoader.status === Loader.Ready){
                                            popUpLoader.item.back()
                                            popUpLoader.nextSource = "qrc:///qml/SirosLMore.qml"
                                            popUpLoader.nextDevice = model
                                            popUpLoader.nextHeight = root.viewHeight
                                        }
                                        else{
                                            popUpLoader.setSource("qrc:///qml/SirosLMore.qml",  {"device": model, "height" : root.viewHeight})
                                        }
                                    }
                                }
                                else{
                                    devicesGrid.currentIndex = index
                                    event.accepted = true
                                    focusItem = devicesDlgitem
                                    toolTitle = "ADJUST UNIT"
                                    if(setterLoader.status === Loader.Ready){
                                        setterLoader.item.back()
                                        setterLoader.nextSource = "qrc:///qml/DeviceSelection.qml"
                                        setterLoader.nextDevice = model
                                        setterLoader.nextSelDevice = "LAMP " + (index + 1)
                                        setterLoader.nextCogniCol = aDevice.cogniColor
                                    }
                                    else{
                                        setterLoader.setSource("qrc:///qml/DeviceSelection.qml",
                                                               {"device": model,
                                                                   "selDevice": "LAMP " + (index + 1),
                                                                   "cogniCol": aDevice.cogniColor})
                                    }
                                }
                            }
                        }
                        Lamp{
                            id: aDevice
                            device: model
                            zoom: DisplayCtrl.userZoom
                            anchors.left: parent.left
                            cogniColor: BronColors.intToColer(model.cogniColor)
                            apertureMode: root.apertureMode
                            onLongClicked: {
                                devicesGrid.currentIndex = devicesDlgitem.DelegateModel.compactIndex
                                focusItem = parent
                                model.test = 1
                            }
                            onMoreLongClicked: {
                                devicesGrid.currentIndex = devicesDlgitem.DelegateModel.compactIndex
                                focusItem = parent
                                model.standby = !model.standby
                            }

                            onMoreClicked: {
                                devicesGrid.currentIndex = devicesDlgitem.DelegateModel.compactIndex
                                focusItem = parent
                                toolTitle = "SETTINGS"
                                var name = model.fullModelName
                                if(name === "Siros 400 " || name === "Siros 800 "){
                                    if(popUpLoader.status === Loader.Ready){
                                        if(popUpLoader.item.device !== model){
                                            popUpLoader.item.back()
                                            popUpLoader.nextSource = "qrc:///qml/SirosMore.qml"
                                            popUpLoader.nextDevice = model
                                            popUpLoader.nextHeight = root.viewHeight
                                        }
                                    }
                                    else{
                                        popUpLoader.setSource("qrc:///qml/SirosMore.qml", {"device": model, "height" : root.viewHeight})
                                    }

                                }
                                else if(name === "Siros 400 S" || name === "Siros 800 S"){
                                    if(popUpLoader.status === Loader.Ready){
                                        if(popUpLoader.item.device !== model){
                                            popUpLoader.item.back()
                                            popUpLoader.nextSource = "qrc:///qml/SirosSMore.qml"
                                            popUpLoader.nextDevice = model
                                            popUpLoader.nextHeight = root.viewHeight
                                        }
                                    }
                                    else{
                                        popUpLoader.setSource("qrc:///qml/SirosSMore.qml", {"device": model, "height" : root.viewHeight})
                                    }
                                }
                                else if(name === "Siros 400 L" || name === "Siros 800 L"){
                                    if(popUpLoader.status === Loader.Ready){
                                        if(popUpLoader.item.device !== model){
                                            popUpLoader.item.back()
                                            popUpLoader.nextSource = "qrc:///qml/SirosLMore.qml"
                                            popUpLoader.nextDevice = model
                                            popUpLoader.nextHeight = root.viewHeight
                                        }
                                    }
                                    else{
                                        popUpLoader.setSource("qrc:///qml/SirosLMore.qml",  {"device": model, "height" : root.viewHeight})
                                    }
                                }
                            }
                            onLampClicked: {
                                devicesGrid.currentIndex = devicesDlgitem.DelegateModel.compactIndex
                                focusItem = parent
                                toolTitle = "ADJUST UNIT"
                                if(setterLoader.status === Loader.Ready){
                                    if(setterLoader.item.device !== model){
                                        setterLoader.item.back()
                                        setterLoader.nextSource = "qrc:///qml/DeviceSelection.qml"
                                        setterLoader.nextDevice = model
                                        setterLoader.nextSelDevice = "LAMP " + (devicesDlgitem.DelegateModel.compactIndex + 1)
                                        setterLoader.nextCogniCol = aDevice.cogniColor
                                    }
                                }
                                else{
                                    setterLoader.setSource("qrc:///qml/DeviceSelection.qml",
                                                           {"device": model,
                                                               "selDevice": "LAMP " + (devicesDlgitem.DelegateModel.compactIndex + 1),
                                                               "cogniCol": aDevice.cogniColor})
                                }
                            }
                            hasFocus: parent.activeFocus
                        }
                    }
                }
                GridView{
                    id: devicesGrid
                    function getHeight(){
                        if(devicesGrid.model.count > 0 && devicesGrid.width > 0){
                            var rowElements = devicesGrid.width / devicesGrid.cellWidth
                            return Math.ceil(devicesGrid.model.count / rowElements) * devicesGrid.cellHeight// + devicesGrid.headerItem.height
                        }
                        else{
                            return 0;
                        }
                    }
                    onWidthChanged: {
                        Layout.preferredHeight = getHeight()
                    }
                    function adjustHeight(){
                        Layout.preferredHeight = getHeight()
                    }
                    Layout.preferredHeight: 0
                    Layout.alignment: Qt.AlignLeft//Qt.AlignHCenter
                    Layout.leftMargin: DisplayCtrl.portraitWidth * 0.05
                    Layout.preferredWidth: parent.width - DisplayCtrl.portraitWidth * 0.05

                    clip: true
                    interactive: true
                    focus: true
                    KeyNavigation.tab: btback
                    KeyNavigation.backtab: geniGrid

                    cellHeight: (DisplayCtrl.itemSize + DisplayCtrl.gridMargin) * DisplayCtrl.userScale
                    cellWidth: cellHeight
                    onCellHeightChanged: adjustHeight()
                    model: devicesDisplayDelegateModel
                }

                ///////////////////////////////////////////////////////////////////////////////////////////////////
                // end devices
                ///////////////////////////////////////////////////////////////////////////////////////////////////

            }

            Item{
                Layout.fillHeight: true
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


                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    propagateComposedEvents: true

                    onEntered: flicker.isIn = true
                    onExited: flicker.isIn = false
                }

            }

        }
    }

    // do version handling here!! a write of device.swVersion starts the read process
    Connections{
        target: studioMgr
        property bool wasShown: false
        onSwVersionRead:{
            if(deviceType === "sirosS"){
                if(version < 4909) versionWaring.showDialog()
            }
            else if(deviceType === "sirosS"){
                if(version < 4909) versionWaring.showDialog()
            }
            else if(deviceType === "sirosL"){
                if(version < 5204) versionWaring.showDialog()
            }
            else if(deviceType === "scoro"){

            }
            else if(deviceType === "genLamp"){

            }
        }
        onErrorChanged:{
            errorMsg.text = errorText + "!"
            errorMsg.visible = true
        }
    }

    MessageDialog{
        id: errorMsg
        title: "Error!"
        text: "!"
        icon: StandardIcon.Warning
    }



    MessageDialog{
        id: versionWaring
        property bool wasShown: false
        function showDialog(){
            if(!wasShown){
                wasShown = true
                visible = true
            }
        }
        title: "Version Waring!"
        text: "update Siros to actual Version \nto support all functionality"
        icon: StandardIcon.Warning
    }

    header: ToolBar{

        id: toolbar
        //        implicitWidth: parent.width
        //        implicitHeight: btback.height//DisplayCtrl.dp(70)
        background: Rectangle {
            anchors.fill: parent
            color: BronColors.bronBlack
        }

        RowLayout{
            anchors.fill: parent
            spacing: DisplayCtrl.gridMargin
            BronToolButton{
                id: btback

                focus: true
                KeyNavigation.tab: btHelp
                KeyNavigation.backtab: devicesGrid
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: DisplayCtrl.dp(10)

                font.family: googleMaterial.name
                text: DisplayCtrl.icBack

                onClicked: {

                    if(settingsLoader.state == "" && popUpLoader.state == "" && setterLoader.state == ""){
                        root.studio.online = false
                        stack.pop()
                    }
                    else{
                        if(setterLoader.status == Loader.Ready){
                            setterLoader.item.back()
                        }
                        if(popUpLoader.status == Loader.Ready ){
                            popUpLoader.item.back()
                        }
                        if(settingsLoader.status == Loader.Ready){
                            settingsLoader.item.back()
                        }
                    }
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

            Text{
                id: speedAudio
                font.family: bronFontBold.name
                font.pointSize: DisplayCtrl.point10Size
                enabled: root.studio.speed > 0  || root.studio.audio > 0
                visible: enabled
                color: "white"
                text: (root.studio.speed > 0 ? "SPEED " : "") + (root.studio.audio > 0 ? "AUDIO" : "")
            }
            RoundButton{
                id: rbG

                property color focusColor: BronColors.bronBlue
                property bool active: false
                text: "G"
                font.family: bronFontBold.name
                font.pointSize: DisplayCtrl.point25Size
                font.bold: true

                visible: !root.showGroupsRight

                Layout.preferredWidth: btHelp.impHeight//  parent.height/1.8
                Layout.preferredHeight: Layout.preferredWidth
                radius: width/2
                focus: visible
                contentItem: Text {
                    text: rbG.text
                    font: rbG.font
                    width: rbG.width
                    height: rbG.height
                    anchors.centerIn: parent
//                    padding: 0
                    minimumPointSize: Math.max(DisplayCtrl.point5Size,1)
                    // change color in focus
                    color: rbG.active ? "white" : "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    fontSizeMode: Text.Fit
                }

                enabled: groupsGrid.model.count > 0
                opacity: enabled ? 1.0 : 0.5

                // make background not to change in case of focus
                background: Rectangle{
                    anchors.fill: parent
                    color: rbG.active ? "transparent"  : "white"
                    radius: rbG.radius
                    border.color: rbG.activeFocus ? rbG.focusColor : "white"
                }
                onClicked: {
                    if(active){
                        active = false
                    }
                    else{
                        active = true
                    }
                }
            }

            BronToolButton{
                id: btHelp
                font.family: googleMaterial.name
                text: DisplayCtrl.icHelp
                focus: true
                KeyNavigation.tab: btsettings
                KeyNavigation.backtab: btback
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

                Keys.onTabPressed: {
                    event.accepted = true
                    mainEnergyEnergy.forceActiveFocus()
                }
                KeyNavigation.backtab: btHelp
                onClicked:{
                    settingsLoader.setSource("qrc:///qml/Settings.qml",
                                             {"studio" : root.studio,
                                                 "apertureMode": root.apertureMode})
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // popUp selection
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Loader{
        id: popUpLoader

        property string nextSource: ""
        property var nextDevice
        property real nextHeight


        function clearLoader(){
            if(nextSource === ""){
                toolTitle = ""//studio.name
                popUpLoader.source = ""
                focusItem.forceActiveFocus()
            }
            else{
                popUpLoader.setSource(nextSource, {"device": nextDevice, "height": nextHeight})
            }
            nextSource = ""
        }
        onLoaded: {
            y = -popUpLoader.item.height
            popUpLoader.state = "popUpSelected"
        }
        focus: true


        states:[
            State{
                name: "popUpSelected"
                PropertyChanges {
                    target: popUpLoader
                    y: 0
                }
            }
        ]
        transitions: [
            Transition{
                from: ""; to: "popUpSelected"
                NumberAnimation { target: popUpLoader; property: "y"; duration: 400; easing.type: Easing.InOutQuad }
            },
            Transition{
                from: "popUpSelected"; to: ""
                SequentialAnimation{
                    NumberAnimation { target: popUpLoader; property: "y"; duration: 400; easing.type: Easing.InOutQuad }
                    ScriptAction{ script: popUpLoader.clearLoader()}
                }
            }
        ]
    }
    Connections{
        target: popUpLoader.item
        onBack:{
            popUpLoader.state = ""
        }
    }

    Loader{
        id: setterLoader

        property string nextSource: ""
        property var nextGroup
        property var nextCogniCol
        property var nextSelGroup
        property var nextDevice
        property var nextSelDevice

        width: DisplayCtrl.portraitWidth
        height: DisplayCtrl.portraitHeight/3
        y: -height

        // function is called from state transisition
        function clearLoader(){
            if(nextSource === ""){
                toolTitle = ""//studio.name
                setterLoader.source = ""
                focusItem.forceActiveFocus()
            }
            else{
                if(nextSource !== "qrc:///qml/DeviceSelection.qml"){
                    setterLoader.setSource(nextSource, {"group": nextGroup,
                                                        "cogniCol": nextCogniCol,
                                                        "selGroup": nextSelGroup})
                }
                else{
                    setterLoader.setSource(nextSource, {"device": nextDevice,
                                                  "selDevice": nextSelDevice,
                                                  "cogniCol": nextCogniCol})
                }
            }
            nextSource = ""
        }
        onLoaded: {
           setterLoader.state = "setter"
        }
        focus: true

        states:[
            State{
                name: "setter"
                PropertyChanges {
                    target: setterLoader
                    y: 0
                }
            }
        ]
        transitions: [
            Transition{
                from: ""; to: "setter"
                NumberAnimation { target: setterLoader; property: "y"; duration: 400; easing.type: Easing.InOutQuad }
            },
            Transition{
                from: "setter"; to: ""
                SequentialAnimation{
                    NumberAnimation { target: setterLoader; property: "y"; duration: 400; easing.type: Easing.InOutQuad }
                    ScriptAction{ script: setterLoader.clearLoader()}
                }
            }
        ]

    }
    Connections{
        target: setterLoader.item
        onBack:{
            setterLoader.state = ""
        }
    }

    Loader{
        id: settingsLoader
        width: parent.width
        height: parent.height
        x: -width
        function clearLoader(){
            settingsLoader.source = ""
            toolTitle = ""//studio.name
//            focusItem.forceActiveFocus()
        }
        onLoaded:{
            toolTitle = "SETTINGS"
            settingsLoader.state = "settings"
        }
        focus: true
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
    Connections{
        target: settingsLoader.item
        onBack:{
            settingsLoader.state = ""
        }
    }
    Connections{
        target: settingsLoader.item
        enabled:  settingsLoader.state === "settings"
        onApertureModeChanged: {
           root.apertureMode = settingsLoader.item.apertureMode
        }
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // popUp selection end
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    footer: Rectangle{
            width: parent.width
            height: toolbar.height * 2 / 3
            color: "black"

            Image{
                id: conImage
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: DisplayCtrl.dp(20)
                height: parent.height*0.6
                fillMode: Image.PreserveAspectFit
                source: root.studio.name === "Bron-Demo0" ? "qrc:///res/reconnect_icon.png" :
                    root.studio.networkState === StudioProxy.Active ? "qrc:///res/reconnect_icon.png" : "qrc:///res/reconnect_failed_icon.png"
            }
            Text{
                id: connectText
                anchors.left: conImage.right
                anchors.leftMargin: DisplayCtrl.dp(15)
                height: parent.height
                font.family: bronFontBold.name
                font.bold: true
                fontSizeMode: Text.VerticalFit
                minimumPointSize: Math.max(DisplayCtrl.point5Size,1)
                color: "white"
                verticalAlignment: Qt.AlignTop
                font.pointSize: parent.height * 0.8
                text: root.studio.name
                padding: 0
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
            id: helpEnergySetter
            hidden: mainEnergyEnergy
            text: qsTr("use arrow keys (up/down) to increase or decrease studio power in 0.1 F-Stop-Range steps. Use shift arrow keys to change in one F-Stop-Range steps")
            onClicked: help.show = false
        }
        HelpText{
            id: helpSwipeArea
            hidden: btLine
            text: qsTr("further functions for all studio devices. Use swipe to see more")
            onClicked: help.show = false
        }
        HelpText{
            id: helpGroupsGrid
            hidden: groupsGrid
            visible: groupsGrid.visible
            text: qsTr("adjust devices belonging to a group (same channel (compact flashes) or lampAddress (generators))")
            onClicked: help.show = false
        }
        HelpText{
            id: helpFlicker
            hidden: flicker
            text: qsTr("devices area. Setup single devices within the device icon")
            onClicked: help.show = false
        }
    }
}
