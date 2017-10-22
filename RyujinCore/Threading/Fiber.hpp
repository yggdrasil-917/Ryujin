
#pragma once

#include "../OS/Platform.hpp"

#if PLATFORM_MAC
#define _XOPEN_SOURCE 800

#ifdef _FORTIFY_SOURCE
#undef _FORTIFY_SOURCE
#endif
#endif

#if PLATFORM_MAC 
#include <Foundation/Foundation.h>
#elif PLATFORM_LINUX
#include <ucontext.h>
#endif

#include <functional>

namespace Ryujin
{
#if PLATFORM_WINDOWS
#define CoroutineStackSize (64 * 1024)
#else
#define CoroutineStackSize (64 * 1024)
#endif
    
    
#if PLATFORM_MAC
#define persist __block
#else
#define persist
#endif


	class CORE_API Fiber
	{
	public:
		Fiber();
		Fiber(uint32 stackSize);
		~Fiber();

		void* Main();
		bool Resume();

		static void* EnableFibersInCurrentThread() 
		{
#if PLATFORM_WINDOWS
			return ::ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);
#else
			return nullptr;
#endif
		}
		static bool DisableFibersInCurrentThread() 
		{
#if PLATFORM_WINDOWS
			return (FALSE != ::ConvertFiberToThread());
#else
			return true;
#endif
		}

		enum class FiberState
		{
			FS_Created,
			FS_Running,
			FS_StopPending,
			FS_Stopped,
		};

	protected:
		VIRTUAL void Run() = 0;
		void FiberYield(bool bShouldContinue = true);
        void WaitForSeconds(float secondsToWait);

		FiberState state;
        uint64 timestamp;
#if PLATFORM_WINDOWS
		void* fiber;
		void* previousFiber;
#elif PLATFORM_LINUX
		ucontext_t  fiber;
		ucontext_t  previousFiber;
		uint8*		stack;
#endif

	private:
#if PLATFORM_LINUX
        static void FiberStartFnc(void* fiberParams);
#else
		static void RYUJIN_CALLBACK FiberStartFnc(void* fiberParams);
#endif
	};
    
    
    struct StacklessCoroutine
    {
        StacklessCoroutine()
        : state(0)
        , loop(false)
        , done(false)
        {
        }
        
        void Reset()
        {
            state = 0;
            loop = true;
            done = false;
        }
        
        VIRTUAL void Main() = 0;
        
        uint32 state;
        bool loop;
        bool done;
    };
    
#define STACKLESS_COROUTINE_START() \
  switch (state)             \
  {                                    \
  case 0:                              \
    do                                 \
    {
    
#define STACKLESS_COROUTINE_YIELD() \
        do                               \
        {                                \
            state = __LINE__;      \
            return;                        \
            case __LINE__:;                \
        } while (0)
    
#define STACKLESS_COROUTINE_END() \
        if (loop)            \
        {                              \
            Reset();            \
        }                              \
        else                           \
        {                              \
            done = true;         \
            state = __LINE__;    \
            return; case __LINE__:;      \
        }                              \
    } while (0);                     \
  }
}
