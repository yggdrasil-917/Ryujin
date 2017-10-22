#pragma once

#include "Profiler.hpp"

#if PLATFORM_WINDOWS
#ifndef PROFILE
#define PROFILE 1
#endif
#include <pix3.h>

#pragma comment(lib, "WinPixEventRuntime.lib")

#define MakePixColor(r, g, b) PIX_COLOR(r, g, b)
#else
#define MakePixColor(r, g, b)
#endif

namespace Ryujin
{
	class CORE_API PixProfiler : public Profiler
	{
#if PLATFORM_WINDOWS
	public:
		void Init() OVERRIDE FINAL;
		void Shutdown() OVERRIDE FINAL;
		void BeginEvent(const ProfileEvent& inEvent) OVERRIDE FINAL;
		void EndEvent(const ProfileEvent& inEvent) OVERRIDE FINAL;

#else
	public:
		void Init() OVERRIDE FINAL {}
		void Shutdown() OVERRIDE FINAL {}
		void BeginEvent(const ProfileEvent& inEvent) OVERRIDE FINAL {}
		void EndEvent(const ProfileEvent& inEvent) OVERRIDE FINAL {}
#endif
	};
}