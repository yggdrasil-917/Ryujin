
#ifndef APPLICATION_HPP
#define APPLICATION_HPP


//#include "../Logger/Logger.hpp"
#include "../Containers/String.hpp"

#if PLATFORM_WINDOWS
#include <process.h>
#elif PLATFORM_LINUX
#include <sys/types.h>
#include <unistd.h>
#endif

namespace Ryujin
{
#if PLATFORM_WINDOWS
	using pid_t = int32;
#endif

    class CORE_API AppInfo
    {
    private:
        static String* appDir;
        static String* exeDir;
        static String* resourceDir;
        static String* cookedDir;
        static String* exeName;
        static String* profilingDir;
        static String* appDataDir;
        
    public:
        static const char* GetApplicationDir();
        
        static const char* GetExeDir();
        
        static const char* GetResourcesDir();
        
        static const char* GetCookedContentDir();
        
        static const char* GetDevCookedContentDir();
        
        static const char* GetExeName();
        
        static const char* GetProfilingDir();
        
        static const char* GetAppDataDir();
        
#if PLATFORM_WINDOWS
		static pid_t GetPID()
		{
			return _getpid();
		}
#else
        static pid_t GetPID()
        {
            return getpid();
        }
#endif
    };
}


#endif
