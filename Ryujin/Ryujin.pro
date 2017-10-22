TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
#QMAKE_CXXFLAGS += -v
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-function -Wno-ignored-qualifiers -Wno-sign-compare -Wno-switch -fno-strict-aliasing -Wno-unused-variable -Wno-unused-result

INCLUDEPATH += $$_PRO_FILE_PWD_/../ThirdParty/tbb/include \
    $$PWD/../ThirdParty/vulkan-1.0.21 \
    $$_PRO_FILE_PWD_/..


#LIBS += $$PWD/../ThirdParty/tbb/lib/linux
LIBS += -lpthread -Wl,--no-as-needed -ldl
LIBS += -L$$PWD/../ThirdParty/vulkan-1.0.21/lib/linux/lib -lvulkan
QMAKE_LFLAGS += -Wl,-rpath=$$PWD/../ThirdParty/vulkan-1.0.21/lib/linux/lib

CONFIG(debug, debug|release) {
    debug:OBJECTS_DIR = $$PWD/../_Intermediate/Ryujin/linux/Debug
    debug:DESTDIR = $$PWD/../_Build/linux/Debug
    debug:LIBS += -L$$PWD/../_Build/linux/Debug -lRyujinCore
    debug:LIBS += -L$$PWD/../_Build/linux/Debug -lRyujinRenderer
    QMAKE_LFLAGS += -Wl,-rpath=$$PWD/../_Build/linux/Debug # Embed the .so path into the exe so it finds it at runtime
}

CONFIG(release, debug|release) {
    release:OBJECTS_DIR = $$PWD/../_Intermediate/Ryujin/linux/Release
    release:DESTDIR = $$PWD/../_Build/linux/Release
    release:LIBS += -L$$PWD/../_Build/linux/Release -lRyujinCore
    release:LIBS += -L$$PWD/../_Build/linux/Release -lRyujinRenderer
    QMAKE_LFLAGS += -Wl,-rpath=$$PWD/../_Build/linux/Release # Embed the .so path into the exe so it finds it at runtime
}

SOURCES +=

#include(../RyujinCore/RyujinCore.pro)
include(IO/IO.pri)
include(ECS/ECS.pri)
include(ResourceManagers/ResourceManagers.pri)
include(SceneManagement/SceneManagement.pri)
include(Ryujin/Ryujin.pri)


