QT       += core gui qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets qml

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cell.cpp \
    controlmodel.cpp \
    cordsystem.cpp \
    main.cpp \
    storage.cpp

HEADERS += \
    cell.h \
    controlmodel.h \
    cordsystem.h \
    storage.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    main.qml

RESOURCES += \
    resources.qrc
