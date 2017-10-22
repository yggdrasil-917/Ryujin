#pragma once


#include "RyujinCore/Logger/Logger.hpp"


#if PLATFORM_WINDOWS

#if RYUJIN_EXTENSIONS_EXPORT
#define EXTENSIONS_API __declspec(dllexport)
#else
#define EXTENSIONS_API __declspec(dllimport)
#endif

#elif PLATFORM_MAC
#define EXTENSIONS_API __attribute__ ((visibility ("default")))

#else
#define EXTENSIONS_API __attribute__ ((visibility ("default")))

#endif
