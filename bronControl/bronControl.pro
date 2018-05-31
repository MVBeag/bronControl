TEMPLATE = app

QT += core quickcontrols2 widgets network

CONFIG += C++11

SOURCES += main.cpp \
    middleware/asn/asn_codecs_prim.c \
    middleware/asn/ber_decoder.c \
    middleware/asn/ber_tlv_length.c \
    middleware/asn/ber_tlv_tag.c \
    middleware/asn/BIT_STRING.c \
    middleware/asn/BOOLEAN.c \
    middleware/asn/constr_CHOICE.c \
    middleware/asn/constr_SEQUENCE.c \
    middleware/asn/constr_TYPE.c \
    middleware/asn/constraints.c \
    middleware/asn/der_encoder.c \
    middleware/asn/IA5String.c \
    middleware/asn/INTEGER.c \
    middleware/asn/Message.c \
    middleware/asn/MessageData.c \
    middleware/asn/MessageError.c \
    middleware/asn/MessageResult.c \
    middleware/asn/MessageSetParameter.c \
    middleware/asn/NativeEnumerated.c \
    middleware/asn/NativeInteger.c \
    middleware/asn/NULL.c \
    middleware/asn/OCTET_STRING.c \
    middleware/asn/ParamValue.c \
    middleware/asn/per_decoder.c \
    middleware/asn/per_encoder.c \
    middleware/asn/per_opentype.c \
    middleware/asn/per_support.c \
    middleware/asn/xer_decoder.c \
    middleware/asn/xer_encoder.c \
    middleware/asn/xer_support.c \
    BronControls/NetWorkScanner.cpp \
    BronControls/resolution.cpp \
    BronDevices/device.cpp \
    BronDevices/devicepara.cpp \
    BronDevices/devicesirros.cpp \
    BronDevices/errors.cpp \
    BronManagers/studiomgr.cpp \
    BronModelElements/bronstudio.cpp \
    BronModelElements/brongroup.cpp \
    BronHelpers/request.cpp \
    StateMachines/remoteClientSM.cpp \
    QMLInterface/deviceproxy.cpp \
    QMLInterface/paraproxy.cpp \
    QMLInterface/studioproxy.cpp \
    QMLInterface/bronlistmodel.cpp \
    QMLInterface/bronitemapi.cpp \
    QMLInterface/brongroupproxy.cpp \
    QMLInterface/bronstudiomgrproxy.cpp \
    BronDevices/demodevicepara.cpp \
    BronDevices/deviceFinder.cpp \
    BronDevices/scanDevice.cpp \
    BronDevices/creatorDevice.cpp \
    BronDevices/devicesirrosL.cpp \
    BronDevices/devicesirrosS.cpp \
    BronHelpers/helpers.cpp \
    BronDevices/devicescoro.cpp \
    BronDevices/genLamp.cpp \
    QMLInterface/paramappings.cpp \
    RemoteInterface/bronconinterface.cpp \
    appcontrol.cpp \
    BronHelpers/commandsequence.cpp \
    middleware/timerapi.cpp \
    middleware/networkapi_impl.cpp \
    middleware/sequencegenerator.cpp \
    BronHelpers/deviceaction.cpp \
    BronHelpers/studiosequence.cpp \
    BronModelElements/treemodel.cpp \
    BronModelElements/treeitem.cpp \
    QMLInterface/networkproxy.cpp \
    QMLInterface/treerootproxy.cpp


RESOURCES += qml.qrc

