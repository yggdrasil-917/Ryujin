#ifndef DELEGATE_HPP
#define DELEGATE_HPP


#include "../Containers/DynamicArray.hpp"
#include "MethodCallback.hpp"
#include "StaticFunctionCallback.hpp"

namespace Ryujin
{
	template<typename...Args>
	class Delegate
	{
	private:
		DynamicArray< ICallback<Args...>* > callbacks;

	public:
		Delegate() {}
		~Delegate()
		{
			for(unsigned i = 0; i < callbacks.GetSize(); ++i)
			{
				delete callbacks[i]; 
			}
		}

		template<typename T, typename Method>
		void AddCallback(Method newMethod, T* obj)
		{
			callbacks.Add(RYUJIN_NEW MethodCallback<T, Method, Args...>(obj, newMethod));
		}

		template<typename Method>
		void AddCallback(Method newMethod)
		{
			callbacks.Add(RYUJIN_NEW StaticFunctionCallback<Args...>(newMethod));
		}

		void RemoveAllCallbacks()
        {
            for(unsigned i = 0; i < callbacks.GetSize(); ++i)
            {
                delete callbacks[i];
            }
			callbacks.RemoveAll();
		}

		template<typename T, typename Method>
		void RemoveCallback(T* obj, Method methodToRemove)
		{
			// TODO: finish implementation
			for (int i = 0; i < callbacks.GetSize(); ++i)
			{
			}
		}

		void operator()(Args...args)
		{
			for (uint32 i = 0; i < callbacks.GetSize(); ++i)
			{
				if (callbacks[i] != nullptr)
					callbacks[i]->Invoke(args...);
			}
		}
	};
};


#endif