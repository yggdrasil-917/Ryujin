
#include "Profiler.hpp"


namespace Ryujin
{
	bool Profiler::bIsProfiling = false;
	Profiler* Profiler::instance = nullptr;

	ScopedProfileEvent::ScopedProfileEvent(Profiler* owner, ProfileCategory category, const char* eventName, uint8 r, uint8 g, uint8 b, void* inUserData)
		: profiler(owner)
	{
		if (Profiler::IsProfiling())
		{
			if (profiler->RequiresFullProfileData())
			{
				event.chromeData.timestamp = Time::TimeSinceEpoch();
				strcpy(event.chromeData.name, eventName);
				event.chromeData.profileCategory = category;
				event.chromeData.threadId = Threading::GetThreadID();
			}
			else
			{
				strcpy(event.pixData.name, eventName);
				event.pixData.color[0] = r;
				event.pixData.color[1] = g;
				event.pixData.color[2] = b;
				event.pixData.userData = inUserData;
			}
			profiler->BeginEvent(event);
		}
	}

	ScopedProfileEvent::~ScopedProfileEvent()
	{
		if (Profiler::IsProfiling())
		{
			if (profiler->RequiresFullProfileData())
			{
				event.chromeData.timestamp = Time::TimeSinceEpoch();
			}
			profiler->EndEvent(event);
		}
	}
}