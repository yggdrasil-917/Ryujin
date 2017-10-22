
DEFINES += LINUX

CONFIG += c++11

HEADERS += \
    OS/Macros.hpp \
    OS/Platform.hpp \
    OS/Types.hpp \
    OS/Application.hpp \
    OS/FileSystem.hpp \
    OS/Process.hpp \
    OS/StackTrace.hpp

SOURCES += \
    OS/ApplicationLinux.cpp \
    OS/Platform.cpp \
    OS/ProcessLinux.cpp
