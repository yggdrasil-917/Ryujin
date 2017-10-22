
#ifndef PROFILER_HPP
#define PROFILER_HPP


#include "../Containers/String.hpp"

namespace Ryujin
{
    // NOTE: make sure to add a string entry to categories below if a new enum is added below
    enum ProfileCategory
    {
        PC_Rendering = 0,
		PC_Engine,
        PC_Cooker,
        PC_Other,
        
        PC_Count
    };
    
    static const char* categories[] =
    {
        "Rendering",
		"Engine",
        "Cooker",
        "Other"
	};

	struct ProfileEvent
	{
		struct ChromeData 
		{
			uint64 timestamp;
			uint64 id; // only used for async events
			uint64 threadId;
			char name[64];
			ProfileCategory profileCategory;
			void* userData;
		};

		struct PixData
		{
			char name[64];
			uint8 color[3];
			void* userData;
		};

		union
		{
			ChromeData chromeData;
			PixData pixData;
		};
	};

	class CORE_API Profiler
	{
	private:
		static bool bIsProfiling;

	protected:
		static Profiler* instance;

	public:
		VIRTUAL ~Profiler() {}

		VIRTUAL void Init()
		{
			bIsProfiling = true;
		}

		VIRTUAL void Shutdown()
		{
			bIsProfiling = false;
		}
		
		VIRTUAL void BeginEvent(const ProfileEvent& inEvent) {}
		VIRTUAL void EndEvent(const ProfileEvent& inEvent) {}

		VIRTUAL void SaveProfileData() {}

		VIRTUAL bool RequiresFullProfileData() const { return false; }

		static bool IsProfiling() { return bIsProfiling; }
		static Profiler* Instance() { return instance; }
	};
    
    enum class ProfilerType : uint8
    {
        PT_Chrome,
        PT_Pix,
        PT_None
    };
    
    class CORE_API ScopedProfileEvent
    {
    private:
        ProfileEvent event;
        Profiler* profiler;
        
    public:
		ScopedProfileEvent(Profiler* owner, ProfileCategory category, const char* eventName, uint8 r, uint8 g, uint8 b, void* inUserData = nullptr);
		~ScopedProfileEvent();
    };
    
    
#define SCOPED_PROFILE_EVENT(name, category, r, g, b)	ScopedProfileEvent name##_event(Profiler::Instance(), category, STRINGIZE(name), r,g,b); 

    
}



#endif
