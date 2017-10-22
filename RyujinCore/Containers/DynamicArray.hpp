#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP


//#include "../Logger/Logger.hpp"

#include "../Allocators/GlobalAllocators.hpp"
#include "../CoreUtils/Common.hpp"

#include <initializer_list>

namespace Ryujin
{
    template<class T, class Allocator = DefaultEngineAllocator>
	class DynamicArray
	{
	public:
		using Value = T;
		using Iterator = Value*;
		using ConstIterator = const Value*;
		using Reference = Value&;
		using ConstReference = const Value&;

		// STL compatibility
		using iterator = Iterator;
		using const_iterator = ConstIterator;
		using reference = Reference;
		using const_reference = ConstReference;

	protected:
		Value* data;
		uint32 size;
		uint32 itemCount;
        
        FORCEINLINE void DestructItems(uint32 index, uint32 count = 1)
        {
            if (!std::is_trivially_destructible<Value>())
            {
                for (uint32 i = index; i < index + count; ++i)
                {
                    (data + i)->~Value();
                }
            }
        }

	public:
		DynamicArray()
            : data(nullptr), size(0), itemCount(0)
        {
            
        }

        DynamicArray(const uint32 count)
            : data(nullptr), size(0), itemCount(0)
		{
			Resize(count);
		}

		DynamicArray(std::initializer_list<T> l)
			: data(nullptr), size(0), itemCount(0)
		{
			Resize((uint32)l.size());
			for (auto it = l.begin(); it != l.end(); ++it)
			{
				Add(*it);
			}
		}
        
        DynamicArray(const DynamicArray<Value, Allocator>& other)
        {
            itemCount = other.itemCount;
            size = other.size;
            data = (Value*)Allocator::Instance().Allocate(size * sizeof(Value));
            
            if (std::is_pod<Value>())
            {
                Memory::MemCpy(data, other.data, itemCount * sizeof(Value));
            }
            else
            {
                for (const auto& item : other)
                {
                    Add(item);
                }
            }
        }
        
        DynamicArray<Value, Allocator>& operator=(const DynamicArray<Value, Allocator>& other)
        {
            itemCount = other.itemCount;
            size = other.size;
            data = (Value*)Allocator::Instance().Allocate(size * sizeof(Value));
            
            if (std::is_pod<Value>())
            {
                Memory::MemCpy(data, other.data, itemCount * sizeof(Value));
            }
            else
            {
                for (const auto& item : other)
                {
                    Add(item);
                }
            }
            
            return *this;
        }

		DynamicArray(DynamicArray<Value, Allocator>&& other)
		{
			if (this != &other)
			{
				data = Move(other.data);
				size = Move(other.size);
                itemCount = Move(other.itemCount);

				// other needs to be reset, otherwise they point to the same data
				other.data = nullptr;
                other.size = 0;
                other.itemCount = 0;
			}
		}

		DynamicArray<Value, Allocator>& operator=(DynamicArray<Value, Allocator>&& right)
		{
			if (this != &right)
			{
				data = Move(right.data);
                size = Move(right.size);
                itemCount = Move(right.itemCount);

				// right needs to be reset, otherwise they point to the same data
				right.data = nullptr;
				right.size = 0;
                right.itemCount = 0;
			}

			return *this;
		}

		~DynamicArray()
		{
            RemoveAll();
            
            if (data)
            {
                Allocator::Instance().Free(data);
                data = nullptr;
            }
            
            size = 0;
		}

		void Add(const Value& newValue)
		{
			if (itemCount == size)
			{
				Resize(size * 2 + 8);
            }
            RYUJIN_ASSERT(data, "Null data array");
            
            new(data + itemCount++) Value(newValue);
		}

		void Add(Value&& newValue)
		{
			if (itemCount == size)
			{
				Resize(size * 2 + 8);
            }
            RYUJIN_ASSERT(data, "Null data array");
            
            new(data + itemCount++) Value(Move(newValue));
		}

		void AddZeroed(uint32 count)
		{
			for (uint32 i = 0; i < count; ++i)
			{
				Value zeroed;
				Memory::MemSet(&zeroed, 0, sizeof(Value));
				Add(zeroed);
			}
		}
        
        int32 AddUnique(const Value& newValue)
        {
            for (uint32 i = 0; i < itemCount; ++i)
            {
                if ((*this)[i] == newValue)
                {
                    return (int32)i;
                }
            }
            
            Add(newValue);
            
            return itemCount - 1;
        }

		void Remove(const uint32 index)
		{
			RYUJIN_ASSERT(index >= 0 && index < size, "Index out of bounds");
            
            DestructItems(index);
            if (itemCount > 0)
            {
                data[index] = data[itemCount-1];
            }
            --itemCount;
		}
        
        void RemoveSlow(const uint32 index)
        {
            RYUJIN_ASSERT(index >= 0 && index < size, "Index out of bounds");
            
            DestructItems(index);
            
            // Shift all elements to the right of index left to preserve element order
            for (uint32 i = index + 1; i <= itemCount - 1; ++i)
            {
                data[i - 1] = Move(data[i]);
            }
            --itemCount;
        }

		void RemoveAll()
		{
			if (itemCount == 0)
				return;
            
            DestructItems(0, itemCount);

			itemCount = 0;
		}

		void Resize(const uint32 newSize)
		{
			if (itemCount > 0)
            {
                Value* newData = (Value*)Allocator::Instance().Allocate(newSize * sizeof(Value));
                RYUJIN_ASSERT(newData, "Could not allocate new block");

				Memory::MemCpy(newData, data, size * sizeof(Value));
                Allocator::Instance().Free(data);
                data = newData;
                size = newSize;
			}
			else
			{
                data = (Value*)Allocator::Instance().Allocate(newSize * sizeof(Value));
                RYUJIN_ASSERT(data, "Could not allocate new block");
                Memory::MemSet(data, 0, newSize * sizeof(Value));
				size = newSize;
			}
		}
        
        void Copy(Value* inData, uint32 count)
        {
            RemoveAll();
            
            Resize(count);
            
            if (std::is_pod<Value>())
            {
                Memory::MemCpy(data, inData, count * sizeof(Value));
            }
            else
            {
                for (uint32 i = 0; i < count; ++i)
                {
                    data[i] = inData[i];
                }
            }
            itemCount = count;
        }

		void MoveTemp(Value* inData, uint32 count)
		{
			RemoveAll();

			Resize(count);

			data = Move(inData);
			itemCount = count;

			inData = nullptr;
		}
        
        bool Contains(const Value& value) const
        {
            for (uint32 i = 0; i < itemCount; ++i)
            {
                if (data[i] == value)
                    return true;
            }
            
            return false;
        }


		FORCEINLINE Reference operator[](const uint32 i)
		{
			RYUJIN_ASSERT(i >= 0 && i < size, "Index out of bounds");
			return data[i];
		}

		FORCEINLINE ConstReference operator[](const uint32 i) const
		{
			RYUJIN_ASSERT(i >= 0 && i < size, "Index out of bounds");
			return data[i];
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE Iterator Begin()
		{
			return data;
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE ConstIterator Begin() const
		{
			return data;
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE Iterator End()
		{
			return data + itemCount;
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE ConstIterator End() const
		{
			return data + itemCount;
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE Iterator begin()
		{
			return Begin();
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE ConstIterator begin() const
		{
			return Begin();
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE Iterator end()
		{
			return End();
		}

		/// Make it compatible with the C++11 range based for loop.
		FORCEINLINE ConstIterator end() const
		{
			return End();
		}

		/// Get first element.
		FORCEINLINE Reference Front()
		{
			return data[0];
		}

		/// Get first element.
		FORCEINLINE ConstReference Front() const
		{
			return data[0];
		}

		/// Get last element.
		FORCEINLINE Reference Back()
		{
			return data[itemCount - 1];
		}

		/// Get last element.
		FORCEINLINE ConstReference Back() const
		{
			return data[itemCount - 1];
		}

		/// Get last element.
		FORCEINLINE ConstReference back() const
		{
			return data[itemCount - 1];
		}

        FORCEINLINE void* GetData() const { return static_cast<void*>(data); }
        FORCEINLINE T* GetTypedData() const { return data; }
		FORCEINLINE uint32 GetSize() const { return itemCount; }
		FORCEINLINE bool IsEmpty() const { return itemCount == 0; }
        FORCEINLINE bool IsFull() const { return itemCount == size; }
	};
}

#endif
