import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import ch.bron.BronModels 1.0
import QtQml.Models 2.2
import "."


Page {
    id: root
    property var studio
    property Item focusItem: Item{}

    function checkCount(){
        if(studio.devices.count > 0){
            pageTimer.stop()
            waitTimer.restart()
        }
    }

    background: Rectangle{
        id:bg
        color: BronColors.bronBlack
    }

    Timer{
        id: pageTimer
        interval: 20000
        running: false
        onTriggered: {
            root.studio.online = false
            stack.pop()
        }
    }
    Timer{
        id: waitTimer
        interval: 5500
        running: false
        onTriggered:{
            stack.replace("qrc:///qml/StudioScene.qml", {"studio" : root.studio})
        }
    }



    ColumnLayout{
        anchors.fill: parent
        spacing: DisplayCtrl.gridMargin

        Item{
            Layout.preferredHeight: parent.height/4
        }

        Text{
            id: selectedGroup
            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point20Size
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            color: "white"
            text: root.studio.name
        }
        Image{
            id: image
            source: "qrc:///res/studio_loading_image.png"
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: DisplayCtrl.dp(400)
            Layout.preferredHeight: DisplayCtrl.dp(400)
            fillMode: Image.PreserveAspectFit
            mipmap: true
            Text{
                id: scannText

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: DisplayCtrl.dp(10)

                font.family: bronFontBold.name
                font.pointSize: DisplayCtrl.point20Size
                font.bold: true
                color: "white"
                text: "scanning..."
            }
            BronBusyIndicator{
                id: busy
                running: true
                anchors.centerIn: image
            }

        }

        Repeater{
            model: root.studio.devices
            Item{
                property var count: model.count
                onCountChanged: {
                    var rowCount = root.studio.devices.count
                    for( var i = 0;i < rowCount;i++ ) {
                        var entry = repFilter.items.get(i)
                        if(entry.model.type !== 2) {
                            repFilter.items.get(i).inShow = true
                        }
                    }
                    repFilter.handleTmr()
                }
            }
        }



        DelegateModel{
            id: repFilter

            property int ix: 0

            function handleTmr(){
                pageTimer.stop()
                waitTimer.restart()
            }

            Component.onCompleted: {
                var rowCount = model.count
                for( var i = 0;i < rowCount;i++ ) {
                    var entry = model.get(i)
                    if(entry.type !== 2) {
                        items.get(i).inShow = true
                    }
                }
            }

            groups:[
                DelegateModelGroup{
                    includeByDefault: false
                    name: "show"
                }
            ]

            filterOnGroup: "show"

            model: root.studio.devices
            delegate: Rectangle{
                color: "transparent"
                width: DisplayCtrl.gridMargin * 1.5
                height: width
                Rectangle{
                    anchors.centerIn: parent
                    width: DisplayCtrl.gridMargin
                    height: width
                    color: model.type === 1 ? "grey" : "white"
                }
            }
        }

        Row{
            Layout.alignment: Qt.AlignHCenter
            Repeater {
                id: countId
                model: repFilter
            }
        }
        Item{
            Layout.fillHeight: true
        }
    }


    header: ToolBar{
        id: toolbar
        implicitWidth: parent.width
        implicitHeight: DisplayCtrl.dp(70)
        background: Rectangle {
            anchors.fill: parent
            color: BronColors.bronBlack
        }
        RowLayout {
            anchors.fill: parent
            BronToolButton{
                id: btback
                Layout.margins: DisplayCtrl.dp(10)

                font.family: googleMaterial.name
                text: DisplayCtrl.icBack

                onClicked: {
                    root.studio.online = false
                    stack.pop()
                }
            }
            Item{id: fillRow}
        }
    }


    footer: Rectangle{
            width: parent.width
            height: toolbar.height *2 / 3
            color: "black"

            Image{
                id: conImage
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: DisplayCtrl.dp(20)
                height: parent.height * 0.6
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
                fontSizeMode: Text.Fit
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
                height: parent.height * 0.6
                fillMode: Image.PreserveAspectFit
                source:"qrc:///res/bron_logo.png"
                mipmap: true
            }
    }

    Component.onCompleted: {
        pageTimer.start()
        root.checkCount()
    }

    Component.onDestruction: {
        pageTimer.stop()
        waitTimer.stop()
    }

}