INCLUDEPATH += middleware \
               middleware/asn \
               middleware  \
               BronControls \
               BronDevices \
               BronModels \
               BronHelpers \
               BronManagers \
               BronModelElements \
               StateMachines \
               QMLInterface \
               RemoteInterface

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH = qml

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    middleware/asn/asn_application.h \
    middleware/asn/asn_codecs.h \
    middleware/asn/asn_codecs_prim.h \
    middleware/asn/asn_internal.h \
    middleware/asn/asn_system.h \
    middleware/asn/ber_decoder.h \
    middleware/asn/ber_tlv_length.h \
    middleware/asn/ber_tlv_tag.h \
    middleware/asn/BIT_STRING.h \
    middleware/asn/BOOLEAN.h \
    middleware/asn/constr_CHOICE.h \
    middleware/asn/constr_SEQUENCE.h \
    middleware/asn/constr_TYPE.h \
    middleware/asn/constraints.h \
    middleware/asn/der_encoder.h \
    middleware/asn/IA5String.h \
    middleware/asn/INTEGER.h \
    middleware/asn/Message.h \
    middleware/asn/MessageData.h \
    middleware/asn/MessageError.h \
    middleware/asn/MessageResult.h \
    middleware/asn/MessageSetParameter.h \
    middleware/asn/NativeEnumerated.h \
    middleware/asn/NativeInteger.h \
    middleware/asn/NULL.h \
    middleware/asn/OCTET_STRING.h \
    middleware/asn/ParamValue.h \
    middleware/asn/per_decoder.h \
    middleware/asn/per_encoder.h \
    middleware/asn/per_opentype.h \
    middleware/asn/per_support.h \
    middleware/asn/xer_decoder.h \
    middleware/asn/xer_encoder.h \
    middleware/asn/xer_support.h \
    BronControls/NetWorkScanner.h \
    BronControls/resolution.h \
    BronDevices/device.h \
    BronDevices/devicepara.h \
    BronDevices/devicesirros.h \
    BronDevices/paraSelectables.h \
    BronDevices/errors.h \
    BronManagers/studiomgr.h \
    BronModelElements/bronstudio.h \
    BronModelElements/brongroup.h \
    BronHelpers/request.h \
    StateMachines/remoteClientSM.h \
    BronHelpers/helpers.h \
    QMLInterface/deviceproxy.h \
    QMLInterface/paraproxy.h \
    QMLInterface/studioproxy.h \
    QMLInterface/bronlistmodel.h \
    QMLInterface/bronitemapi.h \
    doxygenmain.h \
    QMLInterface/brongroupproxy.h \
    QMLInterface/bronstudiomgrproxy.h \
    BronDevices/demodevicepara.h \
    BronDevices/deviceFinder.h \
    globalinit.h \
    BronDevices/scanDevice.h \
    BronDevices/creatorDevice.h \
    BronDevices/devicesirrosL.h \
    BronDevices/devicesirrosS.h \
    BronDevices/devicescoro.h \
    BronDevices/genLamp.h \
    QMLInterface/paramappings.h \
    RemoteInterface/bronconinterface.h \
    appcontrol.h \
    BronHelpers/ipersistency.h \
    generalinfo.h \
    QMLInterface/texteditdoublevalidator.h \
    BronHelpers/commandsequence.h \
    middleware/networkapi.h \
    middleware/timerapi.h \
    middleware/remoteaddressinfo.h \
    middleware/networkapi_impl.h \
    middleware/mwtypes.h \
    middleware/sequencegenerator.h \
    BronHelpers/deviceaction.h \
    BronHelpers/studiosequence.h \
    BronModelElements/treemodel.h \
    BronModelElements/treeitem.h \
    QMLInterface/networkproxy.h \
    QMLInterface/treerootproxy.h \
    QMLInterface/texteditintvalidator.h \

DISTFILES += \
    Doc/Doxyfile \
    bronControl.rc \
    res/icons font codes.txt \
    Doc/QMLUpdateCallGraph.txt \
    Doc/deployWin.bat \
    ../BronConInstaller/config/config.xml \
    ../BronConInstaller/packages/ch.beag.bronCon/meta/package.xml \
    ../BronConInstaller/packages/ch.beag.bronCon/meta/installscript.qs \
    IOS/iconMaster/ios/AppIcon.appiconset/Contents.json \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-20x20@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-20x20@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-20x20@3x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-29x29@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-29x29@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-29x29@3x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-40x40@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-40x40@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-40x40@3x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-57x57@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-57x57@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-60x60@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-60x60@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-60x60@3x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-72x72@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-72x72@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-76x76@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-76x76@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-76x76@3x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-App-83.5x83.5@2x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-Small-50x50@1x.png \
    IOS/iconMaster/ios/AppIcon.appiconset/Icon-Small-50x50@2x.png

