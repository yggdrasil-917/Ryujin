#ifndef PLATFORM_HPP
#define PLATFORM_HPP


#if _WIN32 || _WIN64

//#include <SDKDDKVer.h>
//
//#ifndef WIN32_LEAN_AND_MEAN
//	#define WIN32_LEAN_AND_MEAN 1
//#endif
//	#include <windows.h>

#else
	#include <sys/time.h>
#if defined(LINUX)
    //#include <X11/keysym.h>
	#include <stdio.h>
    #include <unistd.h>
    #include <assert.h>
    #include <sys/stat.h>
    #include <fcntl.h>

    #define __stdcall

#elif defined(__APPLE__)

#ifdef __OBJC__
    #import <Foundation/Foundation.h>
    #import <Cocoa/Cocoa.h>
#endif

#define __stdcall

#endif

#endif // _WIN32


#include "../Logger/Logger.hpp"

#include <limits>

#define STATS_ENABLED 1

#define RYUJIN_CALLBACK __stdcall

namespace Ryujin
{
	void CORE_API ErrorMsg(const char *string);
	void CORE_API WarningMsg(const char *string);
	void CORE_API InfoMsg(const char *string);
    
#if PLATFORM_MAC
    void MessageBoxOK(const char* header, const char* msg);
    int32 BlockingAlert(const char* title, const char* message, const char* button0, const char* button1 = nullptr, const char* button2 = nullptr);
#endif
    
#if PLATFORM_WINDOWS
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
#endif

    template<typename T>
    struct NumericLimit
    {
        static T Min() { return std::numeric_limits<T>::min(); }
        static T Max() { return std::numeric_limits<T>::max(); }
        static T Infinity() { return std::numeric_limits<T>::infinity(); }
    };

#if PLATFORM_WINDOWS
#ifndef min
	#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
	#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#endif
    
    
    template < typename T, size_t N >
    PtrSize countof( T ( & arr )[ N ] )
    {
        return std::extent< T[ N ] >::value;
    }
    
    template<PtrSize N1 = 16, PtrSize N2 = 16>
    struct Handle
    {
        uint32 index        : N1;
        uint32 generation   : N2;
        
        FORCEINLINE Handle operator=(const uint32 other)
        {
            index = other & ((1 << N1) - 1);
            generation = (other >> N1) & ((1 << N2) - 1);
            return *this;
        }
        
        FORCEINLINE operator uint32() const
        {
            return generation << (32 - N2) | index;
        }
    };
    
    template<PtrSize N1 = 16, PtrSize N2 = 16>
    static Handle<N1, N2> CreateInvalidHandle()
    {
        Handle<N1, N2> invalidHandle;
        uint32 h = (uint32)-1;
        invalidHandle.index = h;
        invalidHandle.generation = h;
        
        return invalidHandle;
    }
    
    template<typename T = uint32, PtrSize N1 = 16, PtrSize N2 = 12, PtrSize N3 = 4>
    struct GenericHandle
    {
        T index         : N1;
        T generation    : N2;
        T type          : N3; // useful for when handles are used to store heterogenous data
        
        FORCEINLINE operator T() const
        {
            const T typeBitPos = sizeof(T) * 8 - N3;
            const T generationBitPos = typeBitPos - N2;
            return type << typeBitPos | generation << generationBitPos | index;
        }
    };
    
    
    static const char* GetPlatformString()
    {
#if PLATFORM_MAC
        return "Mac";
#elif PLATFORM_WINDOWS
        return "Windows";
#elif PLATFORM_LINUX
        return "Linux";
#else
        return "Unknown";
#endif
    }
    
    
#if PLATFORM_MAC
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
    
    // https://developer.apple.com/library/mac/qa/qa1361/_index.html
    static bool AmIBeingDebugged(void)
    // Returns true if the current process is being debugged (either
    // running under the debugger or has a debugger attached post facto).
    {
        int                 junk;
        int                 mib[4];
        struct kinfo_proc   info;
        size_t              size;
        
        // Initialize the flags so that, if sysctl fails for some bizarre
        // reason, we get a predictable result.
        
        info.kp_proc.p_flag = 0;
        
        // Initialize mib, which tells sysctl the info we want, in this case
        // we're looking for information about a specific process ID.
        
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PID;
        mib[3] = getpid();
        
        // Call sysctl.
        
        size = sizeof(info);
        junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
        assert(junk == 0);
        
        // We're being debugged if the P_TRACED flag is set.
        
        return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
    }
#elif PLATFORM_LINUX
    //extern CORE_API int32 IsDebuggerPresent(void)
    // http://stackoverflow.com/questions/3596781/how-to-detect-if-the-current-process-is-being-run-by-gdb
    static int32 IsDebuggerPresent(void)
    {
        int32 rc = 0;
        FILE *fd = fopen("/tmp", "r");

        if (fileno(fd) > 5)
        {
            rc = 1;
        }

        fclose(fd);
        return rc;
    }
#endif

    FORCEINLINE bool IsDebuggerConnected()
    {
#if PLATFORM_WINDOWS
        return IsDebuggerPresent() != 0;
#elif PLATFORM_MAC
        return AmIBeingDebugged();
#elif PLATFORM_LINUX
        extern int32 IsDebuggerPresent(void);
        return IsDebuggerPresent();
#else
        return false;
#endif
    }
    
    static void DoDebugBreak()
    {
#if PLATFORM_WINDOWS
        __debugbreak();
#elif PLATFORM_MAC
        __asm__("int $3");
#else
        assert(0);
#endif
	}

#if !PLATFORM_WINDOWS
#define __analysis_assume(x)
#endif

#define RYUJIN_ASSERT(expression, msg) __analysis_assume(expression); if (!(expression)) { Errorf(Info, "\nfile: %s\nline: %d\nerror: %s", __FILE__, __LINE__, msg); if (IsDebuggerConnected()) DoDebugBreak(); }

#define RYUJIN_STATIC_ASSERT(cond, msg) static_assert((cond), msg)

#define WARN_ONCE_IF(expression, ...) \
	{ \
		__analysis_assume(expression); \
		static bool triggeredWarning = false; \
		if ((bool)(expression) && !triggeredWarning) { \
			triggeredWarning = true; \
			Warnf(Info, "%s, %s", #expression, __VA_ARGS__); \
		} \
	}

#define WARN_ONCE_IF_NOT( isTrue, ... ) WARN_ONCE_IF(!(isTrue), __VA_ARGS__)
    
#if PLATFORM_WINDOWS
#define STRUCT_ALIGN(structName, structContent, alignment) __declspec(align(alignment)) struct structName { structContent }
    
#else
#define STRUCT_ALIGN(structName, structContent, alignment) struct structName { structContent } __attribute__((aligned(16)))
    
#endif
}


#endif
