#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP


#include "../OS/Types.hpp"


#if PLATFORM_MAC || PLATFORM_LINUX
#include <stdlib.h>
#else
#include <random>
#endif
#include <ctime>
#include <cmath>
#include <nmmintrin.h>



namespace Ryujin
{
	namespace MathUtils
    {
		static constexpr float PI = 3.1415926535897932384626433832795f;
        static constexpr float PiOverTwo = PI * 0.5f;
        static constexpr float TwoPi = PI * 2.0f;
		static constexpr float OneOverPI = 0.31830988618379067153776752674503f;
        static constexpr float PiOver180 = PI / 180.0f;
        static constexpr float PiOver180Inverse = 180.0f / PI;

		// http://www.gamedev.net/topic/681723-faster-sin-and-cos/
		FORCEINLINE real FastCos(real rads)
		{
			int32 i32I = int32(rads * OneOverPI);
			rads = (rads - real(i32I) * PI);

			real fX2 = rads * rads;

			return (i32I & 1) ?
				-real(1.0) -
				fX2 * (real(-5.00000000000000000e-01) +
					fX2 * (real(4.16666641831398010e-02) +
						fX2 * (real(-1.38888671062886715e-03) +
							fX2 * (real(2.48006836045533419e-05) +
								fX2 * (real(-2.75369188784679864e-07) +
									fX2 * (real(2.06202765973273472e-09) +
										fX2 * real(-9.77589970779790818e-12))))))) :
				real(1.0) +
				fX2 * (real(-5.00000000000000000e-01) +
					fX2 * (real(4.16666641831398010e-02) +
						fX2 * (real(-1.38888671062886715e-03) +
							fX2 * (real(2.48006836045533419e-05) +
								fX2 * (real(-2.75369188784679864e-07) +
									fX2 * (real(2.06202765973273472e-09) +
										fX2 * real(-9.77589970779790818e-12)))))));
		}

		FORCEINLINE real FastSin(real rads)
		{
			int32 i32I = int32(rads * OneOverPI);
			rads = (rads - real(i32I) * PI);

			real fX2 = rads * rads;

			return (i32I & 1) ?
				-rads * (real(1.0) +
					fX2 * (real(-1.66666671633720398e-01) +
						fX2 * (real(8.33333376795053482e-03) +
							fX2 * (real(-1.98412497411482036e-04) +
								fX2 * (real(2.75565571428160183e-06) +
									fX2 * (real(-2.50368437093584362e-08) +
										fX2 * (real(1.58846852338356825e-10) +
											fX2 * real(-6.57978446033657960e-13)))))))) :
				rads * (real(1.0) +
					fX2 * (real(-1.66666671633720398e-01) +
						fX2 * (real(8.33333376795053482e-03) +
							fX2 * (real(-1.98412497411482036e-04) +
								fX2 * (real(2.75565571428160183e-06) +
									fX2 * (real(-2.50368437093584362e-08) +
										fX2 * (real(1.58846852338356825e-10) +
											fX2 * real(-6.57978446033657960e-13))))))));
		}

        FORCEINLINE real Cos(real rads)
        {
#if USE_DOUBLE_PRECISION_MATH
            return cos(rads);
#else
            return cosf(rads);
#endif
        }
        
        FORCEINLINE real Sin(real rads)
        {
#if USE_DOUBLE_PRECISION_MATH
            return sin(rads);
#else
            return sinf(rads);
#endif
        }
        
        FORCEINLINE real Tan(real rads)
        {
#if USE_DOUBLE_PRECISION_MATH
            return tan(rads);
#else
            return tanf(rads);
#endif
        }
        
        FORCEINLINE real ArcCos(real rads)
        {
#if USE_DOUBLE_PRECISION_MATH
            return acos(rads);
#else
            return acosf(rads);
#endif
        }
        
        FORCEINLINE real ArcSin(real rads)
        {
#if USE_DOUBLE_PRECISION_MATH
            return asin(rads);
#else
            return asinf(rads);
#endif
        }
        
        FORCEINLINE real ArcTan(real rads)
        {
#if USE_DOUBLE_PRECISION_MATH
            return atan(rads);
#else
            return atanf(rads);
#endif
        }
        
        FORCEINLINE real Abs(real value)
        {
#if USE_DOUBLE_PRECISION_MATH
            return abs(value);
#else
            return fabs(value);
#endif
        }
        
        FORCEINLINE real Mod(real x, real y)
        {
            return fmod(x, y);
        }
        
        FORCEINLINE real Sqrt(real value)
        {
#if USE_DOUBLE_PRECISION_MATH
            return sqrt(value);
#else
            return sqrtf(value);
#endif
        }
        
		// Taken from AMD's sample on Forward Plus rendering
		static uint16 ConvertF32ToF16(float value)
		{
			// single precision floating point format:
			// |1|   8    |          23           |
			// |s|eeeeeeee|mmmmmmmmmmmmmmmmmmmmmmm|

			// half precision floating point format:
			// |1|  5  |    10    |
			// |s|eeeee|mmmmmmmmmm|

			uint32 uFloatBits = (*(uint32 *)&value);

			// note, this functions does not handle values that are too large (i.e. overflow), 
			// nor does it handle NaNs or infinity
			int32  nExponent = (int32)((uFloatBits & 0x7F800000u) >> 23) - 127 + 15;
			//assert( nExponent < 31 );

			// if the resulting value would be a denorm or underflow, then just return a (signed) zero
			if (nExponent <= 0)
			{
				return (uint16)((uFloatBits & 0x80000000u) >> 16);
			}

			// else, exponent is in the range [1,30], and so we can represent 
			// the value to convert as a normalized 16-bit float (with some loss of precision, of course)
			uint32 uSignBit = (uFloatBits & 0x80000000u) >> 16;
			uint32 uExponentBits = (uint32)nExponent << 10;
			uint32 uMantissaBits = (uFloatBits & 0x007FFFFFu) >> 13;

			return (uint16)(uSignBit | uExponentBits | uMantissaBits);
		}

