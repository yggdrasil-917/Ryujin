#-------------------------------------------------
#
# Project created by QtCreator 2016-08-20T20:22:08
#
#-------------------------------------------------

QT       -= core gui

TARGET = RyujinCore
TEMPLATE = lib

INCLUDEPATH += $$PWD/../ThirdParty/tbb/include \
    $$PWD/..

#LIBS += $$PWD/../ThirdParty/tbb/lib/linux
LIBS += -L/usr/lib/x86_64-linux-gnu -lxcb

DEFINES += RYUJINCORE_EXPORTS \
            LINUX

CONFIG += c++11
#QMAKE_CXXFLAGS += -v
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-function -Wno-ignored-qualifiers -Wno-sign-compare -Wno-switch -fno-strict-aliasing -Wno-unused-variable -Wno-unused-result


unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug, debug|release) {
    debug:OBJECTS_DIR = $$PWD/../_Intermediate/RyujinCore/linux/Debug
}

CONFIG(release, debug|release) {
    release:OBJECTS_DIR = $$PWD/../_Intermediate/RyujinCore/linux/Release
}

include(OS/OS.pri)
include(CoreUtils/CoreUtils.pri)
include(Logger/Logger.pri)
include(Containers/Containers.pri)
include(Allocators/Allocators.pri)
include(Math/Math.pri)
include(Threading/Threading.pri)
include(Serialization/Serialization.pri)
include(Profiler/Profiler.pri)
include(Resource/Resource.pri)
include(Window/Window.pri)
include(Input/Input.pri)

HEADERS +=

SOURCES +=




