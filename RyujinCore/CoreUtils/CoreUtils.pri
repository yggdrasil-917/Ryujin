HEADERS += \
    CoreUtils/CRC.hpp \
    CoreUtils/ServiceLocator.hpp \
    CoreUtils/Singleton.hpp \
    CoreUtils/SmartEnum.hpp \
    CoreUtils/BinaryReader.hpp \
    CoreUtils/BinaryWriter.hpp \
    CoreUtils/Common.hpp \
    CoreUtils/ConfigCache.hpp \
    CoreUtils/SmartPointer.hpp \
    CoreUtils/Timer.hpp \
    CoreUtils/Json.hpp \
    CoreUtils/Delegate.hpp \
    CoreUtils/ICallback.hpp \
    CoreUtils/MethodCallback.hpp \
    CoreUtils/ObjectListener.hpp

SOURCES += \
    CoreUtils/ServiceLocator.cpp \
    CoreUtils/BinaryReader.cpp \
    CoreUtils/BinaryWriter.cpp \
    CoreUtils/ConfigCache.cpp \
    CoreUtils/Timer.cpp \
    CoreUtils/Json.cpp

CONFIG += c++11
