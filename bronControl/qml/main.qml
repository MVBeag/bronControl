import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import ch.bron.BronModels 1.0
import "."

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: DisplayCtrl.portraitWidth
    height: DisplayCtrl.portraitHeight
    minimumWidth: DisplayCtrl.portraitWidth
//    maximumWidth: DisplayCtrl.portraitWidth
//    minimumHeight: Screen.desktopAvailableHeight/3

    property bool show: studioMgr.show
    property bool showBackside: false

    function changeApplicationSize(flipped){
        if(flipped === true){
            showBackside = true;
        }
        else{
            showBackside = false;
        }
    }


    color: "black"
    title: qsTr("bronControl")

    // used fonts
    FontLoader {id: bronFont; source: "qrc:///fonts/DINOT-Regular.otf"}
    FontLoader {id: bronFontLight; source: "qrc:///fonts/DINOT-Light.otf"}
    FontLoader {id: bronFontMedium; source: "qrc:///fonts/DINOT-Medium.otf"}
    FontLoader {id: bronFontBold; source: "qrc:///fonts/DINOT-Bold.otf"}
    FontLoader {id: bronFontBlack; source: "qrc:///fonts/DINOT-Black.otf"}
    FontLoader {id: googleMaterial; source: "qrc:///fonts/MaterialIcons-Regular.ttf"}

    Flipable{
        id: flipable
        width: parent.width
        height: parent.height

        property bool flipped: false

        front: StackView {
            id: stack
            z: 1        // make it visible
            focus: true // give childs focus
            initialItem: Splash{
                stdMgr: studioMgr
            }
            anchors.fill: parent    // same size a ApplicationWindow
            onCurrentItemChanged: {
                if(currentItem){
                    currentItem.focusItem.forceActiveFocus()
                }
            }
        }

        Connections{
            target: stack.currentItem
            ignoreUnknownSignals: true
            onSwitchStudioMgrView:{
                if(frontside === false){
                    flipable.flipped = true
                }
                else{
                    flipable.flipped = false
                }
            }
        }

//        back: StudioManager{
        back: Item{
            signal back()
            anchors.fill: parent
            onBack: flipable.flipped = false
        }

        transform: Rotation {
            id: rotation
            origin.x: flipable.width/2
            origin.y: flipable.height/2
            axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
            angle: 0    // the default angle
        }

        states: State {
            name: "back"
            PropertyChanges { target: rotation; angle: 180 }
            when: flipable.flipped
        }

        transitions: Transition {
            SequentialAnimation{
                SmoothedAnimation { target: rotation; property: "angle"; duration: 400 }
                ScriptAction{ script: changeApplicationSize(flipable.flipped)}
            }
        }
    }


    Item{
        id: widthController

        states: State {
            name: "mgrActive"
            PropertyChanges {
                target: applicationWindow
                width: Screen.desktopAvailableWidth/2
                height: Screen.desktopAvailableHeight*7/8
                minimumWidth: 0
                maximumWidth: 200000
            }
            when: applicationWindow.showBackside
        }

//        transitions: Transition {
//            NumberAnimation { target: applicationWindow; property: "width"; duration: 200 }
//            NumberAnimation { target: applicationWindow; property: "height"; duration: 200 }
//        }
    }



}