#command switches for msvc2015
win32-msvc {
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG(debug, debug|release):CONFIG += qml_debug declarative_debug
    CONFIG(debug, debug|release):QMAKE_CXXFLAGS += /MDd /FS
    CONFIG(release, debug|release):QMAKE_CXXFLAGS += /MD /FS
    QMAKE_LFLAGS += /NODEFAULTLIB:libcmt.lib
    message("MSVC2015")
    SOURCES += BronHelpers/target/ethaddress.cpp
    INCLUDEPATH += BronHelpers/target
    HEADERS += BronHelpers/target/ethaddress.h
    RC_FILE = bronControl.rc
}

#command switches for msvc2017
win32-msvc2017 {
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG(debug, debug|release):CONFIG += qml_debug declarative_debug
    CONFIG(debug, debug|release):QMAKE_CXXFLAGS += /MDd /FS
    CONFIG(release, debug|release):QMAKE_CXXFLAGS += /MD /FS
    QMAKE_LFLAGS += /NODEFAULTLIB:libcmt.lib
    message("MSVC2017")
    SOURCES += BronHelpers/target/ethaddress.cpp
    INCLUDEPATH += BronHelpers/target
    HEADERS += BronHelpers/target/ethaddress.h
    RC_FILE = bronControl.rc
}

#command switches for msvc2017
amd64-msvc2017 {
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG(debug, debug|release):CONFIG += qml_debug declarative_debug
    CONFIG(debug, debug|release):QMAKE_CXXFLAGS += /MDd /FS
    CONFIG(release, debug|release):QMAKE_CXXFLAGS += /MD /FS
    QMAKE_LFLAGS += /NODEFAULTLIB:libcmt.lib
    message("MSVC2017")
    SOURCES += BronHelpers/target/ethaddress.cpp
    INCLUDEPATH += BronHelpers/target
    HEADERS += BronHelpers/target/ethaddress.h
    RC_FILE = bronControl.rc
}


#command switches for gnu
win32-g++{
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG(debug, debug|release):CONFIG += qml_debug declarative_debug
    message("MinGW 32")
    SOURCES += BronHelpers/target/ethaddress.cpp
    INCLUDEPATH += BronHelpers/target
    HEADERS += BronHelpers/target/ethaddress.h
    RC_FILE = bronControl.rc
}

macx-ios-clang{
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG(debug, debug|release):CONFIG += qml_debug declarative_debug
    message("clang for ios")
    SOURCES += BronHelpers/target/ios/ethaddress.cpp
    INCLUDEPATH += BronHelpers/target/ios
    HEADERS += BronHelpers/target/ios/ethaddress.h
    ICON = res/BronCon.icns
}

macx-clang{
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG(debug, debug|release):CONFIG += qml_debug declarative_debug
    message("macx-clang")
    SOURCES += BronHelpers/target/ethaddress.cpp
    INCLUDEPATH += BronHelpers/target
    HEADERS += BronHelpers/target/ethaddress.h
    ICON = res/BronCon.icns
}

android-g++{
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG(debug, debug|release):CONFIG += qml_debug declarative_debug
    message("android-g++")
    QT += androidextras
    SOURCES += BronHelpers/target/ethaddress.cpp

    INCLUDEPATH += BronHelpers/target
    HEADERS += BronHelpers/target/ethaddress.h

    DISTFILES += android/AndroidManifest.xml \
                 android/gradle/wrapper/gradle-wrapper.jar \
                 android/gradlew \
                 android/res/values/libs.xml \
                 android/build.gradle \
                 android/gradle/wrapper/gradle-wrapper.properties \
                 android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

}

ios{
    QMAKE_INFO_PLIST = IOS/Info.plist
    ios_icon.files = $$files($$PWD/IOS/iconMaster/ios/AppIcon.appiconset/Icon-App*.png)
    QMAKE_BUNDLE_DATA += ios_icon
}
