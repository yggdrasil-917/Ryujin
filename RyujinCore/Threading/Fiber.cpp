

#include "Fiber.hpp"
#include "../Allocators/GlobalAllocators.hpp"

#if PLATFORM_MAC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace Ryujin
{
	Fiber::Fiber()
		: state(FiberState::FS_Created),
            timestamp(0)
#if PLATFORM_WINDOWS
		  , previousFiber(nullptr)
#endif
	{
#if PLATFORM_WINDOWS
		fiber = ::CreateFiber(CoroutineStackSize, FiberStartFnc, this);
#elif PLATFORM_LINUX
        stack = RYUJIN_NEW uint8[CoroutineStackSize];
        getcontext(&fiber);
        fiber.uc_stack.ss_sp = stack;
        fiber.uc_stack.ss_size = CoroutineStackSize;
        fiber.uc_link = &previousFiber;
        makecontext(&fiber, (void(*)())FiberStartFnc, 1, this);
#endif
	}

	Fiber::Fiber(uint32 stackSize)
        : state(FiberState::FS_Created),
            timestamp(0)
#if PLATFORM_WINDOWS
		  , previousFiber(nullptr)
#endif
	{
#if PLATFORM_WINDOWS
		fiber = ::CreateFiber(stackSize, FiberStartFnc, this);
#elif PLATFORM_LINUX
		stack = RYUJIN_NEW uint8[stackSize];
		getcontext(&fiber);
		fiber.uc_stack.ss_sp = stack;
		fiber.uc_stack.ss_size = stackSize;
		fiber.uc_link = &previousFiber;
        makecontext(&fiber, (void(*)())FiberStartFnc, 1, this);
#endif
	}

	Fiber::~Fiber()
	{
		if (state == FiberState::FS_Running)
		{
			state = FiberState::FS_StopPending;
#if PLATFORM_WINDOWS
			previousFiber = ::GetCurrentFiber();
			::SwitchToFiber(fiber);
#elif PLATFORM_LINUX
			swapcontext(&previousFiber, &fiber);
#endif
		}

#if PLATFORM_WINDOWS
		::DeleteFiber(fiber);
#elif PLATFORM_LINUX
        SAFE_DELETE_ARRAY(stack);
#endif
	}

	bool Fiber::Resume()
	{
#if PLATFORM_MAC
        Warnf(Info, "Attempting to use coroutine on Mac, which does not work!!!");
        state = FiberState::FS_Stopped;
#endif
        if (state == FiberState::FS_Stopped)
        {
            return false;
        }
        
#if PLATFORM_WINDOWS
        if (fiber == nullptr)
        {
            return false;
        }
        
        if (previousFiber == nullptr)
        {
            previousFiber = ::GetCurrentFiber();
        }
        RYUJIN_ASSERT(previousFiber != fiber, "previousFiber != fiber");
        
        ::SwitchToFiber(fiber);
#elif PLATFORM_LINUX
        swapcontext(&previousFiber, &fiber);
#endif

		return state == FiberState::FS_Running;
	}

	void RYUJIN_CALLBACK Fiber::FiberStartFnc(void* fiberParams)
	{
#if !PLATFORM_MAC
		Fiber* _fiber = (Fiber*)fiberParams;
		void* _previousFiber = _fiber->Main();
    #if PLATFORM_WINDOWS
		::SwitchToFiber(_previousFiber);
    #else
        const ucontext_t& prevFiber = *(ucontext_t*)_previousFiber;
		swapcontext(&_fiber->fiber, &prevFiber);
    #endif
#endif
	}

	void Fiber::FiberYield(bool bShouldContinue /* = true */)
	{
#if PLATFORM_MAC
        return;
#else
		if (!bShouldContinue)
		{
			state = FiberState::FS_Stopped; // like C#'s yield break
		}

    #if PLATFORM_WINDOWS
		::SwitchToFiber(previousFiber);
    #elif PLATFORM_LINUX
		swapcontext(&fiber, &previousFiber);
    #endif
#endif
	}
    
    void Fiber::WaitForSeconds(float secondsToWait)
    {
        if (timestamp == 0)
        {
            timestamp = Time::TimeSinceEpoch();
        }
        
        while (float(Time::TimeSinceEpoch() - timestamp) * 0.001f < secondsToWait)
        {
            FiberYield();
        }
        
        timestamp = 0;
    }

	void* Fiber::Main()
	{
		state = FiberState::FS_Running;

		Run();

		state = FiberState::FS_Stopped;
#if PLATFORM_WINDOWS
		return previousFiber;
#elif PLATFORM_LINUX
        return &previousFiber;
#else
        return nullptr;
#endif
	}
}

#if PLATFORM_MAC
#pragma GCC diagnostic pop
#endif
