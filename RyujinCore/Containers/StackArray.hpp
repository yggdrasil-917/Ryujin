#ifndef STACK_ARRAY_HPP
#define STACK_ARRAY_HPP



#include "../OS/Platform.hpp"

#if PLATFORM_MAC
#include <stdlib.h>
#else
#include <malloc.h>
#endif


namespace Ryujin
{
#define STACK_NEW(type,size) new(static_cast<type*>(_malloca(sizeof(type) * (size)))) type[size], (size)

#define STACK_ARRAY(name, type, size) StackArray<type> name(STACK_NEW(type, size))

	// Class representing a variable sized array allocated on the stack
	template<typename T>
	class StackArray
	{
	private:
		T* data;
		uint32 count;

	public:
		StackArray(T* inData, uint32 numElements) : data(inData), count(numElements) {}

		~StackArray()
		{
			// Destructor should destroy objects in reverse order from construction
			for (int32 i = count - 1; i >= 0; --i)
				data[i].~T();
		}

		FORCEINLINE T** GetDataAddress() { return &data; }
		FORCEINLINE T* GetData() { return data; }
		FORCEINLINE operator T*() const { return data; }
		FORCEINLINE uint32 Size() const { count; }

		typedef T* iterator;
		T* begin() { return data; }
		T* end() { return data + count; }
	};
}


#endif