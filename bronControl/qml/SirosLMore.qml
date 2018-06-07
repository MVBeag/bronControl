import QtQuick 2.8
import QtQuick.Controls 2.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2
import ch.bron.BronModels 1.0
import TextFieldDoubleValidator 1.0
import "."


FocusScope{
  id: root

  width: DisplayCtrl.portraitWidth

  property var deviceType
  property var device
  property string name: root.device.name

  property var intval: IntValidator{bottom: 1; top: 99}
  property var intval050: IntValidator{bottom: 0; top:50}
  property var intval150: IntValidator{bottom: 1; top:50}
  property var regval: RegExpValidator{}
  property var dblval010_3: TextFieldDoubleValidator{bottom: 0.000; decimals: 3; top: 10.000; notation: DoubleValidator.StandardNotation}
  property var dblval050_2: TextFieldDoubleValidator{bottom: 0.00; decimals: 2; top: 50.00; notation: DoubleValidator.StandardNotation}

  property bool on: true
  property bool off: false

  signal back();

  Component.onCompleted: btQuit.forceActiveFocus()

  function findTagFromValue(val,sel){
      if(Array.isArray(sel)){
          var i
          var length = sel.length
          for(i =  0; i < length; i++){
              if(val === sel[i].val){
                  return sel[i].tag
              }
          }

      }
      else{
          return undefined
      }
  }

  ColumnLayout{
      id: layout
      width: parent.width

      property real heightWoFlicker: header.height + DisplayCtrl.gridMargin * 2 + deviceMoreSettingBottenLine.height
      property real maxFlickerHeight: root.height - heightWoFlicker
      spacing: 0
      Item{
          id: header
          Layout.preferredWidth: parent.width
          Layout.preferredHeight: childrenRect.height
          Rectangle{
            id: deviceMoreCogniCol
            width: DisplayCtrl.dp(60)
            height: width
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.05
            anchors.margins: DisplayCtrl.dp(20)
            color: BronColors.intToColer(root.device.cogniColor)
          }
          Text{
              id: selectedMoreName
              font.family: bronFontBold.name
              font.pointSize: DisplayCtrl.point20Size
              font.bold: true
              anchors.verticalCenter: deviceMoreCogniCol.verticalCenter
              anchors.left: deviceMoreCogniCol.right
              anchors.leftMargin: DisplayCtrl.dp(20)
              color: "white"
              text: root.device.fullModelName
          }
          Text{
              id: swVersion
              font.family: bronFontBold.name
              font.pointSize: DisplayCtrl.point20Size
              font.bold: true
              anchors.verticalCenter: deviceMoreCogniCol.verticalCenter
              anchors.left: selectedMoreName.right
              anchors.leftMargin: DisplayCtrl.dp(30)
              color: "white"
              text: "V"+(root.device.swVersion/100).toFixed(2)
          }
          BronToolButton{
              id: btQuit
              anchors.verticalCenter: deviceMoreCogniCol.verticalCenter
              anchors.right: parent.right
              font.family: googleMaterial.name
              font.pointSize: DisplayCtrl.point30Size

              KeyNavigation.tab: addressLine
              KeyNavigation.backtab: hSLine
              Keys.onReturnPressed:{
                  event.accepted = true
                  root.back()
              }
              focus: true
              text: DisplayCtrl.icClose
              onClicked: root.back()
          }
          Rectangle{
            id: deviceMoreSettingUpperLine
            property real minuend//mapToItem(deviceMore,0,0).y

            width: parent.width
            height: DisplayCtrl.dp(4)
            anchors.top: deviceMoreCogniCol.bottom
            anchors.topMargin: DisplayCtrl.gridMargin
            color: "white"
            Component.onCompleted:{
                minuend = mapToItem(deviceMore,0,0).y + deviceMoreSettingUpperLine.height + DisplayCtrl.gridMargin + DisplayCtrl.gridMargin
            }
          }
      }
      Item{
          Layout.preferredHeight: DisplayCtrl.gridMargin
      }
      Flickable{
          id: flick

          // do the following to break dependency loops
          property real fheight
          Component.onCompleted: fheight = flick.height

          property real flickLeftHeight: layout.maxFlickerHeight - fheight

          Layout.preferredWidth: parent.width
          Layout.preferredHeight: flickLeftHeight > 0 ? contentHeight : contentHeight + flickLeftHeight
          Layout.alignment: Qt.AlignHCenter
          focus: true
          clip: true
          interactive: height < contentHeight ? true : false
          contentWidth: parent.width
          contentHeight:  scoroLayout.height

          ScrollBar.vertical: ScrollBar {
              id: controlSB

              implicitWidth: Math.max(background ? background.implicitWidth : 0,
                                      contentItem.implicitWidth + leftPadding + rightPadding)
              implicitHeight: Math.max(background ? background.implicitHeight : 0,
                                       contentItem.implicitHeight + topPadding + bottomPadding)


              contentItem: Rectangle {
                  id: handleCt

                  implicitWidth: DisplayCtrl.dp(5)
                  implicitHeight: DisplayCtrl.dp(5)

                  radius: width / 2
                  color: controlSB.pressed ? BronColors.bronBlue : "white"
                  visible: controlSB.size < 1.0
                  opacity: 1.0

                  states: State {
                      name: "active"
                      when: controlSB.active
                  }

                  transitions: [
                      Transition {
                          to: "active"
                          SequentialAnimation {
                              NumberAnimation { target: handleCt; duration: 200; property: "opacity"; to: 1.0 }
                          }
                      },
                      Transition {
                          from: "active"
                          SequentialAnimation {
                              PauseAnimation { duration: 450 }
                              NumberAnimation { target: handleCt; duration: 200; property: "opacity"; to: 0.0 }
                          }
                      }
                  ]
              }
          }

          ColumnLayout{
              id: scoroLayout
              width: parent.width * 0.9
              focus: true
              anchors.horizontalCenter: parent.horizontalCenter
              spacing: DisplayCtrl.gridMargin
              TextInputLine{
                  id: addressLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: countLowEnergyFlashLine
                  prevFocusItem: btQuit
                  focus: true
                  hints: Qt.ImhDigitsOnly
                  name: "LAMP ADDRESS"
                  isPW: false
                  validate: intval
                  readOnly: false
                  actValue: root.device.address
                  decPlaces: 0
                  onValue: root.device.address = result.val
              }
              TextInputLine{
                  id: countLowEnergyFlashLine
                  Layout.preferredWidth: parent.width * 1.5
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: countMiddleEnergyFlashLine
                  prevFocusItem: addressLine
                  focus: true
                  hints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                  name: "COUNT LOW ENERGY FLASH"
                  isPW: false
                  validate: intval
                  readOnly: false
                  actValue: root.device.CountLowEnergyFlash
                  decPlaces: 0
                  onValue: root.device.CountLowEnergyFlash = result.val
              }
              TextInputLine{
                  id: countMiddleEnergyFlashLine
                  Layout.preferredWidth: parent.width * 1.5
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: countHighEnergyFlashLine
                  prevFocusItem: countLowEnergyFlashLine
                  focus: true
                  hints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                  name: "COUNT MIDDLE ENERGY FLASH"
                  isPW: false
                  validate: intval
                  readOnly: false
                  actValue: root.device.CountMiddleEnergyFlash
                  decPlaces: 0
                  onValue: root.device.CountLowEnergyFlash = result.val
              }
              TextInputLine{
                  id: countHighEnergyFlashLine
                  Layout.preferredWidth: parent.width * 1.5
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: tempCond
                  prevFocusItem: countMiddleEnergyFlashLine
                  focus: true
                  hints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                  name: "COUNT HIGH ENERGY FLASH"
                  isPW: false
                  validate: intval
                  readOnly: false
                  actValue: root.device.CountHighEnergyFlash
                  decPlaces: 0
                  onValue: root.device.CountLowEnergyFlash = result.val
              }
              TextInputLine{
                  id: tempCond
                  Layout.preferredWidth: parent.width * 1.5
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: tempCooling
                  prevFocusItem: countHighEnergyFlashLine
                  focus: true
                  hints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                  name: "TEMP_COND"
                  isPW: false
                  validate: intval
                  readOnly: false
                  actValue: root.device.TempCond
                  decPlaces: 0
                  onValue: root.device.CountLowEnergyFlash = result.val
              }
              TextInputLine{
                  id: tempCooling
                  Layout.preferredWidth: parent.width * 1.5
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: delayLine
                  prevFocusItem: tempCond
                  focus: true
                  hints: Qt.ImhDigitsOnly | Qt.ImhNoPredictiveText
                  name: "TEMP_COOLING"
                  isPW: false
                  validate: intval
                  readOnly: false
                  actValue: root.device.TempCooling
                  decPlaces: 0
                  onValue: root.device.CountLowEnergyFlash = result.val
              }
              TextInputLine{
                  id: delayLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: standbyLine
                  prevFocusItem: tempCooling
                  focus: true
                  hints: Qt.ImhFormattedNumbersOnly
                  name: "LAMP DELAY"
                  isPW: false
                  validate: dblval050_2
                  unit: "s"
                  readOnly: false
                  actValue: root.device.delay
                  decPlaces: 2
                  onValue: root.device.delay = result.val
              }
              ButtonInputLine{
                  id: standbyLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: modellingLightLine
                  prevFocusItem: delayLine
                  focus: true
                  name: "STANDBY"
                  actValue: root.device.standby
                  isPush: false
                  onValue: root.device.standby = result.val
              }
              ButtonInputLine{
                  id: modellingLightLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: sequenceLine
                  prevFocusItem: standbyLine
                  focus: true
                  name: "MODELLING LIGHT"
                  actValue: root.device.modLight === 0 ? 0 : 1
                  onValue: root.device.modLight = result.val === false ? 0 : 1
              }
              TextInputLine{
                  id: sequenceLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: intervalLine
                  prevFocusItem: modellingLightLine
                  focus: true
                  hints: Qt.ImhDigitsOnly
                  name: "SEQUENCE"
                  validate: intval150
                  actValue: root.device.sequence
                  onValue: root.device.sequence = result.val
              }
              TextInputLine{
                  id: intervalLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: remoteCtrl
                  prevFocusItem: sequenceLine
                  focus: true
                  hints: Qt.ImhFormattedNumbersOnly
                  name: "INTERVAL"
                  unit:"s"
                  decPlaces: 3
                  validate: dblval010_3
                  actValue: root.device.pauseTime
                  onValue: root.device.pauseTime = result.val
              }
              ComboInputLine{
                  id: remoteCtrl
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: synSequenceModeLine
                  prevFocusItem: intervalLine
                  focus: true
                  name: "FLASH CONTROL"
                  selections: SirosSelects.syncMode
                  selRole: "tag"
                  actValue: SirosSelects.convertOverallSyncToSirosSync(root.device.remoteSwitch)
                  onValue: root.device.remoteSwitch = SirosSelects.convertSirosSyncToOverallSync(result.val)
              }
              ComboInputLine{
                  id: synSequenceModeLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: alternateNumDevicesLine
                  prevFocusItem: remoteCtrl
                  focus: true
                  name: "SYN. SEQ. MODE"
                  selections: SirosSelects.synchronizedSequenceMode//ScoroSelects.alternateNumDevices
                  selRole: "tag"
                  actValue: root.device.syncSequence
                  onValue: root.device.syncSequence = result.val
              }
              ComboInputLine{
                  id: alternateNumDevicesLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: alternateDevicesDelay
                  prevFocusItem: synSequenceModeLine
                  focus: true
                  name: actValue < 2 ?"FREEMASK" : "ALTERNATE"
                  selections: SirosSelects.freemaskAlternateType//ScoroSelects.alternateNumDevices
                  selRole: "tag"
                  actValue: root.device.application
                  onValue: root.device.application = result.val
              }
              ComboInputLine{
                  id: alternateDevicesDelay
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: chargeTimeLine
                  prevFocusItem: alternateNumDevicesLine
                  focus: true
                  name: alternateNumDevicesLine.actValue < 3 ? "FREEMASK STATE" : "ALTERNATE STATE"
                  selections: SirosSelects.maskGroupType
                  selRole: "tag"
                  actValue: root.device.maskgroup
                  onValue: root.device.maskgroup = result.val
              }
              ButtonInputLine{
                  id: chargeTimeLine
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: speedLine
                  prevFocusItem: alternateDevicesDelay
                  focus: true
                  name: "ECO"
                  actValue: root.device.slowCharge
                  onValue: root.device.slowCharge = result.val
              }
              ButtonInputLine{
                  id: speedLine

                  property int speedMode: device.speed
                  onSpeedModeChanged:  {
                      actValue = speedMode === ParaSelectors.Speed ? 1 : 0
                  }

                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: hSLine
                  prevFocusItem: chargeTimeLine
                  focus: true
                  name: "SPEED"
                  actValue: root.device.speed === ParaSelectors.Speed ? 1 : 0
                  onValue: root.device.speed = result.val === true ? ParaSelectors.Speed : ParaSelectors.Normal
              }
              ButtonInputLine{
                  id: hSLine

                  property int speedMode: device.speed
                  onSpeedModeChanged: actValue = speedMode === ParaSelectors.HS ? 1 : 0
                  Layout.preferredWidth: parent.width
                  Layout.preferredHeight: DisplayCtrl.isMobilePlattform ? DisplayCtrl.dp(120) : DisplayCtrl.dp(60)
                  nextFocusItem: btQuit
                  prevFocusItem: speedLine
                  focus: true
                  name: "HS MODE"
                  actValue: root.device.speed === ParaSelectors.HS ? 1 : 0
                  onValue: root.device.speed = result.val === true ? ParaSelectors.HS : ParaSelectors.Normal
              }
          }
      }
      Item{
          Layout.preferredHeight: DisplayCtrl.gridMargin
      }
      Rectangle{
        id: deviceMoreSettingBottenLine
        Layout.preferredWidth: parent.width
        Layout.preferredHeight: DisplayCtrl.dp(2)
        color: "white"//BronColors.disabledGrey
      }
  }

  Rectangle{
    id: deviceMore
    anchors.fill: layout
    z: layout.z - 1
    color: BronColors.bronBlack
  }
  Rectangle{
      anchors.top: layout.top
      anchors.left: layout.right
      width: DisplayCtrl.dp(2)
      height: layout.height
  }

}
