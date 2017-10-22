#include "ChromeProfiler.hpp"

#include "../OS/Application.hpp"
#include "../OS/FileSystem.hpp"

namespace Ryujin
{
	// https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview#heading=h.lc5airzennvk
	static const char* tracePhaseName[] =
	{
		"B",
		"E",
		"I",
		"b",
		"e"
	};

	ChromeProfiler::ChromeProfiler()
	{

	}

	ChromeProfiler::~ChromeProfiler()
	{

	}

	void ChromeProfiler::Init()
	{
		Profiler::Init();

		instance = this;
	}

	void ChromeProfiler::Shutdown()
	{
		if (eventList.Size() > 0)
		{
			SaveProfileData();
		}

		Profiler::Shutdown();
	}

	void ChromeProfiler::BeginEvent(const ProfileEvent& inEvent)
	{
		ChromeTracingEvent(inEvent, TP_Begin);
	}

	void ChromeProfiler::EndEvent(const ProfileEvent& inEvent)
	{
		ChromeTracingEvent(inEvent, TP_End);
	}

	void ChromeProfiler::ChromeTracingEvent(const ProfileEvent& event, TracePhase tracePhase, JsonObject* metadata)
	{
		JsonObject object;
		object["cat"] = RYUJIN_NEW JsonValue(categories[event.chromeData.profileCategory]);
		object["pid"] = RYUJIN_NEW JsonValue((PtrSize)AppInfo::GetPID());
		object["tid"] = RYUJIN_NEW JsonValue(event.chromeData.threadId);
		if (tracePhase == TP_Begin || tracePhase == TP_AsyncStart)
		{
			object["ts"] = RYUJIN_NEW JsonValue(event.chromeData.timestamp);
		}
		else if (tracePhase == TP_End || tracePhase == TP_AsyncFinish)
		{
			object["ts"] = RYUJIN_NEW JsonValue(event.chromeData.timestamp);
		}
		if (tracePhase == TP_AsyncStart || tracePhase == TP_AsyncFinish)
		{
			object["id"] = RYUJIN_NEW JsonValue(event.chromeData.id);
		}
		object["ph"] = RYUJIN_NEW JsonValue(tracePhaseName[tracePhase]);
		object["name"] = RYUJIN_NEW JsonValue(event.chromeData.name);

		if (metadata)
		{
			object["args"] = RYUJIN_NEW JsonValue(*metadata);
		}

		ScopedLock lock(mtx);
		eventList.Add(RYUJIN_NEW JsonValue(object));
	}


	void ChromeProfiler::SaveProfileData()
	{
		const char* profilingDir = AppInfo::GetProfilingDir();
		if (!Directory::DirectoryExists(profilingDir))
		{
			Directory::CreateDirectory(profilingDir);
		}
		String filepath = String::Printf("%s%sProfile_%s.json", profilingDir, AppInfo::GetExeName(), *String::CurrentDate());
		FileHandle handle = File::Open(*filepath, FileMode::FM_Write, false);

		if (!handle)
		{
			Warnf(Profiler, "Failed to save profiling data");
		}
		else
		{
			JsonObject object;
			JsonArray eventArray;
			eventArray.AddZeroed(eventList.Size());
			uint32 i = eventArray.GetSize() - 1;
			JsonValue* it;
			if (eventList.GetFirst(it))
			{
				do
				{
					eventArray[i--] = it;
				} while (eventList.GetNext(it));
			}

			object["traceEvents"] = RYUJIN_NEW JsonValue(eventArray);
			JsonValue* root = RYUJIN_NEW JsonValue(object);
			String str = root->Stringify(0);

			File::Write(*str, sizeof(char), str.Length(), handle);
			File::Close(handle);
			FREE_OBJECT(object);

//			Debugf(Profiler, "Saved profiling data to %s", *filepath); // SmartEnum ToString() causing memory crash
		}

		JsonValue* it;
		if (eventList.GetFirst(it))
		{
			do
			{
				delete it;
			} while (eventList.GetNext(it));
		}
		eventList.RemoveAll();
	}
}
