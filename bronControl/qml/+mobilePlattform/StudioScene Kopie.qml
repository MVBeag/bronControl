import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
//import QtQuick.Controls.Styles 1.4
//import QtQml 2.2
import QtQml.Models 2.2
import ch.bron.BronModels 1.0
import "."

Page {
    id: root

    property var studio
    property Item focusItem: mainEnergyEnergy
    property int apertureMode: Selectables.fStopRange
    property string toolTitle: studio.name

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
        console.log("studioScene completed")
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
                    height: parent.height - swipeArea.height - pageInd.height - DisplayCtrl.gridMargin
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

                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(60)

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

                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(60)

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

                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(60)

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

                    ComboBox{
                        id: cbMore
                        signal selected()

                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(60)

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

                        property bool isOpen: false

                        focus: true

                        function setReset(doDefault){
                            var settings = {}
                            settings.isReset = doDefault ? false : true
                            //                        settings.isDefault = doDefault
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

                        displayText: "MORE"
                        Keys.onPressed: {
                            if(event.key === Qt.Key_Return){
                                if(false === isOpen){
                                    doSelection(currentIndex, currentText)
                                    event.accepted = true
                                }
                            }
                        }

                        font.family: bronFontMedium.name
                        font.bold: true
                        font.pointSize: DisplayCtrl.point15Size
                        padding: DisplayCtrl.dp(10)
                        hoverEnabled: true
                        // only enabled if more than one element (whitch can take part) is active
                        enabled: geniGrid.model.count + devicesGrid.model.count > 1 ? true : false
                        model: comboSel

                        delegate: BronItemDelegate {
                            id: popupDelegate
                            width: cbMore.width
                            text: modelData
                            font: cbMore.font
                            highlighted: cbMore.highlightedIndex == index
                            hoverEnabled: cbMore.hoverEnabled
                        }

                        indicator: Item{}

                        contentItem: Text {
                            leftPadding: cbMore.mirrored && cbMore.indicator ? cbMore.indicator.width + cbMore.spacing : 0
                            rightPadding: !cbMore.mirrored && cbMore.indicator ? cbMore.indicator.width + cbMore.spacing : 0

                            text: cbMore.displayText
                            font: cbMore.font
                            color: "white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                            opacity: enabled ? 1 : 0.3
                            scale: (contentHeight + 2 * padding) > height ? (height / (contentHeight + 2 * padding)) : 1
                        }

                        background: Rectangle {
                            color: "black"
                            border.color: cbMore.activeFocus ? "white" : BronColors.bronBlue
                            border.width: DisplayCtrl.dp(4)
                        }

                        popup: Popup {
                            id: cbMorePopup
                            y: cbMore.height - (cbMore.visualFocus ? 0 : 1)
                            width: cbMore.width
                            implicitHeight: listview.contentHeight
                            topMargin: 6
                            bottomMargin: 6
                            padding: 1

                            onOpened: cbMore.isOpen = true
                            onClosed: cbMore.isOpen = false

                            contentItem: ListView {
                                id: listview
                                clip: true
                                implicitHeight: contentHeight
                                model: cbMore.popup.visible ? cbMore.delegateModel : null
                                currentIndex: cbMore.highlightedIndex
                                highlightRangeMode: ListView.ApplyRange
                                highlightMoveDuration: 0

                                ScrollIndicator.vertical: ScrollIndicator { }
                            }

                            background: Rectangle {
                                color: "black"
                                border.width: DisplayCtrl.dp(4)
                                border.color: cbMore.activeFocus ? "white" : BronColors.bronBlue
                            }
                        }

                        onHighlighted: {
                            forceActiveFocus()
                            currentIndex = index
                        }

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

                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(60)

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

                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(60)

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

                        width: DisplayCtrl.itemSize
                        height: DisplayCtrl.dp(60)

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
                }
                ListModel{
                    id: btListModel1
                    ListElement{
                        name: "speedButton"
                    }
                    ListElement{
                        name: "audioButton"
                    }
                    ListElement{
                        name: "congiButton"
                    }
                }

                RowLayout{
                    id: swipeArea
                    anchors.top: mainEnergyEnergy.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width * 0.05
                    width: parent.width * 0.9
                    height: DisplayCtrl.dp(60)
                    clip: true

                    SwipeView{
                        id: btLine
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        currentIndex: 0
                        interactive: true
                        focus: true

                        ListView{
                            id: btLineL1
                            width: DisplayCtrl.itemSize * 4 + DisplayCtrl.gridMargin * 3
                            height: DisplayCtrl.dp(60) // was dp(90) with horizontal scroll bar
                            interactive: false
                            clip: true
                            focus: true

                            orientation: ListView.Horizontal
                            spacing: DisplayCtrl.gridMargin

                            model: btListModel
                            delegate: FocusScope{
                                id: delegateItem
                                width: DisplayCtrl.itemSize
                                height: DisplayCtrl.dp(60)
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
                                        btLineL1.currentIndex = index
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
                                }
                            }
                        }
                        ListView{
                            id: btLineL2
                            width: DisplayCtrl.itemSize * 4 + DisplayCtrl.gridMargin * 3
                            height: DisplayCtrl.dp(60) // was dp(90) with horizontal scroll bar
                            interactive: false
                            clip: true
                            focus: true

                            orientation: ListView.Horizontal
                            spacing: DisplayCtrl.gridMargin

                            model: btListModel1
                            delegate: FocusScope{
                                id: delegateItem1
                                width: DisplayCtrl.itemSize
                                height: DisplayCtrl.dp(60)
                                focus: true
                                property string name: model.name
                                Loader{
                                    id: componentLoader1
                                    focus: true
                                    anchors.fill: parent
                                }
                                Connections{
                                    target: componentLoader1.item
                                    onSelected: {
                                        btLineL2.currentIndex = index
                                    }
                                }

                                Component.onCompleted: {
                                    if(name === "speedButton"){
                                        componentLoader1.sourceComponent = speedBtComponent
                                    }
                                    else if(name === "audioButton"){
                                        componentLoader1.sourceComponent = audioBtComponent
                                    }
                                    else if(name === "congiButton"){
                                        componentLoader1.sourceComponent = cogniBtComponent
                                    }
                                }
                            }
                        }
                    }
                }

                PageIndicator{
                    id: pageInd
                    anchors.top: swipeArea.bottom
                    anchors.topMargin: DisplayCtrl.gridMargin/2
                    anchors.horizontalCenter: parent.horizontalCenter
                    count: btLine.count
                    currentIndex: btLine.currentIndex
                    interactive: true
                    onCurrentIndexChanged: btLine.currentIndex = currentIndex
                    delegate: Rectangle {
                        implicitWidth: DisplayCtrl.dp(20)
                        implicitHeight: implicitWidth

                        radius: width / 2
                        color: BronColors.bronBlue

                        opacity: index === pageInd.currentIndex ? 1.0 : 0.45

                        Behavior on opacity {
                            OpacityAnimator {
                                duration: 100
                            }
                        }
                    }
                }

                Rectangle{
                    id: mainSettingBottenLine
                    anchors.top: pageInd.bottom
                    anchors.topMargin: DisplayCtrl.gridMargin/2
                    width: parent.width
                    height: DisplayCtrl.dp(2)
                    color: "white"//
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
                        border.color: delitem.activeFocus ? "white" : BronColors.bronBlue
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
                        onClicked:{
                            groupsGrid.currentIndex = index
                            focusItem = delitem
                            toolTitle = "ADJUST GROUP"
                            if(setterLoader.status === Loader.Ready){
                                setterLoader.item.back()
                                setterLoader.nextSource = "qrc:///qml/GroupSelection.qml"
                                setterLoader.nextGroup = model
                                setterLoader.nextCogniCol = groupsGrid.currentItem.cogniCol
                                setterLoader.nextSelGroup = groupsGrid.currentItem.text
                            }
                            else{
                                setterLoader.setSource("qrc:///qml/GroupSelection.qml",
                                                       {"group": model,
                                                           "cogniCol": groupsGrid.currentItem.cogniCol,
                                                           "selGroup": groupsGrid.currentItem.text})
                            }
                        }
                        onPressAndHold: {
                            groupsGrid.currentIndex = index
                            focusItem = delitem
                            model.standby = model.standby == 0 ? 1 :0
                        }
                    }
                    Keys.onReturnPressed:{
                        groupsGrid.currentIndex = index
                        focusItem = delitem
                        event.accepted = true
                        toolTitle = "ADJUST GROUP"
                        if(setterLoader.status === Loader.Ready){
                            setterLoader.item.back()
                            setterLoader.nextSource = "qrc:///qml/GroupSelection.qml"
                            setterLoader.nextGroup = model
                            setterLoader.nextCogniCol = groupsGrid.currentItem.cogniCol
                            setterLoader.nextSelGroup = groupsGrid.currentItem.text
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

                ScrollBar.horizontal: ScrollBar {
                    id: gGridSB

                    contentItem: Rectangle {
                        id: handleGGridSB

                        implicitWidth: DisplayCtrl.dp(5)
                        implicitHeight: DisplayCtrl.dp(5)

                        radius: width / 2
                        color: gGridSB.pressed ? BronColors.bronBlue : "white"
                        visible: gGridSB.size < 1.0
                        opacity: 0.0

                        states: State {
                            name: "active"
                            when: groupsGrid.isIn || gGridSB.pressed
                        }

                        transitions: [
                            Transition {
                                to: "active"
                                SequentialAnimation {
                                    NumberAnimation { target: handleGGridSB; duration: 200; property: "opacity"; to: 1.0 }
                                }
                            },
                            Transition {
                                from: "active"
                                SequentialAnimation {
                                    PauseAnimation { duration: 450 }
                                    NumberAnimation { target: handleGGridSB; duration: 200; property: "opacity"; to: 0.0 }
                                }
                            }
                        ]
                    }
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

            ScrollBar.vertical: ScrollBar {
                id: devicesSB

                contentItem: Rectangle {
                    id: handleDevicesSB

                    implicitWidth: DisplayCtrl.dp(5)
                    implicitHeight: DisplayCtrl.dp(5)

                    radius: width / 2
                    color: devicesSB.pressed ? BronColors.bronBlue : "white"
                    visible: devicesSB.size < 1.0
                    opacity: 0.0

                    states: State {
                        name: "active"
                        when: flicker.isIn || devicesSB.pressed
                    }

                    transitions: [
                        Transition {
                            to: "active"
                            SequentialAnimation {
                                NumberAnimation { target: handleDevicesSB; duration: 200; property: "opacity"; to: 1.0 }
                            }
                        },
                        Transition {
                            from: "active"
                            SequentialAnimation {
                                PauseAnimation { duration: 450 }
                                NumberAnimation { target: handleDevicesSB; duration: 200; property: "opacity"; to: 0.0 }
                            }
                        }
                    ]
                }
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
//                        width: geniGrid.cellWidth
//                        height: geniGrid.cellHeight
//                        transform: DisplayCtrl.userZoom
                        width: aGeni.innerWidth
                        Component.onCompleted: console.log("gd " + width + " p " + parent.width)

                        property var geni: model

                        Generator{
                            id: aGeni
                            focus: true
                            generator: geni
                            Component.onCompleted: console.log("gc " + innerWidth + " p " + parent.width)

                            onFocusItemChanged: root.focusItem = focusItem
                            apertureMode: root.apertureMode

                            onLongClicked: {
                                geniGrid.currentIndex = index
                                focusItem = parent
                                geni.test = 1
                            }
                            onMoreLongClicked: {
                                geniGrid.currentIndex = index
                                focusItem = parent
                                geni.standby = !geni.standby
                            }


                            onMoreClickedLamp: {
                                geniGrid.currentIndex = index
                                focusItem = parent
                                toolTitle = "SETTINGS"
                                if(popUpLoader.status === Loader.Ready){
                                    popUpLoader.item.back()
                                    popUpLoader.nextSource = "qrc:///qml/GenLampMore.qml"
                                    popUpLoader.nextDevice = lamp.device
                                    popUpLoader.nextHeight = root.viewHeight
                                }
                                else{
                                    popUpLoader.setSource("qrc:///qml/GenLampMore.qml", {"device": lamp.device, "height" : root.viewHeight})
                                }
                            }//(var lamp)
                            onLampClickedLamp: {
                                geniGrid.currentIndex = index
                                focusItem = parent
                                toolTitle = "ADJUST UNIT"
                                if(setterLoader.status === Loader.Ready){
                                    setterLoader.item.back()
                                    setterLoader.nextSource = "qrc:///qml/DeviceSelection.qml"
                                    setterLoader.nextDevice = lamp.device
                                    setterLoader.nextSelDevice = "Lamp " + lamp.channel
                                    setterLoader.nextCogniCol = lamp.cogniColor
                                }
                                else{
                                    setterLoader.setSource("qrc:///qml/DeviceSelection.qml",
                                                           {"device": lamp.device,
                                                               "selDevice": "Lamp " + lamp.channel,
                                                               "cogniCol": lamp.cogniColor})
                                }
                            }//(var lamp)
                            onMoreClicked: {
                                geniGrid.currentIndex = index
                                focusItem = parent
                                toolTitle = "SETTINGS"
                                var name = geniDlg.geni.fullModelName
                                if(name === "Scoro 1600 E" || name === "Scoro 3200 E"){
                                    if(popUpLoader.status === Loader.Ready){
                                        popUpLoader.item.back()
                                        popUpLoader.nextSource = "qrc:///qml/ScoroEMore.qml"
                                        popUpLoader.nextDevice = geniDlg.geni
                                        popUpLoader.nextHeight = root.viewHeight
                                    }
                                    else{
                                        popUpLoader.setSource("qrc:///qml/ScoroEMore.qml", {"device": geniDlg.geni, "height" : root.viewHeight})
                                    }
                                }
                                else if(name === "Scoro 3200 S" || name === "Scoro 1600 S"){
                                    if(popUpLoader.status === Loader.Ready){
                                        popUpLoader.item.back()
                                        popUpLoader.nextSource = "qrc:///qml/ScoroSMore.qml"
                                        popUpLoader.nextDevice = geniDlg.geni
                                        popUpLoader.nextHeight = root.viewHeight
                                    }
                                    else{
                                        popUpLoader.setSource("qrc:///qml/ScoroSMore.qml", {"device": geniDlg.geni, "height" : root.viewHeight})
                                    }
                                }
                            }
                            onGeneratorClicked: {
                                geniGrid.currentIndex = index
                                focusItem = parent
                                toolTitle = "ADJUST UNIT"
                                if(setterLoader.status === Loader.Ready){
                                    setterLoader.item.back()
                                    setterLoader.nextSource = "qrc:///qml/DeviceSelection.qml"
                                    setterLoader.nextDevice = geniDlg.geni
                                    setterLoader.nextSelDevice = "GENERATOR " + (index + 1)
                                    setterLoader.nextCogniCol = "transparent"
                                }
                                else{
                                    setterLoader.setSource("qrc:///qml/DeviceSelection.qml",
                                                           {"device": geniDlg.geni,
                                                               "selDevice": "GENERATOR " + (index + 1),
                                                               "cogniCol": "transparent"})
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
                    cellWidth: cellHeight
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
                                devicesGrid.currentIndex = index
                                focusItem = parent
                                model.test = 1
                            }
                            onMoreLongClicked: {
                                devicesGrid.currentIndex = index
                                focusItem = parent
                                model.standby = !model.standby
                            }

                            onMoreClicked: {
                                devicesGrid.currentIndex = index
                                focusItem = parent
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
                            onLampClicked: {
                                devicesGrid.currentIndex = index
                                focusItem = parent
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

                pinch.minimumScale: 0.5
                pinch.maximumScale: 4
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

    header: ToolBar{
        id: toolbar
//        implicitWidth: parent.width
        implicitHeight: btback.height//DisplayCtrl.dp(70)
        background: Rectangle {
            anchors.fill: parent
            color: BronColors.bronBlack
        }

        BronToolButton{
            id: btback

            focus: true
            KeyNavigation.tab: btHelp
            KeyNavigation.backtab: devicesGrid

            anchors.left: parent.left
            anchors.leftMargin: DisplayCtrl.gridMargin

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
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point20Size
            font.bold: true
            color: "white"
            text: root.toolTitle
        }

        Text{
            id: speedAudio

            anchors.right: rbG.left
            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point10Size
            enabled: root.studio.speed > 0  || root.studio.audio > 0
            visible: enabled
            color: "white"
            text: (root.studio.speed > 0 ? "SPEED " : "") + (root.studio.audio > 0 ? "AUDIO" : "")
            Component.onCompleted: {
                speedAudio.enabled = Qt.binding(function(){return root.studio.speed > 0  || root.studio.audio > 0})
                speedAudio.text = Qt.binding(function() {return (root.studio.speed > 0 ? "SPEED " : "") + (root.studio.audio > 0 ? "AUDIO" : "")})
            }
        }

        RoundButton{
            id: rbG

            property color focusColor: BronColors.bronBlue
            property bool active: false

            anchors.right: btHelp.left
            anchors.rightMargin: DisplayCtrl.dp(15)
            anchors.verticalCenter: btHelp.verticalCenter
            text: "G"
            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point25Size
            font.bold: true

            visible: !root.showGroupsRight

            width: parent.height/1.8
            height: width
            radius: width/2
            focus: visible
            contentItem: Text {
                text: rbG.text
                font: rbG.font
                width: rbG.width
                height: rbG.height
                anchors.centerIn: parent
                padding: 0
                minimumPointSize: DisplayCtrl.point15Size
                // change color in focus
                color: rbG.active ? "white" : "black"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle
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

            anchors.right: btsettings.left

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

            anchors.right: parent.right
            anchors.rightMargin: DisplayCtrl.gridMargin

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
                toolTitle = studio.name
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
                toolTitle = studio.name
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
            toolTitle = studio.name
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
                    root.studio.online ? "qrc:///res/reconnect_icon.png" : "qrc:///res/reconnect_failed_icon.png"
            }
            Text{
                id: connectText
                anchors.left: conImage.right
                anchors.leftMargin: DisplayCtrl.dp(15)
                height: parent.height
                font.family: bronFontBold.name
                font.bold: true
                fontSizeMode: Text.VerticalFit
                minimumPointSize: DisplayCtrl.dp(5)
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
            hidden: swipeArea
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
