
#ifndef COMPONENT_MANAGER_COMMON_HPP
#define COMPONENT_MANAGER_COMMON_HPP


#include "RyujinCore/Containers/DynamicArray.hpp"
#include "RyujinCore/Containers/SparseArray.hpp"
#include "RyujinCore/Containers/HashTable.hpp"
#include "RyujinCore/Containers/SlotArray.hpp"
#include "RyujinCore/Containers/String.hpp"


namespace Ryujin
{
    using Entity = Handle<16, 16>;
    
    struct EntityHash
    {
        uint64 operator ()(const Entity& key, uint32 tableSize) const
        {
            return uint32(key) % tableSize;
        }
    };
    
    
    using EntityArray = SparseArray<Entity, Entity>;
}


#endif
