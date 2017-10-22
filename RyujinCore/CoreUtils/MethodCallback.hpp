#ifndef METHOD_CALLBACK_HPP
#define METHOD_CALLBACK_HPP


#include "ICallback.hpp"

namespace Ryujin
{
	template<typename T, typename Method, typename...Args>
	class MethodCallback : public ICallback<Args...>
	{
	private:
		T * object;
		Method method;

	public:
		MethodCallback(T * obj, Method newMethod) : object(obj), method(newMethod) {}
        ~MethodCallback() OVERRIDE {}

		VIRTUAL void Invoke(Args...args) OVERRIDE FINAL
		{
			if (object != nullptr)
				(object->*method)(args...);
		}
	};
};


#endif