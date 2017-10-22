#ifndef STATIC_FUNCTION_CALLBACK_HPP
#define STATIC_FUNCTION_CALLBACK_HPP


#include "ICallback.hpp"


namespace Ryujin
{
	template<typename...Args>
	class StaticFunctionCallback : public ICallback<Args...>
	{
	private:
		void (*func)(Args...);

	public:
		StaticFunctionCallback(void (*newFunc)(Args...)) : func(newFunc) {}

		VIRTUAL void Invoke(Args...args) OVERRIDE FINAL
		{
			(*func)(args...);
		}
	};
};


#endif