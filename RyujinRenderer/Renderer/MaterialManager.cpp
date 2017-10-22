#include "MaterialManager.hpp"


namespace Ryujin
{
    MaterialManager::~MaterialManager()
    {
        for (HashTable<uint64, Material*>::Iterator it(materialCache); it; ++it)
        {
            delete it->value;
        }
    }
    
    Material* MaterialManager::GetOrCacheMaterial(const MaterialDescriptor& desc)
    {
        uint64 hash = desc.Hash();
        Material* mat = nullptr;
        if (materialCache.TryGetValue(hash, mat))
        {
            return mat;
        }
        
        // Create it and add to cache
        return mat;
    }
}
