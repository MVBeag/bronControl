import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import "."


Rectangle{
    id: sequFrame

    property real visualPosition: 0.0
    property bool isActive: false
    property string unit: ""
    property real lowerLimit: 0.0
    property real higherLimit: 10.0
    property int comma: 0
    property real stepSize: 1.0
    property alias value: slider.value

    function reset(){
        btSequOnOff.isActive = false
        slider.value = 0
    }


    Component.onCompleted: {
        sequFrame.visualPosition = Qt.binding(function() {return slider.visualPosition})
        sequFrame.isActive = Qt.binding(function() {return btSequOnOff.isActive})
    }

    color: "black"//btSequOnOff.isActive ? BronColors.bronBlue : "black"
    border.color: BronColors.bronBlue
    border.width: DisplayCtrl.dp(2)
    Rectangle{
        id: btBackground
        width: btSequOnOff.width + DisplayCtrl.dp(2)
        height: btSequOnOff.height + DisplayCtrl.dp(2)
        color: "black"
    }
    Button{
        id: btSequOnOff
        width: parent.width/5
        height: parent.height/3
        
        anchors.top: parent.top
        anchors.left: parent.left
        
        text: isActive ? "ON" : "OFF"

        onClicked: {
            if(isActive){
                slider.value = 2.0
            }
            else{
                slider.value = 0.0
            }
        }
    }
    
    RowLayout{
        id: laySlider
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: DisplayCtrl.dp(10)
        width: parent.width
        height: parent.height - btBackground.height - anchors.bottomMargin

        BronToolButton{
            id: decrease

            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter | Qt.AlignHCenter
            Layout.leftMargin: DisplayCtrl.dp(20)
            Layout.fillHeight: true
            Layout.fillWidth: true
            font.family: googleMaterial.name
            font.pointSize: DisplayCtrl.point25Size

            text: DisplayCtrl.icKeyboardArrorLeft

            onClicked:{
                slider.decrease()
                if(slider.value < 0.05){
                    btSequOnOff.isActive = false
                }
            }
        }

        Text{
            id: lowerText
            Layout.alignment: Qt.AlignLeading | Qt.AlignVCenter | Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point15Size
            text: sequFrame.unit !== "" ?  sequFrame.lowerLimit.toFixed(sequFrame.comma).toString() + " " + sequFrame.unit.toString()
                                        :  sequFrame.lowerLimit.toFixed(sequFrame.comma).toString()
            color: "white"
        }
        Slider{
            id: slider
            Layout.alignment:  Qt.AlignVCenter | Qt.AlignHCenter
            Layout.preferredWidth: parent.width * 0.7
            from: sequFrame.lowerLimit.toFixed(sequFrame.comma)
            to: sequFrame.higherLimit.toFixed(sequFrame.comma)
            stepSize: sequFrame.stepSize
            snapMode: Slider.SnapOnRelease
            onValueChanged: {
                if(slider.value > 0){
                    btSequOnOff.isActive = true
                }
                else{
                    btSequOnOff.isActive = false
                }
            }
        ///////////////////////////////////////////////////////////////////////
            background: Rectangle {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitHeight: DisplayCtrl.dp(40)
                width: slider.availableWidth
                height: implicitHeight
                radius: 2
                color: BronColors.bronBlack

                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    color: BronColors.bronBlue
                    radius: 2
                }
            }

            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: DisplayCtrl.dp(40)
                implicitHeight: implicitWidth
                radius: implicitWidth / 2
                color: slider.pressed ? "#f0f0f0" : "#f6f6f6"
                border.color: "#bdbebf"
            }
        /////////////////////////////////////////////////////////////////
        }
        Text{
            id: highText
            Layout.alignment: Qt.AlignTrailing | Qt.AlignVCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: bronFontBold.name
            font.pointSize: DisplayCtrl.point15Size
            text: sequFrame.unit !== "" ? sequFrame.higherLimit.toFixed(sequFrame.comma).toString() + " " + sequFrame.unit.toString()
                                         : sequFrame.higherLimit.toFixed(sequFrame.comma).toString()
            color: "white"
        }
        
        BronToolButton{
            id: increase

            Layout.alignment: Qt.AlignTrailing | Qt.AlignVCenter | Qt.AlignHCenter
            Layout.rightMargin: DisplayCtrl.dp(20)
            Layout.fillHeight: true
            Layout.fillWidth: true

            font.family: googleMaterial.name
            font.pointSize: DisplayCtrl.point25Size

            text: DisplayCtrl.icKeyboardArrowRight

            onClicked:{
                slider.increase()
                if(slider.value > 0){
                    btSequOnOff.isActive = true
                }
            }
        }

    }
}
