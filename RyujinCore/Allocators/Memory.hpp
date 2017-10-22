#ifndef MEMORY_HPP
#define MEMORY_HPP


#include <memory>

#include "../CoreUtils/CRC.hpp"
#include "../Math/MathUtils.hpp"


#if PLATFORM_WINDOWS
#define MallocSize _msize

#elif PLATFORM_MAC
#include <malloc/malloc.h>
#define MallocSize malloc_size

#elif PLATFORM_LINUX
#include <malloc.h>
#define MallocSize malloc_usable_size

#endif

//#define USE_TBB 1//!PLATFORM_MAC

#ifndef USE_TBB
#   define USE_TBB 0
#   define USE_DEFAULT_ALLOCATOR 1
#endif

namespace Ryujin
{
	namespace Memory
	{
#if PLATFORM_MAC
#define PLATFORM_ALIGNMENT 16
#else
#define PLATFORM_ALIGNMENT 8
#endif

		enum ByteConstant
		{
			OneByte		= 1,
			OneKiloByte = 1024,
			OneMegaByte = 1048576,
			OneGigaByte = 1073741824,
		};

		static PtrSize GetAlignedSize(PtrSize sizeInBytes, uint32 alignment)
		{
			return ((sizeInBytes + alignment - 1) - (sizeInBytes + alignment - 1) % alignment);
		}
        
        static void AlignPointer(void*& ptr, PtrSize alignment)
        {
            ptr = (void*)((uintptr_t(ptr) + alignment - 1) & ~(alignment - 1));
        }

		static void* Malloc(PtrSize bytes)
		{
			return malloc(bytes);
		}
        
        static void* Malloca(PtrSize bytes)
        {
#if PLATFORM_WINDOWS
            return _malloca(bytes);
#else
            return alloca(bytes);
#endif
        }

		static void Free(void* p)
		{
			free(p);
		}
        
        static void* Realloc(void* ptr, PtrSize sizeInBytes)
        {
            return realloc(ptr, sizeInBytes);
        }
        
        template<typename T>
        static T* Calloc(uint32 count)
        {
            return reinterpret_cast<T*>(calloc(count, sizeof(T)));
        }

		static void MemCpy(void* dst, void* src, PtrSize size)
		{
			memcpy(dst, src, size);
        }
        
        static void MemMove(void* dst, void* src, PtrSize size)
        {
            memmove(dst, src, size);
        }

		static void MemSet(void* dst, int32 value, PtrSize size)
		{
			memset(dst, value, size);
		}
        
        static int32 MemCmp(const void* ptr1, const void* ptr2, PtrSize numBytes)
        {
            return memcmp(ptr1, ptr2, numBytes);
        }

		static void* AlignedAllocate(PtrSize bytesToAllocate, uint32 alignment)
		{
#if PLATFORM_WINDOWS
			return _aligned_malloc(bytesToAllocate, alignment);
#else
            void* mem = nullptr;
            posix_memalign(&mem, alignment, bytesToAllocate);
            
            return mem;
#endif
		}

		template<typename T>
		static void AlignedFree(T* p)
        {
#if PLATFORM_WINDOWS
            _aligned_free(reinterpret_cast<void*>(p));
#else
            free(p);
#endif
		}
	}
    
    
}

#if USE_DEBUG_ALLOCATORS

extern CORE_API void* GlobalNew(Ryujin::PtrSize count, Ryujin::int32 line, const char* file);
extern CORE_API void* GlobalNewArray(Ryujin::PtrSize count, Ryujin::int32 line, const char* file);

extern CORE_API void GlobalDelete(void* p) noexcept;
extern CORE_API void GlobalDeleteArray(void* p) noexcept;

FORCEINLINE void* operator new(Ryujin::PtrSize count, Ryujin::int32 line, const char* file)
{
	return GlobalNew(count, line, file);
}

FORCEINLINE void* operator new[](Ryujin::PtrSize count, Ryujin::int32 line, const char* file)
{
	return GlobalNewArray(count, line, file);
}

#define RYUJIN_NEW new(__LINE__, __FILE__)

#else

extern CORE_API void* GlobalNew(Ryujin::PtrSize count);
extern CORE_API void* GlobalNewArray(Ryujin::PtrSize count);

extern CORE_API void GlobalDelete(void* p) noexcept;
extern CORE_API void GlobalDeleteArray(void* p) noexcept;

#if PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning (disable: 28251) // Turn off inconsistent annotation warning from code analyzer
#endif
FORCEINLINE void* operator new(Ryujin::PtrSize count)
{
	return GlobalNew(count);
}

FORCEINLINE void* operator new[](Ryujin::PtrSize count)
{
	return GlobalNewArray(count);
}

#if PLATFORM_WINDOWS
#pragma warning(pop)
#endif

#define RYUJIN_NEW new

#endif

FORCEINLINE void operator delete(void* p) noexcept
{
	GlobalDelete(p);
}

FORCEINLINE void operator delete[](void* p) noexcept
{
	GlobalDeleteArray(p);
}


#endif