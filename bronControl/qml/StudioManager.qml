import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import "."

Page {
    id: root
    signal back()
    anchors.fill: parent


    header: ToolBar{

    }

    SplitView{
        anchors.fill: parent
        orientation: Qt.Horizontal
        resizing: true

        handleDelegate: Rectangle{
            width: DisplayCtrl.dp(10)
            color: "grey"
            radius: DisplayCtrl.dp(5)
        }


        Rectangle {
            width: parent.width/4
            Layout.maximumWidth: parent.width
            color: "lightblue"
            TreeView{
                anchors.fill: parent
                TableViewColumn {
                          title: "Networks"
                          role: "display"
                          width: parent.width
                          resizable: true
                }
                model: networks
            }

        }
        Rectangle {
            id: centerItem
            Layout.minimumWidth: 0
            Layout.fillWidth: true
            color: "lightgray"
            Text {
                text: "View 2"
                anchors.centerIn: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: root.back()
            }

        }
//        Loader{
//            width: parent.width*2/3
//            height: parent.height

//        }
    }
}
