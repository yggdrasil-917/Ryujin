#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include "../OS/Types.hpp"

template<typename T>
using seconds_t = std::chrono::duration<T, std::ratio<1, 1> >;

template<typename T>
using milliseconds_t = std::chrono::duration<T, std::milli >;

template<typename T>
using nanoseconds_t = std::chrono::duration<T, std::nano >;

#if USE_HIGH_PRECISION_TIMER
using seconds = seconds_t<double>;
using milliseconds = milliseconds_t<double>;
using nanoseconds = nanoseconds_t<double>;
using TimeResult = double;

#else
using seconds = seconds_t<float>;
using milliseconds = milliseconds_t<float>;
using nanoseconds = nanoseconds_t<float>;
using TimeResult = float;

#endif

namespace Ryujin
{
	class CORE_API Timer
	{
	private:
		std::chrono::high_resolution_clock::time_point start;
		std::chrono::high_resolution_clock::time_point end;
		bool bIsStopped;

	public:
		Timer(bool startNow = false);

		void Start();
		void Stop();

		TimeResult ElapsedSeconds(bool bUpdateStart = false);
		TimeResult ElapsedMilliseconds(bool bUpdateStart = false);
		TimeResult ElapsedNanoseconds(bool bUpdateStart = false);
	};
    
    namespace Time
    {
        static uint64_t TimeSinceEpoch()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }
        
        static TimeResult TimeSinceEpochNoCount()
        {
            return TimeSinceEpoch() * 0.001f;
        }
    }
}

#endif
