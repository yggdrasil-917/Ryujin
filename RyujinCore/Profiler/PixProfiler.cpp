#include "PixProfiler.hpp"


#if PLATFORM_WINDOWS

namespace Ryujin
{
	void PixProfiler::Init()
	{
		Profiler::Init();

		instance = this;
	}

	void PixProfiler::Shutdown()
	{
		Profiler::Shutdown();
	}

	void PixProfiler::BeginEvent(const ProfileEvent& inEvent)
	{
		uint64 color = PIX_COLOR(inEvent.pixData.color[0],
								inEvent.pixData.color[1],
								inEvent.pixData.color[2]);
		PIXBeginEvent(color, inEvent.pixData.name);
	}

	void PixProfiler::EndEvent(const ProfileEvent& inEvent)
	{
		PIXEndEvent();
	}
}

#endif