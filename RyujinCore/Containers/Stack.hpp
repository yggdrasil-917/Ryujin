#ifndef STACK_HPP
#define STACK_HPP


namespace Ryujin
{
	template <class T>
	class Stack
	{
	private:
		T * container;
		unsigned int capacity;
		unsigned int top;

	public:
		Stack(unsigned int stackCapacity);
		Stack(const Stack<T> & other);
        Stack(Stack<T>&& other);
		~Stack();

        Stack<T>& operator =(const Stack<T> & other);
        Stack<T>& operator =(Stack<T>&& other);

		bool IsEmpty() const { return top == -1; }
		bool IsFull() const { return top == capacity - 1; }
		unsigned int GetCount() const { return top + 1; }
		unsigned int Capacity() const { return capacity; }
		T* GetData() const { return container; }

		void Push(const T & item);
		void Pop(T & item);
		void Clear();
		bool GetTop(T & item) const;
		void GetItemAt(T & item, unsigned index);
	};

	template <class T>
	Stack<T>::Stack(unsigned int stackCapacity)
	{
		capacity = stackCapacity;
		container = new T[capacity];
		top = -1;
	}

	template <class T>
	Stack<T>::Stack(const Stack<T> & other)
	{
		Clear();

		capacity = other.Capacity();
		container = capacity > 0 ? new T[capacity] : nullptr;
		T item;
		for (unsigned i = 0; i < other.GetCount(); ++i)
		{
			other.GetItemAt(item, i);
			container[i] = item;
		}
    }
    
    template <class T>
    Stack<T>::Stack(Stack<T>&& other)
    {
        Clear();
        
        capacity = other.Capacity();
        container = Move(other.container);
        other.container = nullptr;
        other.capacity = 0;
    }

	template <class T>
	Stack<T>::~Stack()
	{
		delete [] container;
		top = -1;
	}

	template <class T>
	Stack<T>& Stack<T>::operator =(const Stack<T> & other)
	{
		Clear();

		capacity = other.Capacity();
		container = capacity > 0 ? new T[capacity] : nullptr;
		T item;
		for (unsigned i = 0; i < other.GetCount(); ++i)
		{
			other.GetItemAt(item, i);
			container[i] = item;
		}
        return *this;
    }
    
    template <class T>
    Stack<T>& Stack<T>::operator =(Stack<T>&& other)
    {
        Clear();
        
        capacity = other.Capacity();
        container = Move(other.container);
        other.container = nullptr;
        other.capacity = 0;
        return *this;
    }

	template <class T>
	void Stack<T>::Push(const T & item)
	{
		if (top >= capacity-1)
			return;

		container[++top] = item;
	}

	template <class T>
	void Stack<T>::Pop(T & item)
	{
		if (top < 0)
			return;

		item = container[top--];
	}

	template <class T>
	void Stack<T>::Clear()
	{
		top = -1;
	}

	template <class T>
	bool Stack<T>::GetTop(T & item) const
	{
		if (top < 0 || top >= capacity-1)
			return false;
		item = container[top];

		return true;
	}

	template <class T>
	void Stack<T>::GetItemAt(T & item, unsigned index)
	{
		if (index < 0 || index >= capacity)
			return;
		item = container[index];
	}
};


#endif
