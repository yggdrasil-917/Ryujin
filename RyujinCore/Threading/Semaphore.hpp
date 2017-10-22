
#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP


#include "Thread.hpp"


namespace Ryujin
{
    
#if PLATFORM_WINDOWS
    //---------------------------------------------------------
    // Semaphore (Windows)
    //---------------------------------------------------------
    
//#include <windows.h>
//#undef min
//#undef max
    
    class Semaphore : public NonCopyable
    {
    private:
        HANDLE hSema;
        
    public:
        Semaphore(int32 initialCount = 0)
        {
            RYUJIN_ASSERT(initialCount >= 0, "Semaphore initial count must be positive");
            hSema = CreateSemaphore(NULL, initialCount, MAXLONG, NULL);
        }
        
        ~Semaphore()
        {
            CloseHandle(hSema);
        }
        
        void Wait()
        {
            WaitForSingleObject(hSema, INFINITE);
        }
        
        void Signal(int32 count = 1)
        {
            ReleaseSemaphore(hSema, count, NULL);
        }
    };
    
    
#elif PLATFORM_MAC
    
#include <mach/mach.h>
    
    class Semaphore : public NonCopyable
    {
    private:
        semaphore_t sema;
        
    public:
        Semaphore(int32 initialCount = 0)
        {
            RYUJIN_ASSERT(initialCount >= 0, "Semaphore initial count must be positive");
            semaphore_create(mach_task_self(), &sema, SYNC_POLICY_FIFO, initialCount);
        }
        
        ~Semaphore()
        {
            semaphore_destroy(mach_task_self(), sema);
        }
        
        void Wait()
        {
            semaphore_wait(sema);
        }
        
        void Signal()
        {
            semaphore_signal(sema);
        }
        
        void Signal(int32 count)
        {
            while (count-- > 0)
            {
                semaphore_signal(sema);
            }
        }
    };
    
    
#elif PLATFORM_LINUX
    
#include <semaphore.h>
    
    class Semaphore : public NonCopyable
    {
    private:
        sem_t sema;
        
    public:
        Semaphore(int32 initialCount = 0)
        {
            RYUJIN_ASSERT(initialCount >= 0, "Semaphore initial count must be positive");
            sem_init(&sema, 0, initialCount);
        }
        
        ~Semaphore()
        {
            sem_destroy(&sema);
        }
        
        void Wait()
        {
            // http://stackoverflow.com/questions/2013181/gdb-causes-sem-wait-to-fail-with-eintr-error
            int32 rc;
            do
            {
                rc = sem_wait(&sema);
            } while (rc == -1 && errno == EINTR);
        }
        
        void Signal()
        {
            sem_post(&sema);
        }
        
        void Signal(int32 count)
        {
            while (count-- > 0)
            {
                sem_post(&sema);
            }
        }
    };
    
    
#else
    
#error Unsupported platform!
    
#endif
    
    // Should use this instead of a semaphore directly since it should perform faster
    // as it tries to do less system calls
    // http://preshing.com/20150316/semaphores-are-surprisingly-versatile/
    class AdaptiveSemaphore
    {
    private:
        std::atomic<int32> count;
        Semaphore sema;
        
        void WaitWithPartialSpinning()
        {
            int32 oldCount;
            int32 spin = 10000;
            while (spin--)
            {
                oldCount = count.load(std::memory_order_relaxed);
                if ((oldCount > 0) && count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire))
                    return;
                
                std::atomic_signal_fence(std::memory_order_acquire);     // Prevent the compiler from collapsing the loop.
            }
            
            oldCount = count.fetch_sub(1, std::memory_order_acquire);
            if (oldCount <= 0)
            {
                sema.Wait();
            }
        }
        
    public:
        AdaptiveSemaphore(int32 initialCount = 0) : count(initialCount)
        {
            RYUJIN_ASSERT(initialCount >= 0, "Semaphore initial count must be positive");
        }
        
        bool TryWait()
        {
            int32 oldCount = count.load(std::memory_order_relaxed);
            return (oldCount > 0 && count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire));
        }
        
        void Wait()
        {
            if (!TryWait())
                WaitWithPartialSpinning();
        }
        
        void Signal(int32 initialCount = 1)
        {
            int32 oldCount = count.fetch_add(initialCount, std::memory_order_release);
            int32 toRelease = -oldCount < initialCount ? -oldCount : initialCount;
            
            if (toRelease > 0)
            {
                sema.Signal(toRelease);
            }
        }
    };
}


#endif