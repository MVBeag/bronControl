import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import "."


Item{
  id: root
  
  property var group
  property alias selGroup: selectedGroup.text
  property color cogniCol: "white"

  signal back();

  Rectangle{
      id: visualBackground
      anchors.top: parent.top
      anchors.left: parent.left

      width: DisplayCtrl.portraitWidth
      height: DisplayCtrl.portraitHeight / 3
      color: "black"

      Rectangle{
          id: cogniCol
          width: DisplayCtrl.dp(60)
          height: width
          anchors.top: parent.top
          anchors.left: parent.left
          anchors.margins: DisplayCtrl.dp(20)
          color: root.cogniCol
      }
      Text{
          id: selectedGroup
          font.family: bronFontBold.name
          font.pointSize: DisplayCtrl.point20Size
          anchors.top: parent.top
          anchors.left: cogniCol.right
          anchors.leftMargin: DisplayCtrl.dp(20)
          color: "white"
      }
      BronToolButton{
          id: backButton

          anchors.top: parent.top
          anchors.right: parent.right
          anchors.margins: DisplayCtrl.dp(20)

          font.family: googleMaterial.name

          text: DisplayCtrl.icClose
          onClicked: root.back()
      }



      EnergySetter{
          id: groupEnergy

          color: visualBackground.color

          width: DisplayCtrl.energySetterWidth
          height: parent.height - cogniCol.height - DisplayCtrl.gridMargin
          anchors.top: cogniCol.bottom
          anchors.horizontalCenter: parent.horizontalCenter

          min: 0.0
          max: 10.0
          currentMin: root.group.maxMinPossibleEnergy
          currentMax: root.group.minMaxPossibleEnergy
          currentRangeMax: root.group.maxPower
          currentRangeMin: root.group.minPower
          onIncPower: root.group.increasePower = increment
          onDecPower: root.group.decreasePower = decrement
      }
  }
  Rectangle{
    id: groupSettingBottenLine
    width: parent.width
    height: DisplayCtrl.dp(2)
    anchors.top: visualBackground.bottom
    color: "white"
  }
  Rectangle{
    id: transparentArea
    width: parent.width
    height: parent.height - visualBackground.height - groupSettingBottenLine.height
    anchors.top: groupSettingBottenLine.bottom
    color: "black"
    opacity: 0.4
  }
  MouseArea{
    id: backArea
    anchors.fill: transparentArea
    onClicked: root.back()
  }
}
