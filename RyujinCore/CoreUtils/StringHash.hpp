#ifndef STRING_HASH_HPP
#define STRING_HASH_HPP


#include "../Containers/String.hpp"


namespace Ryujin
{
#define FNV_OFFSET 2166136261u
#define FNV_PRIME 16777619u

	// FNV-1a hash implementation
	static PtrSize CalculateFNV(const char* str, PtrSize offset = FNV_OFFSET, PtrSize prime = FNV_PRIME)
	{
		PtrSize hash = offset;
		while (*str != 0)
		{
            hash = (hash ^ (*str)) * prime;
            ++str;
		}

		return (hash ^ (*str)) * prime;
	}

	template <uint32 N, uint32 I>
	struct FnvHash
	{
		static uint32 Hash(const char(&str)[N])
		{
            return str[I - 1] == 0 ? FnvHash<N, I - 1>::Hash(str) : (FnvHash<N, I - 1>::Hash(str) ^ str[I - 1]) * FNV_PRIME;
		}
	};

	template <uint32 N>
	struct FnvHash <N, 1>
	{
		static uint32 Hash(const char(&str)[N])
		{
			return (FNV_OFFSET ^ str[0]) * FNV_PRIME;
		}
	};

	// String hash class based on http://www.gamasutra.com/view/news/38198/InDepth_Quasi_CompileTime_String_Hashing.php
	class StringHash
	{
	private:
		uint32 hash;
        String strRef;

	public:
        StringHash() : hash(0) {}
        
		struct ConstCharWrapper
		{
			FORCEINLINE ConstCharWrapper(const char* inStr)
				: str(inStr)
			{

			}
			const char* str;
		};

#if !PLATFORM_WINDOWS // so far only Windows linker seems to hang with an infinite recursion bug with optimization turned on
		template <uint32 N>
		FORCEINLINE StringHash(const char(&str)[N])
			: hash(FnvHash<N, N>::Hash(str))
        {
            strRef = str;
		}
#endif

		explicit StringHash(ConstCharWrapper str)
			: hash((uint32)CalculateFNV(str.str, FNV_OFFSET, FNV_PRIME))
		{
            strRef = str.str;
		}
        
        StringHash(const StringHash& other)
        : hash(other.hash)
        {
            strRef = other.strRef;
        }
        
        StringHash& operator=(const StringHash& other)
        {
            hash = other.hash;
            strRef = other.strRef;
            
            return *this;
        }

		FORCEINLINE uint32 GetHash() const { return hash; }
        FORCEINLINE const char* ToString() const { return *strRef; }
	};
}


#endif
