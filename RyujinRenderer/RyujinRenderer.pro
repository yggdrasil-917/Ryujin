QT       -= core gui

TARGET = RyujinRenderer
TEMPLATE = lib

INCLUDEPATH += $$PWD/../ThirdParty/tbb/include \
    $$PWD/../ThirdParty/stb \
    $$PWD/../ThirdParty/vulkan-1.0.21 \
    $$PWD/..

#LIBS += $$PWD/../ThirdParty/tbb/lib/linux
LIBS += -L/usr/lib/x86_64-linux-gnu -lxcb

DEFINES += RYUJIN_RENDERER_EXPORT \
            LINUX

CONFIG += c++11
#QMAKE_CXXFLAGS += -v
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-function -Wno-ignored-qualifiers -Wno-sign-compare -Wno-switch -fno-strict-aliasing -Wno-unused-variable -Wno-unused-result


unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG(debug, debug|release) {
    debug:OBJECTS_DIR = $$PWD/../_Intermediate/RyujinRenderer/linux/Debug
    debug:DESTDIR = $$PWD/../_Build/linux/Debug
}

CONFIG(release, debug|release) {
    release:OBJECTS_DIR = $$PWD/../_Intermediate/RyujinRenderer/linux/Release
    debug:DESTDIR = $$PWD/../_Build/linux/Release
}
include(Vulkan/Vulkan.pri)
include(Renderer/Renderer.pri)
include(RenderUtils/RenderUtils.pri)
