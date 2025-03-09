QT       += core gui
QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/jsondatabridge.cpp \
    delegates/pathinfotabledelegate.cpp \
    main.cpp                             \
    archway.cpp                          \
    core\pathinfo.cpp                    \
    customWidgets\crudcontrols.cpp       \
    customWidgets\groupeditor.cpp        \
    customWidgets\mapperpathinfo.cpp     \
    customWidgets\statechip.cpp          \
    customWidgets\viewtogglesw.cpp       \
    customWidgets\togglechip.cpp         \
    delegates\groupslistdelegate.cpp     \
    delegates\toggleswitchdelegate.cpp   \
    dialogs\addrenamedialog.cpp          \
    dialogs\pathtypeselectordialog.cpp   \
    models\groupslistmodel.cpp           \
    models\pathsinfotablemodel.cpp       \

HEADERS += \
    archway.h                          \
    core\jsondatabridge.h              \
    core\buttonStyle.h                 \
    core\pathinfo.h                    \
    customWidgets\crudcontrols.h       \
    customWidgets\groupeditor.h        \
    customWidgets\mapperpathinfo.h     \
    customWidgets\statechip.h          \
    customWidgets\viewtogglesw.h       \
    customWidgets\togglechip.h         \
    delegates/pathinfotabledelegate.h \
    delegates\groupslistdelegate.h     \
    delegates\toggleswitchdelegate.h   \
    dialogs\addrenamedialog.h          \
    dialogs\pathtypeselectordialog.h   \
    models\groupslistmodel.h           \
    models\pathsinfotablemodel.h       \

FORMS += \
    archway.ui                        \
    customWidgets\crudcontrols.ui     \
    customWidgets\groupeditor.ui      \
    customWidgets\mapperpathinfo.ui   \
    dialogs\addrenamedialog.ui        \
    dialogs\pathtypeselectordialog.ui \

RC_ICONS = ArchWay_icon_256x256.ico


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc \
