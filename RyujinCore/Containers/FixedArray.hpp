#ifndef FIXED_ARRAY_HPP
#define FIXED_ARRAY_HPP


#include "../OS/Platform.hpp"
#include "../Logger/Logger.hpp"
#include "../Allocators/GlobalAllocators.hpp"

#include <initializer_list>

namespace Ryujin
{
	template <typename T, PtrSize N>
	class FixedArray
	{
	public:
		FixedArray()
		{

		}

		FixedArray(const FixedArray<T, N>& other)
		{
			Memory::MemCpy((void*)data, (void*)other.data, sizeof(data));
		}

		FixedArray(std::initializer_list<T> l)
		{
			RYUJIN_ASSERT(l.size() <= N, "Initializer list is too long");
			int32 i = 0;
			for (auto it = l.begin(); it != l.end(); ++it)
			{
				data[i++] = *it;
			}
		}

		FixedArray& operator=(const FixedArray<T, N>& other)
		{
			Memory::MemCpy((void*)data, (void*)other.data, sizeof(data));
			return *this;
		}

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


		Reference operator[](const PtrSize i)
		{
			RYUJIN_ASSERT(i >= 0 && i < N, "Index out of bounds");
			return data[i];
		}

		ConstReference operator[](const PtrSize i) const
		{
			RYUJIN_ASSERT(i >= 0 && i < N, "Index out of bounds");
			return data[i];
		}

		Iterator Begin()
		{
			return &data[0];
		}

		ConstIterator Begin() const
		{
			return &data[0];
		}

		Iterator End()
		{
			return &data[0] + N;
		}

		ConstIterator End() const
		{
			return &data[0] + N;
		}

		Reference Front()
		{
			return data[0];
		}

		ConstReference Front() const
		{
			return data[0];
		}

		Reference Back()
		{
			return data[N - 1];
		}

		ConstReference Back() const
		{
			return data[N - 1];
		}

		static constexpr PtrSize Size()
		{
			return N;
		}

		// Start of STL compatibility functions
		// Make it compatible with the C++11 range based for loop
		Iterator begin()
		{
			return Begin();
		}

		// Make it compatible with the C++11 range based for loop
		ConstIterator begin() const
		{
			return Begin();
		}

		// Make it compatible with the C++11 range based for loop
		Iterator end()
		{
			return End();
		}

		// Make it compatible with the C++11 range based for loop
		ConstIterator end() const
		{
			return End();
		}

		Reference front()
		{
			return Front();
		}

		ConstReference front() const
		{
			return Front();
		}

		Reference back()
		{
			return Back();
		}

		ConstReference back() const
		{
			return Back();
		}

		static constexpr PtrSize size()
		{
			return N;
		}
		// End STL compatibility functions

	protected:
		Value data[N];
	};
}


#endif
