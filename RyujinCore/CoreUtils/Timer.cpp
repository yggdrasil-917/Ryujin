#include "Timer.hpp"


namespace Ryujin
{
    Timer::Timer(bool startNow)
    {
        bIsStopped = false;
        
        if (startNow)
        {
            Start();
        }
    }
    
    void Timer::Start()
    {
        start = std::chrono::high_resolution_clock::now();
        bIsStopped = false;
    }
    
    void Timer::Stop()
    {
        end = std::chrono::high_resolution_clock::now();
        bIsStopped = true;
    }
    
    TimeResult Timer::ElapsedSeconds(bool bUpdateStart)
    {
        if (!bIsStopped)
        {
            end = std::chrono::high_resolution_clock::now();
        }
        auto t = end - start;
        if (bUpdateStart && !bIsStopped)
            start = end;
        return std::chrono::duration_cast<seconds>(t).count();
    }
    
    TimeResult Timer::ElapsedMilliseconds(bool bUpdateStart)
    {
        if (!bIsStopped)
        {
            end = std::chrono::high_resolution_clock::now();
        }
        auto t = end - start;
        if (bUpdateStart && !bIsStopped)
            start = end;
        return std::chrono::duration_cast<milliseconds>(t).count();
    }
    
    TimeResult Timer::ElapsedNanoseconds(bool bUpdateStart)
    {
        if (!bIsStopped)
        {
            end = std::chrono::high_resolution_clock::now();
        }
        auto t = end - start;
        if (bUpdateStart && !bIsStopped)
            start = end;
        return std::chrono::duration_cast<nanoseconds>(t).count();
    }
}







