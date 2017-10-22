#ifndef COMMON_HPP
#define COMMON_HPP


#include <string>
#include <sstream>
#include <iomanip> // put_time
#include "Timer.hpp"
#include "../Allocators/Memory.hpp"

namespace Ryujin
{
    template<typename T>
    void Swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
    
    template<typename T>
    void MoveSwap(T&& a, T&&b)
    {
        T&& temp = Move(a);
        a = Move(b);
        b = Move(temp);
    }


	class CORE_API NonCopyable
	{
	protected:
		NonCopyable() {} // ensure the class cannot be constructed directly

		~NonCopyable() {} // not to be used polymorphically

	public:
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
		NonCopyable(const NonCopyable&&) = delete;
		NonCopyable& operator=(const NonCopyable&&) = delete;
	};

	class CORE_API OnlyRValueCopyable
	{
	protected:
		OnlyRValueCopyable() {} // ensure the class cannot be constructed directly

		~OnlyRValueCopyable() {} // not to be used polymorphically

	public:
		OnlyRValueCopyable(const OnlyRValueCopyable&) = delete;
		OnlyRValueCopyable& operator=(const OnlyRValueCopyable&) = delete;
	};


	/*Function to left rotate n by d bits*/
	template<typename T>
	uint64 LeftRotate(T n, uint32 d)
	{
		uint32 bits = sizeof(n) * 8;
		/* In n<<d, last d bits are 0. To put first 3 bits of n at
		last, do bitwise or of n<<d with n >>(bits - d) */
		return (n << d) | (n >> (bits - d));
	}

	/*Function to right rotate n by d bits*/
	template<typename T>
	uint64 RightRotate(T n, uint32 d)
	{
		uint32 bits = sizeof(n) * 8;
		/* In n>>d, first d bits are 0. To put last 3 bits of at
		first, do bitwise or of n>>d with n <<(bits - d) */
		return (n >> d) | (n << (bits - d));
	}

	template <typename To, typename From>
	FORCEINLINE bool IsA(From* from)
	{
		return typeid(To) == typeid(*from);
	}

	template <typename To, typename From>
	FORCEINLINE bool IsA(From& from)
	{
		return typeid(To) == typeid(from);
	}
    
    
//    http://blog.molecular-matters.com/2015/12/11/getting-the-type-of-a-template-argument-as-string-without-rtti/
    namespace TypeName
    {
        static const uint32 FRONT_SIZE = sizeof("TypeName::GetTypeNameHelper<") - 1u;
        static const uint32 BACK_SIZE = sizeof(">::GetTypeName") - 1u;
        
        template <typename T>
        struct GetTypeNameHelper
        {
            static const char* GetTypeName(void)
            {
                static const PtrSize size = sizeof(__FUNCTION__) - FRONT_SIZE - BACK_SIZE;
                static char typeName[size] = {};
                Memory::MemCpy(typeName, __FUNCTION__ + FRONT_SIZE, size - 1u);
                
                return typeName;
            }
        };
    }
    
    template <typename T>
    const char* GetTypeName(void)
    {
        return TypeName::GetTypeNameHelper<T>::GetTypeName();
    }
    
    static bool IsDigit(const char digit)
    {
        return digit >= '0' && digit <= '9';
    }
    
    FORCEINLINE float AtoF(char*& numberStr)
    {
        float sign = 1.0f;
        float number = 0.0f;
        float power = 1.0f;
        float decimal = 0.0f;
        
        if (numberStr[0] == '-')
        {
            sign = -1.0f;
            ++numberStr;
        }
        
        while (*numberStr && IsDigit(*numberStr))
        {
            number = number * 10.0f + (*numberStr - '0');
            
            ++numberStr;
            if (*numberStr == '.')
            {
                ++numberStr;
                break;
            }
        }
        
        while (*numberStr && IsDigit(*numberStr))
        {
            decimal = decimal * 10.0f + (*numberStr - '0');
            power *= 10.0f;
            ++numberStr;
        }
        
        return sign * (number + decimal / power);
    }
    
    // remove_reference
    template <class T> struct RemoveReference      { typedef T type; };
    template <class T> struct RemoveReference<T&>  { typedef T type; };
    template <class T> struct RemoveReference<T&&> { typedef T type; };
    
    // Ryujin's way of doing std::move
    template <class T>
    inline typename RemoveReference<T>::type&& Move(T&& t)
    {
        typedef typename RemoveReference<T>::type _Up;
        return static_cast<_Up&&>(t);
    }
    

    
    ///////////////////////////////////////////////////
    //////////// Radix sort
    template<typename T, typename U>
    FORCEINLINE void CountSort(T* data, uint32 n, U exp)
    {
        T output[n];
        int32 count[10] = {0};
        
        for (int32 i = 0; i < n; i++)
            count[(data[i] / exp) % 10]++;
        
        for (int32 i = 1; i < 10; i++)
            count[i] += count[i - 1];
        
        for (int32 i = n - 1; i >= 0; i--)
        {
            output[count[(data[i] / exp) % 10] - 1] = data[i];
            count[(data[i] / exp) % 10]--;
        }
        
        Memory::MemCpy(data, output, sizeof(T) * n);
    }
    
    template<typename T, typename U>
    FORCEINLINE void RadixSort(T* data, uint32 count)
    {
        T currentMax = data[0];
        for (uint32 i = 0; i < count; ++i)
        {
            if (data[i] > currentMax)
                currentMax = data[i];
        }
        
        for (U exp = 1; currentMax / exp > 0; exp *= 10)
            CountSort(data, count, exp);
    }
}



#endif