		static bool Equals(const real a, const real b, const real tolerance = real(0.0001f))
		{
			return MathUtils::Abs(a - b) < tolerance;
		}

		static real Lerp(const real start, const real end, const real factor)
		{
			return start + (end - start) * factor;
		}

		static real Random(const real startRange, const real endRange)
		{
#if PLATFORM_WINDOWS
			static std::mt19937 mt(static_cast<uint32>(std::time(0)));
			std::uniform_real_distribution<real> ud(startRange, endRange);
			return ud(mt);
#else
            return (endRange - startRange) * rand() / (float)RAND_MAX;
#endif
		}
        
        static real DegreesToRadians(real degrees)
        {
            return degrees * real(PiOver180);
        }
        
        static real RadiansToDegrees(real radians)
        {
            return radians * real(PiOver180Inverse);
        }
        
        template<typename T>
        static T RandomFromRange(const T start, const T end)
        {
            return (end - start) * rand() / (T)RAND_MAX;
        }

		static bool IsPowerOfTwo(PtrSize value)
		{
			return ((value & (value - 1)) == 0);
        }
        
        // http://bits.stephan-brumme.com/roundUpToNextPowerOfTwo.html
        static int NextPowerOfTwo(int32 x)
        {
            x--;
            x |= x >> 1; // handle 2 bit numbers
            x |= x >> 2; // handle 4 bit numbers
            x |= x >> 4; // handle 8 bit numbers
            x |= x >> 8; // handle 16 bit numbers
            x |= x >> 16; // handle 32 bit numbers
            x++;
            return x;
        }
        
        template<typename T>
        static uint32 CountLeadingZeros(T value)
        {
            if (!value)
            {
                return sizeof(T) * 8;
            }
            
            uint32 bits = sizeof(T) * 8;
            while(value)
            {
                value >>= 1;
                --bits;
            }
            
            return bits;
        }
        
        template<typename T>
        static uint32 Log2(T value)
        {
            const uint32 bits = sizeof(T) * 8;
            return bits - 1 - CountLeadingZeros<T>(value);
        }

		static bool IsNan(const real value)
		{
			return value != value;
        }
        
        template<typename T>
        T Max(const T& a, const  T& b)
        {
            return a > b ? a : b;
        }
        
        template<typename T>
        T Min(const T& a, const  T& b)
        {
            return a < b ? a : b;
        }
        
        template<typename T>
        T Clamp(const T& value, const T& minValue, const T& maxValue)
        {
            return value < minValue ? minValue : (value > maxValue ? maxValue : value);
        }

		template<typename Type>
		static Type GetAlignedRoundUp(Type value, PtrSize alignment)
		{
            if (!IsPowerOfTwo(alignment))
            {
                return 0;
            }
            
			PtrSize v = (PtrSize)value;
			v = (v + alignment - 1) & ~(alignment - 1);
			return (Type)v;
		}

		template<typename Type>
		static Type AlignRoundUp(Type value, PtrSize alignment)
		{
			return GetAlignedRoundUp(value, alignment);
		}

		template<typename Type>
		static Type GetAlignedRoundDown(Type value, PtrSize alignment)
        {
            if (!IsPowerOfTwo(alignment))
            {
                return 0;
            }
            
			PtrSize v = (PtrSize)value;
			v &= ~(alignment - 1);
			return (Type)v;
		}

		template<typename Type>
		static Type AlignRoundDown(Type value, PtrSize alignment)
		{
			return GetAlignedRoundDown(value, alignment);
		}

		template<typename Type>
		static bool IsAligned(PtrSize alignment, Type value)
		{
			return ((PtrSize)value % alignment) == 0;
		}

		// Taken from the Microsoft MiniEngine
		FORCEINLINE PtrSize HashRange(const uint32* const begin, const uint32* const end, PtrSize hash)
		{
			const uint64* iter64 = (const uint64*)AlignRoundUp(begin, 8);
			const uint64* const end64 = (const uint64* const)AlignRoundDown(end, 8);

			// If not 64-bit aligned, start with a single u32
			if ((uint32*)iter64 > begin)
				hash = _mm_crc32_u32((uint32)hash, *begin);

			// Iterate over consecutive u64 values
			while (iter64 < end64)
				hash = _mm_crc32_u64((uint64)hash, *iter64++);

			// If there is a 32-bit remainder, accumulate that
			if ((uint32*)iter64 < end)
				hash = _mm_crc32_u32((uint32)hash, *(uint32*)iter64);

			return hash;
		}
	}

	template<class T>
	struct Rect
	{
		T x;
		T y;
		T width;
		T height;

		Rect() : x(0), y(0), width(0), height(0) {}
		Rect(T inX, T inY, T inWidth, T inHeight) : x(inX), y(inY), width(inWidth), height(inHeight) {}

		FORCEINLINE T GetArea() const { return width * height; }

		// Assuming axis aligned rectangle
		FORCEINLINE bool Contains(T pointX, T pointY) const
		{
			return (x <= pointX && y <= pointY && (x+width) >= pointX && (y+height) >= pointY);
		}

		FORCEINLINE bool Contains(const Rect<T>& rect)
		{
			return Contains(rect.x, rect.y) && Contains(rect.x + rect.width, rect.y + rect.height);
		}
	};
}


#endif
