#include "AssetRegistry.hpp"


namespace Ryujin
{
    JobId AssetRegistry::LoadAssetsAsync(JobSystem* jobSystem, const DynamicArray<AssetDescriptor>& assetsToLoad)
    {
        JobId jobId = jobSystem->ParallelFor<AssetDescriptor>(assetsToLoad.GetTypedData(), assetsToLoad.GetSize(), this,
                                                              [](uint8* data, uint32 count, void* other)
                                                              {
                                                                  AssetRegistry* ar = (AssetRegistry*)other;
                                                                  AssetDescriptor* descs = (AssetDescriptor*)(data);
                                                                  if (ar && descs)
                                                                  {
                                                                      for (uint32 i = 0; i < count; ++i)
                                                                      {
                                                                          AssetPtr asset = ar->CacheAsset(descs[i]);
                                                                          descs[i].OnAssetLoaded(asset);
                                                                      }
                                                                  }
                                                              },
                                                              [](uint32 count, uint32 elementSize) -> bool
                                                              {
                                                                  return count > 2;
                                                              });
        
        return jobId;
    }
    
    AssetPtr AssetRegistry::CacheAsset(AssetDescriptor& assetDesc)
    {
//       String file = String::Printf("%s%s", AppInfo::GetCookedContentDir(), filename);
//       StringHash hash(*file);
        StringHash hash(assetDesc.name); // Use filename instead of entire path as the hash
        
        ScopedLock lock(registryMutex);
        
        WeakAssetPtr asset;
        if (registry.TryGetValue(hash.GetHash(), asset))
        {
            return asset.Lock();
        }
        
        if (AssetPtr ptr = RemoveAssetFromDeletion(hash.GetHash()))
        {
            registry.Add(hash.GetHash(), WeakAssetPtr(ptr));
            return ptr;
        }
        
        // Asset not yet cached so load it
        AssetPtr newAsset(ResourceFactory::Create(assetDesc.classId, assetDesc.name, assetDesc.desc));
        if (newAsset)
        {
            registry.Add(hash.GetHash(), WeakAssetPtr(newAsset));
            newAsset->SetAssetRegistryOwner(this);
        }
        
        return newAsset;
    }
    
    void AssetRegistry::Tick(float delta)
    {
        uint64 currentTime = Time::TimeSinceEpoch();
        for (int32 i = deferredResourcesToDelete.GetSize() - 1; i >= 0; --i)
        {
            if (currentTime - deferredResourcesToDelete[i]->GetTimeQueuedForDeletion() >= ResourceBase::MAX_INACTIVE_DURATION)
            {
                SAFE_DELETE(deferredResourcesToDelete[i]);
                deferredResourcesToDelete.RemoveSlow(i);
            }
            else
            {
                // the elements in the array are ordered with the oldest one being at the start of the array
                // when the first one is found that does not meet the remove criteria then none of the other
                // elements meet that criteria so we skip checking them
                break;
            }
        }
    }
}
