#include "ContentPackage.hpp"



namespace Ryujin
{
    ContentPackage::ContentPackage()
    {
        
    }
    
    ContentPackage::~ContentPackage()
    {
        
    }
    
    bool ContentPackage::Load(const char *filename)
    {
        if (!filename)
            return false;
        
        blob = UniquePtr<BinaryReader>(RYUJIN_NEW BinaryReader(filename));
        if (!blob)
        {
            return false;
        }
        
        HydrateTOC();
        
        return true;
    }
    
    void ContentPackage::HydrateTOC()
    {
        RYUJIN_ASSERT(blob, "Content package binary data not loaded");
        
        struct TOCEntryKeyValue
        {
            uint64 hash = 0;
            uint64 byteSize = 0;
            uint64 fileOffset = 0;
        };
        blob->SetPosition(0); // make sure read pointer is at the start
        
        uint32 tocEntryCount = blob->Next<uint32>();
        if (tocEntryCount == 0)
            return;
        
        TOCEntryKeyValue* entries = blob->NextArray<TOCEntryKeyValue>(tocEntryCount);
        for (uint32 i = 0; i < tocEntryCount; ++i)
        {
            tableOfContent.Add(entries[i].hash, ContentPackageEntry(entries[i].byteSize, entries[i].fileOffset));
        }
    }
    
    bool ContentPackage::GetAssetDataFromHash(uint64 assetHash, ubyte*& assetData, uint64& assetByteSize)
    {
        ContentPackageEntry entry;
        if (!tableOfContent.TryGetValue(assetHash, entry))
        {
            return false;
        }
        
        assetByteSize = entry.byteSize;
        assetData = (blob->GetData() + entry.fileOffset);
        
        return true;
    }
}
