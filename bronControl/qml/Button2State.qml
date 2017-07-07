import QtQuick 2.6
import "."


Item {
    id: root

    width:  DisplayCtrl.itemSize
    height: DisplayCtrl.dp(60)

    property alias textleft: labelleft.text
    property alias textright: labelright.text
    signal clicked(var anObject)

    property bool isLeftActive: true
    property color rightColor: BronColors.bronBlue
    property color leftColor: BronColors.bronBlue


    Keys.onReturnPressed: {
        if(isLeftActive == true){
            leftbackground.color = "black"
            labelleft.color = "white"
            rightbackground.color = rightColor
            labelright.color = "black"
            isLeftActive = false;
        }
        else{
            leftbackground.color = leftColor
            labelleft.color = "black"
            rightbackground.color = "black"
            labelright.color = "white"
            isLeftActive = true;
        }
        root.clicked(root)
    }



    onIsLeftActiveChanged: {
        if(isLeftActive == true){
            leftbackground.color = leftColor//BronColors.bronBlue
            labelleft.color = "black"
            rightbackground.color = "black"
            labelright.color = "white"
        }
        else{
            leftbackground.color = "black"
            labelleft.color = "white"
            rightbackground.color = rightColor//BronColors.bronBlue
            labelright.color = "black"
            isLeftActive = false;
        }

    }

    Rectangle {
            id: leftbackground
            width: parent.width / 2
            height: parent.height
            anchors.left: parent.left
            color: BronColors.bronBlue
            border.color: DisplayCtrl.isMobilePlattform ? BronColors.bronBlue : parent.isLeftActive ? "white" : BronColors.bronBlue
            border.width: DisplayCtrl.dp(2)

            MouseArea {
                id: mouseArealeft
                anchors.fill: parent
                onClicked: {
                    if(isLeftActive == false){
                        leftbackground.color = leftColor
                        labelleft.color = "black"
                        rightbackground.color = "black"
                        labelright.color = "white"
                        isLeftActive = true;
                    }
                    root.clicked(root)
                }
            }
            Text {
                id: labelleft
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: bronFontMedium.name
                font.pixelSize: DisplayCtrl.sp(height / 2)
                color: "black"
                text: "PROP"
            }

        }


    Rectangle {
            id: rightbackground
            width: parent.width / 2
            height: parent.height
            anchors.left: leftbackground.right
            color: "black"
            border.color: parent.isLeftActive ? BronColors.bronBlue : "white"
            border.width: DisplayCtrl.dp(2)


            Text {
                id: labelright
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: bronFontMedium.name
                font.pixelSize: DisplayCtrl.sp(height / 2)
                color: "white"
                text: "FULL"
            }


            MouseArea {
                id: mouseArearight
                anchors.fill: parent
                onClicked: {
                    if(isLeftActive == true){
                        leftbackground.color = "black"
                        labelleft.color = "white"
                        rightbackground.color = rightColor
                        labelright.color = "black"
                        isLeftActive = false;
                    }
                    root.clicked(root)
                }
            }

        }



}
