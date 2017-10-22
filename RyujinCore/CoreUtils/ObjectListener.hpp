#pragma once

#include "../Logger/Logger.hpp"
#include "../Containers/DynamicArray.hpp"


namespace Ryujin
{
	class IListener
	{
	public:
		VIRTUAL void HandleNotification() = 0;
	};

	class Notifier
	{
	private:
		DynamicArray<IListener*> listeners;

	protected:
		VIRTUAL void Notify()
		{
			for (auto& listener : listeners)
			{
				listener->HandleNotification();
			}
		}

	public:
		VIRTUAL void Register(IListener* l)
		{
			listeners.Add(l);
		}

		VIRTUAL void Unregister(IListener* l)
		{
			for (uint32 i = 0; i < listeners.GetSize(); ++i)
			{
				if (listeners[i] == l)
				{
					listeners.Remove(i);
					break;
				}
			}
		}
	};
}