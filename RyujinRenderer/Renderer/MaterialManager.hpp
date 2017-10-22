#pragma once


#include "Material.hpp"

#include "RyujinCore/Containers/HashTable.hpp"


namespace Ryujin
{
    class RENDER_API MaterialManager
    {
    private:
        HashTable<uint64, Material*> materialCache;
        
    public:
        ~MaterialManager();
        
        Material* GetOrCacheMaterial(const MaterialDescriptor& desc);
    };
}
