import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import "."


FocusScope{
    id: preRoot
    property bool isActive: true
    property alias min: powerBar.min         // start of the power bar
    property alias max: powerBar.max        // end of the power bar
    property alias currentMin: powerBar.currentMin  //absolute lower limit for linear range
    property alias currentMax: powerBar.currentMax //absolute higher limit for linear range
    property alias currentRangeMin : powerBar.currentRangeMin // low limit of movable range
    property alias currentRangeMax : powerBar.currentRangeMax // high limit of movable range
    property color adjustButtonFocusColor: BronColors.bronBlue
    signal incPower(double increment)
    signal decPower(double decrement)
    property alias setterHeight: root.setterHeight
    property alias color: root.color
    signal setFocus()
    signal setBackFocus()

    Rectangle {
        id: root

        property real setterHeight: preRoot.isActive ? preRoot.height * 2 / 3 : preRoot.height
        width: preRoot.width

        color: BronColors.bronBlue
        focus: false

        RowLayout{
            id: line
            height: setterHeight
            width: parent.width
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            focus: false

            BronToolButton{
                id: smallerEnergy
                function doAction(val){
                    if(!powerBar.isNegLimit){
                        if(powerBar.currentRangeMin > powerBar.currentMin){
                            preRoot.decPower(val)
                        }
                    }
                }

                enabled: !powerBar.isNegLimit
                hide: powerBar.isNegLimit

                focusColor: adjustButtonFocusColor

                focus: true
                Keys.onTabPressed:{
                    event.accepted = true
                    setFocus()
                }
                KeyNavigation.backtab: higherEnergy
                Keys.onPressed:{
                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Down){
                        if(event.modifiers & Qt.ShiftModifier){
                            doAction(1.0)
                        }
                        else{
                            doAction(0.1)
                        }
                        event.accepted = true
                    }
                    else if(event.key === Qt.Key_Up){
                        if(event.modifiers & Qt.ShiftModifier){
                            higherEnergy.doAction(1.0)
                        }
                        else{
                            higherEnergy.doAction(0.1)
                        }
                        event.accepted = true
                    }
                }
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillHeight: true

                font.family: googleMaterial.name
                font.pointSize: DisplayCtrl.point80Size

                text: DisplayCtrl.icKeyboardArrorLeft

                onClicked: doAction(0.1)
            }
            Text{
                id: mainEnergyText

                focus: false

                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true

                font.family: bronFontBold.name
                font.pointSize: DisplayCtrl.point80Size
                font.bold: true
                minimumPointSize: 10
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideMiddle

                text: powerBar.currentRangeMax.toFixed(1)
                color: "white"

                MouseArea{
                    id: swipeEmu
                    anchors.fill: parent

                    property int startPosition : 0
                    function swipe(way, x){
                        if(way >= width){
                            higherEnergy.doAction(1.0)
                            startPosition = x
                        }
                        else if(-way > width){
                            smallerEnergy.doAction(1.0)
                            startPosition = x
                        }
                    }

                    onPositionChanged:{
                        swipe((mouse.x - startPosition), mouse.x)
                    }
                    onPressed:{
                        startPosition = mouse.x
                    }
                    onReleased: startPosition = 0
                }
            }
            BronToolButton{
                id: higherEnergy
                function doAction(val){
                    if(!powerBar.isPosLimit){
                        if(powerBar.currentRangeMax < powerBar.currentMax){
                            preRoot.incPower(val)
                        }
                    }
                }

                enabled: !powerBar.isPosLimit
                hide: powerBar.isPosLimit
                focusColor: adjustButtonFocusColor

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.fillHeight: true

                focus: true

                KeyNavigation.tab: smallerEnergy
                Keys.onBacktabPressed:{
                    event.accepted = true
                    setBackFocus()
                }
                Keys.onPressed:{
                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Up){
                        if(event.modifiers & Qt.ShiftModifier){
                            doAction(1.0)
                        }
                        else{
                            doAction(0.1)
                        }
                        event.accepted = true
                    }
                    else if(event.key === Qt.Key_Down){
                        if(event.modifiers & Qt.ShiftModifier){
                            smallerEnergy.doAction(1.0)
                        }
                        else{
                            smallerEnergy.doAction(0.1)
                        }
                        event.accepted = true
                    }
                }

                font.family: googleMaterial.name
                font.pointSize: DisplayCtrl.point80Size

                text: DisplayCtrl.icKeyboardArrowRight
                onClicked: doAction(0.1)
            }
        }
        PowerBar{
            id: powerBar
            width: preRoot.width//parent.width
            height: preRoot.height/3.5//parent.height/3.5
            anchors.top: line.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            visible: preRoot.isActive
            enabled: visible
        }
    }
    Rectangle{
        anchors.fill: root
        color: "yellow"
        opacity: 0.5
    }
}
