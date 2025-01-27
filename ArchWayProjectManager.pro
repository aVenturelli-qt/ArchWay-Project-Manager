QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    groupeditor.cpp \
    main.cpp \
    customWidgets\viewtogglesw.cpp \
    customWidgets\mapperpathinfo.cpp \
    customWidgets\statechip.cpp \
    customWidgets\pathinfo.cpp \
    customWidgets\toggleswitchdelegate.cpp \
    archway.cpp \
    pathsinfotablemodel.cpp

HEADERS += \
    archway.h \
    groupeditor.h \
    customWidgets\mapperpathinfo.h \
    customWidgets\viewtogglesw.h \
    customWidgets\statechip.h \
    customWidgets\pathinfo.h \
    customWidgets\toggleswitchdelegate.h \
    pathsinfotablemodel.h

FORMS += \
    archway.ui \
    groupeditor.ui \
    customWidgets\mapperpathinfo.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
