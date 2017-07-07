TEMPLATE = app

QT += core network widgets

CONFIG += C++11

#command switches for msvc2015
win32-msvc2015 {
    CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT FrameworkOnly
    CONFIG(debug, debug|release):DEFINES += FrameworkOnly
    CONFIG(debug, debug|release):QMAKE_CXXFLAGS += /MDd /FS
    CONFIG(release, debug|release):QMAKE_CXXFLAGS += /MD /FS
    QMAKE_LFLAGS += /NODEFAULTLIB:libcmt.lib
    message("MSVC2015")
}

#command switches for gnu
win32-g++{
    message("MinGW 32")
}

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
    BronDevices/demodevicepara.cpp \
    BronDevices/deviceFinder.cpp \
    BronDevices/scanDevice.cpp \
    BronDevices/creatorDevice.cpp \
    BronDevices/devicesirrosL.cpp \
    BronDevices/devicesirrosS.cpp \
    BronHelpers/helpers.cpp \
    BronDevices/devicescoro.cpp \
    BronDevices/genLamp.cpp \
    BronHelpers/commandsequence.cpp \
    BronHelpers/deviceaction.cpp \
    BronHelpers/studiosequence.cpp \
    BronHelpers/target/ethaddress.cpp \
    middleware/networkapi_impl.cpp \
    middleware/sequencegenerator.cpp \
    middleware/timerapi.cpp \
    appcontrol.cpp

INCLUDEPATH += middleware \
               middleware/asn \
               BronControls \
               BronDevices \
               BronModels \
               BronHelpers \
               BronHelpers/target \
               BronManagers \
               BronModelElements \
               StateMachines

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
    BronDevices/errors.h \
    BronManagers/studiomgr.h \
    BronModelElements/bronstudio.h \
    BronModelElements/brongroup.h \
    BronHelpers/request.h \
    StateMachines/remoteClientSM.h \
    BronHelpers/helpers.h \
    doxygenmain.h \
    BronDevices/demodevicepara.h \
    BronDevices/deviceFinder.h \
    globalinit.h \
    BronDevices/scanDevice.h \
    BronDevices/creatorDevice.h \
    BronDevices/devicesirrosL.h \
    BronDevices/devicesirrosS.h \
    BronHelpers/target/ethaddress.h \
    BronDevices/devicescoro.h \
    BronDevices/genLamp.h \
    BronHelpers/commandsequence.h \
    BronHelpers/deviceaction.h \
    BronHelpers/ipersistency.h \
    BronHelpers/studiosequence.h \
    middleware/mwtypes.h \
    middleware/networkapi.h \
    middleware/networkapi_impl.h \
    middleware/remoteaddressinfo.h \
    middleware/sequencegenerator.h \
    middleware/timerapi.h \
    appcontrol.h \
    generalinfo.h

DISTFILES += \
    Doc/Doxyfile
