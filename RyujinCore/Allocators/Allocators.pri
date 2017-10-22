HEADERS += \
    Allocators/Memory.hpp \
    Allocators/Allocator.hpp \
    Allocators/AnsiAllocator.hpp \
    Allocators/MemoryTrackingPolicies.hpp \
    Allocators/GlobalAllocators.hpp


CONFIG += c++11

SOURCES += \
    Allocators/Allocator.cpp \
    Allocators/GlobalAllocators.cpp
