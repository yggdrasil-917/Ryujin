#pragma once

#include "Profiler.hpp"
#include "../Containers/List.hpp"
#include "../Threading/Thread.hpp"
#include "../CoreUtils/Timer.hpp"

#include "../CoreUtils/Json.hpp"


namespace Ryujin
{
	struct ProfileEvent;

	enum TracePhase
	{
		TP_Begin = 0,
		TP_End,
		TP_Instant,
		TP_AsyncStart,
		TP_AsyncFinish
	};

	class CORE_API ChromeProfiler : public Profiler
	{
	private:
		List<JsonValue*> eventList;
		Mutex mtx;

		void ChromeTracingEvent(const ProfileEvent& event, TracePhase tracePhase, JsonObject* metadata = nullptr);

	public:
		ChromeProfiler();
		~ChromeProfiler();
        
        bool RequiresFullProfileData() const OVERRIDE FINAL { return true; }

		void Init() OVERRIDE FINAL;
		void Shutdown() OVERRIDE FINAL;
		void BeginEvent(const ProfileEvent& inEvent) OVERRIDE FINAL;
		void EndEvent(const ProfileEvent& inEvent) OVERRIDE FINAL;

		void SaveProfileData() OVERRIDE FINAL;
	};
}
