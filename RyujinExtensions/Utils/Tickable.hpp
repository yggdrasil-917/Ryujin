#pragma once


#include "RyujinCore/Containers/DynamicArray.hpp"
#include "ExtensionsBase.hpp"

namespace Ryujin
{
    class EXTENSIONS_API Tickable
    {
    public:
        static DynamicArray<Tickable*> tickableObjects;
        
        Tickable()
        {
            tickableObjects.Add(this);
        }
        
        VIRTUAL void Tick(float delta) = 0;
    };

	extern EXTENSIONS_API void UpdateTickables(float dt);
}
