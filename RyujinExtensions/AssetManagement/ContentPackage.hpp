#pragma once


#include "RyujinCore/Containers/DynamicArray.hpp"
#include "RyujinCore/Containers/HashTable.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"

#include "ExtensionsBase.hpp"


namespace Ryujin
{
#define CONTENT_PACKAGE_EXT ".ryu"
    
    struct ContentPackageEntry
    {
        uint64 byteSize;
        uint64 fileOffset;
        
        ContentPackageEntry()
        : byteSize(0),
        fileOffset(0)
        {
            
        }
        
        ContentPackageEntry(uint64 inByteSize, uint64 inFileOffset)
        : byteSize(inByteSize),
        fileOffset(inFileOffset)
        {
            
        }
    };
    
    class ContentPackage
    {
    private:
        HashTable<uint64, ContentPackageEntry> tableOfContent;
        UniquePtr<BinaryReader> blob;
        
        void HydrateTOC();
        
    public:
        ContentPackage();
        ~ContentPackage();
        
        bool Load(const char* filename);
        bool GetAssetDataFromHash(uint64 assetHash, ubyte*& assetData, uint64& assetByteSize);
        
        FORCEINLINE uint64 GetPackageSize() const { return blob->GetSize(); }
    };
}
