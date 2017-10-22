#ifndef LINEAR_ALLOCATOR_HPP
#define LINEAR_ALLOCATOR_HPP


#include "Memory.hpp"
#include "../CoreUtils/Common.hpp"
#include "../Threading/Thread.hpp"
#include "Allocator.hpp"

#include <iostream>


namespace Ryujin
{
	// Custom allocator for per-frame data
	class LinearAllocator : public OnlyRValueCopyable
	{
	private:
		uint8* start;
		uint8* end;
		uint8* writePtr;
		bool bAllocatedOwnMemory;
		const char* allocatorName;

	public:
		explicit LinearAllocator(const uint64 sizeInBytes, const char* name = "LinearAllocator");
		LinearAllocator(void* inStart, void* inEnd, const char* name = "LinearAllocator");
		~LinearAllocator();
        
        LinearAllocator(LinearAllocator&& other)
        {
            start = Move(other.start);
            end = Move(other.end);
            writePtr = Move(other.writePtr);
            bAllocatedOwnMemory = Move(other.bAllocatedOwnMemory);
            allocatorName = Move(other.allocatorName);
            
            other.start = nullptr;
            other.end = nullptr;
            other.writePtr = nullptr;
        }
        
        LinearAllocator& operator=(LinearAllocator&& other)
        {
            start = Move(other.start);
            end = Move(other.end);
            writePtr = Move(other.writePtr);
            bAllocatedOwnMemory = Move(other.bAllocatedOwnMemory);
            allocatorName = Move(other.allocatorName);
            
            other.start = nullptr;
            other.end = nullptr;
            other.writePtr = nullptr;
            
            return *this;
        }

		FORCEINLINE void* Allocate(PtrSize bytesToAllocate, uint32 alignment = PLATFORM_ALIGNMENT)
		{
			PtrSize freeBytes = GetFreeBytes();
//			PtrSize alignedBytesToAllocate = Memory::GetAlignedSize(bytesToAllocate, alignment);
			void* voidWritePtr = reinterpret_cast<void*>(writePtr);

			if (std::align(alignment, bytesToAllocate, voidWritePtr, freeBytes))
			{
				if ((writePtr + bytesToAllocate) <= end)
				{
					writePtr += bytesToAllocate;
					return voidWritePtr;
				}
			}

			return nullptr;
		}

		FORCEINLINE void Free(void* data)
		{

		}

		FORCEINLINE void Reset()
		{
			writePtr = start;
		}

		FORCEINLINE void SetWritePtrPosition(uint64 offsetFromStart)
		{
			writePtr = (start + offsetFromStart);
		}

		FORCEINLINE uint8* GetStart() const { return start; }
		FORCEINLINE uint8* GetEnd() const { return end; }
		FORCEINLINE PtrSize GetBytesAllocated() const { return writePtr - start; }
		FORCEINLINE PtrSize GetFreeBytes() const { return end - writePtr; }
		FORCEINLINE const char* GetAllocatorName() const { return allocatorName; }
	};


	FORCEINLINE LinearAllocator::LinearAllocator(const uint64 sizeInBytes, const char* name)
	{
		start = reinterpret_cast<uint8*>(Memory::Malloc(sizeInBytes));
		end = (start + sizeInBytes);
		writePtr = start;
		bAllocatedOwnMemory = true;
		allocatorName = name;
	}

	FORCEINLINE LinearAllocator::LinearAllocator(void* inStart, void* inEnd, const char* name)
	{
		start = reinterpret_cast<uint8*>(inStart);
		end = reinterpret_cast<uint8*>(inEnd);
		writePtr = start;
		bAllocatedOwnMemory = false;
		allocatorName = name;
	}

	FORCEINLINE LinearAllocator::~LinearAllocator()
	{
		if (bAllocatedOwnMemory)
			Memory::AlignedFree(start);
		start = nullptr;
		end = nullptr;
		writePtr = nullptr;
	}
    
    class SceneRenderingAllocator
    {
    public:
        static RyujinAllocator<LinearAllocator>& Instance()
        {
            static RyujinAllocator<LinearAllocator> allocator(Memory::ByteConstant::OneKiloByte * 64, "Global scene allocator");
            return allocator;
        }
    };

	static RYUJIN_TLS LinearAllocator* tlsLinearAllocator = nullptr;

	template<typename T>
	class LinearStdAllocator 
	{
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef PtrSize size_type;
		typedef std::ptrdiff_t difference_type;

	public:
		template<typename U>
		struct rebind {
			typedef LinearStdAllocator<U> other;
		};

	public:
		FORCEINLINE LinearStdAllocator() {}
		FORCEINLINE ~LinearStdAllocator() {}
		FORCEINLINE LinearStdAllocator(const LinearStdAllocator& rhs) {}

		template<typename U>
		FORCEINLINE LinearStdAllocator(const LinearStdAllocator<U>& rhs) {}

		FORCEINLINE pointer address(reference r) { return &r; }
		FORCEINLINE const_pointer address(const_reference r) { return &r; }

		FORCEINLINE pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0)
		{
			return reinterpret_cast<pointer>(tlsLinearAllocator->Allocate(uint32(cnt * sizeof(T))));
		}
		FORCEINLINE void deallocate(pointer p, size_type)
		{
			// do nothing
		}

#if PLATFORM_WINDOWS
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
#endif

		FORCEINLINE size_type max_size() const
		{
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

#if PLATFORM_WINDOWS
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#endif

		template <class U, class... Args>
		FORCEINLINE void construct(U* p, Args&&... args)
		{
			new(p) U(std::forward<Args>(args)...);
		}

		FORCEINLINE void destroy(pointer p) { p->~T(); }

		FORCEINLINE bool operator==(LinearStdAllocator const&) const { return false; }
		FORCEINLINE bool operator!=(LinearStdAllocator const& a) const { return false; }

	private:
		template<typename U> friend class LinearStdAllocator;
	};
}


#endif
