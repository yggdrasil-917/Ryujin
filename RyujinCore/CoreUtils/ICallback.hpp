#ifndef ICALLBACK_HPP
#define ICALLBACK_HPP

#include "../OS/Platform.hpp"

namespace Ryujin
{
	template<typename...Args>
	class ICallback
	{
	public:
        VIRTUAL ~ICallback() {}
		VIRTUAL void Invoke(Args...args) = 0;
	};
};


#endif