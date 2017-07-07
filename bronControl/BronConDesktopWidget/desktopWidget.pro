QT += network widgets

HEADERS       = desktopWidget.h \
    bronglobals.h \
    triangularbutton.h \
    ../RemoteInterface/bronconinterface.h
SOURCES       = desktopWidget.cpp \
                main.cpp \
    bronglobals.cpp \
    triangularbutton.cpp \
    ../RemoteInterface/bronconinterface.cpp

INCLUDEPATH += ../RemoteInterface

FORMS +=


