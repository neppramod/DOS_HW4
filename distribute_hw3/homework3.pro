TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -pthread

SOURCES += main.cpp \
    communication.cpp \
    client.cpp \
    utility.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    communication.h \
    message.h \
    client.h \
    constants.h \
    neighbors.h \
    stringtok.h \
    utility.h

OTHER_FILES += \
    portnums \
    ../build-homework3-Desktop-Debug/neighbors.txt

