#ifndef LIST_HPP
#define LIST_HPP

#include "../Allocators/Memory.hpp"

namespace Ryujin
{
	template <class T>
	class List
	{
	protected:
		struct Node
		{
			T element;
			Node * next;
	
			Node(const T& value, Node * nextNode) : element(value), next(nextNode) {}
		};
		Node * head;
		Node * iterator;
		uint32 count;
	
	public:
		List();
		List(const List<T> & other);
        List(List<T>&& other);
		~List();

        List<T>& operator =(const List<T> & other);
        List<T>& operator =(List<T>&& other);
	
		uint32 Size() const { return count; }
	
		void Add(const T & value);
		bool AddUnique(const T & value);
		bool Remove(const T & value);
		bool RemoveFront();
		void RemoveAll();
	
		bool GetFirst(T & value);
		bool GetNext(T & next);
	
		void Reverse();
		bool Contains(const T & value) const;
	};
	
	template <class T>
	List<T>::List()
	{
		count = 0;
		head = nullptr;
		iterator = nullptr;
	}
	
	template <class T>
	List<T>::List(const List<T> & other)
	{
		count = 0;
		head = nullptr;
		iterator = other.head;

		while (iterator)
		{
			Add(iterator->element);
			iterator = iterator->next;
		}
	}
    
    template <class T>
    List<T>::List(List<T>&& other)
    {
        count = other.count;
        head = Move(other.head);
        other.head = nullptr;
        iterator = nullptr;
        other.count = 0;
    }

	template <class T>
	List<T>::~List()
	{
		RemoveAll();
	}

	template <class T>
	List<T>& List<T>::operator =(const List<T> & other)
	{
		RemoveAll();

		count = 0;
		head = nullptr;
		iterator = other.head;

		while (iterator)
		{
			Add(iterator->element);
			iterator = iterator->next;
		}

		return *this;
	}
    
    template <class T>
    List<T>& List<T>::operator=(List<T>&& other)
    {
        count = other.count;
        head = Move(other.head);
        other.head = nullptr;
        iterator = nullptr;
        other.count = 0;
        return *this;
    }
	
	template <class T>
	void List<T>::Add(const T & value)
	{
		if (head == nullptr)
			head = new Node(value, nullptr);
		else
		{
			Node * newNode = new Node(value, head);
			newNode->next = head;
			head = newNode;
		}
		//std::cout << "Added value " << value << std::endl;
		++count;
	}
	
	template <class T>
	bool List<T>::AddUnique(const T & value)
	{
		Node * temp = head;
		while (temp)
		{
			if (temp->element == value)
				return false;
			temp = temp->next;
		}
		Add(value);
		return true;
	}
	
	template <class T>
	bool List<T>::Remove(const T & value)
	{
		Node * curr = head;
		Node * prev = nullptr;
		while (curr)
		{
			if (curr->element == value)
			{
				if (prev == nullptr) // head needs to be removed
				{
					head = head->next;
					curr->next = nullptr;
				}
				else if (curr->next == nullptr) // remove tail
				{
					prev->next = nullptr;
				}
				else  // remove internal node
				{
					prev->next = curr->next;
					curr->next = nullptr;
				}
				delete curr;
				--count;
				return true;
			}
			prev = curr;
			curr = curr->next;
		}
		return false;
	}
	
	template <class T>
	bool List<T>::RemoveFront()
	{
		if (head == nullptr)
			return false;
	
		Node * temp = head;
		head = head->next;
		delete temp;
		--count;
	
		return true;
	}
	
	template <class T>
	void List<T>::RemoveAll()
	{
		Node * curr = head;
		while (curr)
		{
			Node * temp = curr;
			curr = curr->next;
			delete temp;
			--count;
		}
		head = nullptr;
	}
	
	template <class T>
	bool List<T>::GetFirst(T & value)
	{
		if (head == nullptr)
		{
			iterator = nullptr;
			return false;
		}
	
		value = head->element;
		iterator = head->next;
		return true;
	}
	
	template <class T>
	bool List<T>::GetNext(T & value)
	{
		if (iterator == nullptr)
			return false;
	
		value = iterator->element;
		iterator = iterator->next;
		return true;
	}
	
	template <class T>
	void List<T>::Reverse()
	{
		Node * rev = nullptr;
		while (head)
		{
			Node * temp = head;
			head = head->next;
			temp->next = rev;
			rev = temp;
		}
		head = rev;
	}
	
	template <class T>
	bool List<T>::Contains(const T & value) const
	{
		Node * curr = head;
		while (curr)
		{
			if (curr->element == value)
				return true;
			curr = curr->next;
		}
		return false;
	}
};

#endif
