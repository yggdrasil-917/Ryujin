
#pragma once

#if _WIN32 || _WIN64

#include <SDKDDKVer.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#define NOMAXMIN
#include <windows.h>

#endif

#if !_WIN32 && !_WIN64
#include <cstdio>
#include <cstdint>
#include <sys/time.h>
#endif

#include <sstream>


namespace Ryujin
{
    // Define some sized types
    using uint8     = uint8_t;
    using int8      = int8_t;
    using uint16    = uint16_t;
    using int16     = int16_t;
    using uint32    = uint32_t;
    using int32     = int32_t;
    using uint64    = uint64_t;
    using int64     = int64_t;
    using intptr    = intptr_t;
    using uintptr   = uintptr_t;
    
#if _WIN32 || _WIN64

    using timestamp = LARGE_INTEGER;
#else

    using timestamp = timeval;
#endif
    
    using PtrSize   = size_t;
    using ubyte     = uint8_t;
    using byte      = int8_t;
    
#if USE_DOUBLE_PRECISION_MATH
    using real = double;
#else
    using real = float;
#endif

#if _WIN32 || _WIN64
	using LibModule = HMODULE;
#else
	using LibModule = void*;
#endif
    
#include "Macros.hpp"
    
}

#include "../CoreUtils/SmartEnum.hpp"
