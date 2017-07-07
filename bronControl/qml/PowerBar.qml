/*
 * Power Bar
 *
 * A graduated bar that display current f-stop among all f-stops
 * avaiable and min/max f-stops
 *
 * Author: Sacha Bron
 * Date: 18 August 2016
 */

import QtQuick 2.7
import "."

Rectangle {
    id: powerBar

//    FontLoader {id: bronFont; source: "qrc:///fonts/DINOT-Regular.otf"}
//    FontLoader {id: bronFontBold; source: "qrc:///fonts/DINOT-Bold.otf"}

    // Public properties that need to be modified by the controller
    property real min: 0         // start of the power bar
    property real max: 10        // end of the power bar
    property real currentMin: 0  //absolute lower limit for linear range
    property real currentMax: 10 //absolute higher limit for linear range
    property real currentRangeMin :0 // low limit of movable range
    property real currentRangeMax :10 // high limit of movable range

    property real margin: 20
    property real nbBar: (max - min) * 2 + 1
    property real currentRangeXMin: powerBar.currentRangeMin * powerBar.width * 2 / (powerBar.nbBar - 1)
    property real currentRangeXMax: powerBar.currentRangeMax * powerBar.width * 2 / (powerBar.nbBar - 1)
    property real currentMinX: (powerBar.currentMin - powerBar.min) * powerBar.width * 2 / (powerBar.nbBar - 1)
    property real currentMaxX: (powerBar.currentMax - powerBar.min) * powerBar.width * 2 / (powerBar.nbBar - 1)

    property bool isNegLimit: false
    property bool isPosLimit: false

    signal incFunction(var val)
    signal decFunction(var val)

    property real barHeight: powerBar.height/4
    property real textHeight: powerBar.height/4
    property real textWidth: powerBar.width/10

    width: parent.width - margin * 2
    height: DisplayCtrl.dp(40)
    color: "black"

    // Backgrounds
    Rectangle {
        id: backgrounds
        width: parent.width
        height: barHeight
        anchors.bottom: negativeText.top
        color: BronColors.bronBlue
        opacity: 0.2
    }

    Rectangle {
        id: validRange
        x: powerBar.currentMinX
        width: powerBar.currentMaxX - x
        height: barHeight
        anchors.bottom: negativeText.top
        color: BronColors.bronBlue
        opacity: 0.5
    }

    Rectangle {
        x: powerBar.currentRangeXMin
        width: powerBar.currentRangeXMax - x
        height: barHeight
        anchors.bottom: negativeText.top
        color: BronColors.bronBlue
        opacity: 0.7
    }

    // Little bars
    Repeater {
        id: helpRepeater
        model: powerBar.nbBar

        Rectangle {
            anchors.bottom: negativeText.top
            width: DisplayCtrl.dp(2)
            height: {
                // The first and last bars are longer
                if (index == 0 || index == powerBar.nbBar - 1) {
                    barHeight * 2
                } else {
                    // Unit are longer than half f-stops
                    (barHeight * (((index+1) % 2) + 1)) / 2
                }
            }
            color: BronColors.bronBlue
            x: index * powerBar.width / (powerBar.nbBar - 1)
        }
    }

    // currentMin bar
    Rectangle {
        anchors.bottom: negativeText.top
        width: DisplayCtrl.dp(2)
        height: barHeight * 2
        color: BronColors.bronBlue
        x: powerBar.currentMinX
    }

    // currentMax bar
    Rectangle {
        anchors.bottom: negativeText.top
        width: DisplayCtrl.dp(2)
        height: barHeight * 2
        color: BronColors.bronBlue
        x: powerBar.currentMaxX
    }

    // CursorRangeMin
    Rectangle {
        id: cursorRangeMin
        width: DisplayCtrl.dp(4)
        height: barHeight * 1.5
        color: "white"
        x: powerBar.currentRangeXMin
        anchors.bottom: negativeText.top
    }

    // CursorRangeMax
    Rectangle {
        id: cursorRangeMax
        width: DisplayCtrl.dp(4)
        height: barHeight * 1.5
        color: "white"
        x: powerBar.currentRangeXMax
        anchors.bottom: negativeText.top
    }


    // 0.0 text
    Text {
        id: zeroTxt
        height: textHeight
        width: textWidth
        padding: 0

        // Disapear when the currentMin is too low so they don't overlap
        text: (powerBar.currentMin > powerBar.min + 1) ? powerBar.min.toFixed(1) :  ""
        font.family: bronFont.name
        font.pixelSize: DisplayCtrl.dp(20)
        minimumPixelSize: DisplayCtrl.dp(2)
        fontSizeMode: Text.HorizontalFit
        horizontalAlignment: Text.AlignLeft
        anchors.top: powerBar.top
        anchors.left: powerBar.left
        color: "white"
    }

    // 10.0 text
    Text {
        height: textHeight
        width: textWidth
        // Disapear when the currentMax is too high so they don't overlap
        text: (powerBar.currentMax < powerBar.max - 1) ? powerBar.max.toFixed(1) :  ""
        font.family: bronFont.name
        font.pixelSize: DisplayCtrl.dp(20)
        minimumPixelSize: DisplayCtrl.dp(2)
        fontSizeMode: Text.HorizontalFit
        horizontalAlignment: Text.AlignRight
        anchors.top: powerBar.top
        anchors.right: powerBar.right
        color: "white"
    }

    // Min text
    Text {
        height: textHeight
        width: textWidth
        text: "MIN " + powerBar.currentMin.toFixed(1)
        font.family: bronFont.name
        font.pixelSize: DisplayCtrl.dp(20)
        minimumPixelSize: DisplayCtrl.dp(2)
        fontSizeMode: Text.HorizontalFit
        anchors.top: powerBar.top
        horizontalAlignment: Text.AlignRight
        x: powerBar.currentMinX
        color: "white"
    }

    // Max text
    Text {
        z: 1
        height: textHeight
        width: textWidth
        text: "MAX " + powerBar.currentMax.toFixed(1)
        font.family: bronFont.name

        minimumPixelSize: DisplayCtrl.dp(2)
        font.pixelSize: DisplayCtrl.dp(20)
        fontSizeMode: Text.HorizontalFit
        anchors.top: powerBar.top
        anchors.right: powerBar.right
        horizontalAlignment: Text.AlignRight
        color: "white"
    }

    // Negative value under bar
    Text {
        id: negativeText
        height: textHeight
        width: textWidth

        text: (powerBar.currentMin - powerBar.currentRangeMin).toFixed(1)
        font.family: bronFont.name
        font.pixelSize: DisplayCtrl.dp(20)
        minimumPixelSize: DisplayCtrl.dp(2)
        fontSizeMode: Text.HorizontalFit
        horizontalAlignment: Text.AlignHCenter

        anchors.bottom: powerBar.bottom

        x: powerBar.currentMinX + (powerBar.currentRangeXMin - powerBar.currentMinX) / 2 - width/2
        color: "white"
        onTextChanged: {
            if(text == "0.0"){
                powerBar.isNegLimit = true
            }
            else{
                powerBar.isNegLimit = false
            }
        }
    }

    // Positive value under bar
    Text {
        id: positiveText
        height: textHeight
        width: textWidth

        text: "+" + (powerBar.currentMax - powerBar.currentRangeMax).toFixed(1)
        font.family: bronFont.name
        font.pixelSize: DisplayCtrl.dp(20)
        minimumPixelSize: DisplayCtrl.dp(2)
        fontSizeMode: Text.HorizontalFit
        horizontalAlignment: Text.AlignHCenter

        anchors.bottom: powerBar.bottom
        x: powerBar.currentRangeXMax + (powerBar.currentMaxX - powerBar.currentRangeXMax) / 2 - width/2
        color: "white"
        onTextChanged: {
            if(text == "0.0"){
                powerBar.isPosLimit = true
            }
            else{
                powerBar.isPosLimit = false
            }
        }

    }
}
