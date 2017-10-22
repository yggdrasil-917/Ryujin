#pragma once

#include "RyujinCore/CoreUtils/SmartPointer.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"

#include "RyujinCore/Threading/Thread.hpp"
#include "ResourceFactory.hpp"

#include "../Utils/Tickable.hpp"

#include "RyujinCore/Threading/JobSystem.hpp"


namespace Ryujin
{
    class AssetDeleter;
    
    using WeakAssetPtr = WeakPtrConcurrent<ResourceBase, AssetDeleter>;
    using AssetPtr = SharedPtrConcurrent<ResourceBase, AssetDeleter>;
    
    
    struct AssetDescriptor
    {
        char desc[256];
        char name[32];
        uint32 classId;
        AssetPtr* owner;
        
        template<typename ResourceType>
        FORCEINLINE void Setup(const char* inName, uint32 inClassId, ResourceType& resource)
        {
#if DEBUG
            uint32 length = std::strlen(inName) + 1;
            RYUJIN_ASSERT(length <= sizeof(name), "Passed in asset name is too long");
            RYUJIN_STATIC_ASSERT(sizeof(ResourceType) <= sizeof(desc), "Resource type is too big");
#endif
            
            std::strcpy(name, inName);
            classId = inClassId;
            Memory::MemCpy(desc, &resource, sizeof(ResourceType));
        }
        
        void OnAssetLoaded(AssetPtr loadedAsset)
        {
            RYUJIN_ASSERT(owner, "Asset owner is null");
            *owner = loadedAsset;
        }
    };
    
    
    class EXTENSIONS_API AssetRegistry : public Tickable
    {
    private:
        HashTable<uint32, WeakAssetPtr> registry;
        DynamicArray<ResourceBase*> deferredResourcesToDelete;
        Mutex registryMutex;
        
        
        FORCEINLINE AssetPtr RemoveAssetFromDeletion(uint32 hash)
        {
            for (int32 i = deferredResourcesToDelete.GetSize() - 1; i >= 0; --i)
            {
                if (deferredResourcesToDelete[i]->GetUniqueId() == hash)
                {
                    ResourceBase* resource = deferredResourcesToDelete[i];
                    deferredResourcesToDelete[i] = nullptr;
                    deferredResourcesToDelete.RemoveSlow(i);
                    return AssetPtr(resource);
                }
            }
            
            return AssetPtr(nullptr);
        }
        
        
    public:
        
        JobId LoadAssetsAsync(JobSystem* jobSystem, const DynamicArray<AssetDescriptor>& assetsToLoad);
        
        AssetPtr CacheAsset(AssetDescriptor& assetDesc);
        
        FORCEINLINE AssetPtr GetAsset(uint32 hash)
        {
            WeakAssetPtr asset;
            if (registry.TryGetValue(hash, asset))
            {
                return asset.Lock();
            }
            
            if (AssetPtr ptr = RemoveAssetFromDeletion(hash))
            {
                registry.Add(hash, WeakAssetPtr(ptr));
                return ptr;
            }
            
            return AssetPtr(nullptr);
        }
        
        FORCEINLINE void QueueAssetForDeletion(ResourceBase*& resource)
        {
            if (resource)
            {
                registry.Remove(resource->GetUniqueId());
                resource->SetTimeQueuedForDeletion(Time::TimeSinceEpoch());
                deferredResourcesToDelete.Add(Move(resource));
                resource = nullptr;
            }
        }
        
        void Tick(float delta);
    };
    
    class AssetDeleter
    {
    public:
        static void Delete(ResourceBase*& obj)
        {
            if (obj)
            {
                obj->GetRegistryOwner()->QueueAssetForDeletion(obj);
            }
        }
    };
}
