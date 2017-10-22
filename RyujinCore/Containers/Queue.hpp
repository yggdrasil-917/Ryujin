#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../Allocators/GlobalAllocators.hpp"

namespace Ryujin
{
	template <class T>
	class Queue
	{
	private:
		T* container;
		uint32 capacity;
		uint32 placementIndex;

	public:
		Queue(uint32 maxCapacity = 128);
		Queue(const Queue<T>& other);
        Queue(Queue<T>&& other);
		~Queue();

        Queue<T>& operator =(const Queue<T>& other);
        Queue<T>& operator =(Queue<T>&& other);

		FORCEINLINE bool IsEmpty() const { return capacity == placementIndex; }
		FORCEINLINE bool IsFull() const { return placementIndex == 0; }
		FORCEINLINE uint32 PlacementIndex() const { return placementIndex - 1; }
		FORCEINLINE uint32 Capacity() const { return capacity; }

		bool Enqueue(const T & item);
		bool Dequeue(T & item);
		void Clear();
		bool Peek(T & item) const;
		bool PeekAt(T & item, uint32 index) const;
	};

	template <class T>
	Queue<T>::Queue(uint32 maxCapacity)
	{
		capacity = maxCapacity;
		container = RYUJIN_NEW T[capacity];
		placementIndex = capacity;
	}

	template <class T>
	Queue<T>::Queue(const Queue<T>& other)
	{
		capacity = other.capacity;
		container = capacity > 0 ? RYUJIN_NEW T[capacity] : nullptr;
		placementIndex = other.placementIndex;
		for (uint32 i = 0; i < capacity; ++i)
		{
			T item;
			if (other.PeekAt(item, i))
				container[i] = item;
		}
	}
    
    template <class T>
    Queue<T>::Queue(Queue<T>&& other)
    {
        capacity = other.capacity;
        container = Move(other.container);
        placementIndex = other.placementIndex;
        
        other.capacity = 0;
        other.placementIndex = 0;
        other.container = nullptr;
    }

	template <class T>
	Queue<T>::~Queue()
	{
		SAFE_DELETE_ARRAY(container);
		placementIndex = capacity;
	}

	template <class T>
	Queue<T>& Queue<T>::operator =(const Queue<T> & other)
	{
		Clear();
		capacity = other.Capacity();
        container = capacity > 0 ? new T[capacity] : nullptr;
        placementIndex = other.placementIndex;
		for (uint32 i = 0; i < capacity; ++i)
		{
			T item;
			if (other.PeekAt(item, i))
				container[i] = item;
		}

		return *this;
    }
    
    template <class T>
    Queue<T>& Queue<T>::operator =(Queue<T>&& other)
    {
        capacity = other.capacity;
        container = Move(other.container);
        placementIndex = other.placementIndex;
        
        other.capacity = 0;
        other.placementIndex = 0;
        other.container = nullptr;
        
        return *this;
    }

	template <class T>
	bool Queue<T>::Enqueue(const T & item)
	{
		if (IsFull())
			return false;
		container[--placementIndex] = item;
		return true;
	}

	template <class T>
	bool Queue<T>::Dequeue(T & item)
	{
		if (IsEmpty())
			return false;
		item = container[capacity-1];
		for (uint32 i = capacity-1; i > placementIndex; --i)
			container[i] = container[i-1];
		++placementIndex;
        return true;
	}

	template <class T>
	void Queue<T>::Clear()
	{
		placementIndex = capacity-1;
	}

	template <class T>
	bool Queue<T>::Peek(T & item) const
	{
		if (IsEmpty())
			return false;

		item = container[capacity-1];
		return true;
	}

	template <class T>
	bool Queue<T>::PeekAt(T & item, uint32 index) const
	{
		if (IsEmpty() || index < 0 || index >= capacity)
			return false;

		item = container[index];
		return true;
	}
};


#endif
