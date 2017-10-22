#include "Tickable.hpp"


namespace Ryujin
{
	DynamicArray<Tickable*> Tickable::tickableObjects;

	void UpdateTickables(float dt)
	{
		for (auto& tickable : Tickable::tickableObjects)
		{
			if (tickable)
			{
				tickable->Tick(dt);
			}
		}
	}
}