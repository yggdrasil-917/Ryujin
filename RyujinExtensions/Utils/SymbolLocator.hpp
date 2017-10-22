
#pragma once


#include "RyujinCore/Containers/DynamicArray.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"

#include "ExtensionsBase.hpp"

namespace Ryujin
{
    struct EXTENSIONS_API TocEntry
    {
        StringHash stringId;
        PtrSize offset;
        PtrSize entryByteSize;
        
        TocEntry();
        TocEntry(const StringHash& inStringId, const PtrSize inOffset, const PtrSize inEntryByteSize);
    };
    
    class EXTENSIONS_API TableOfContent
    {
    private:
        DynamicArray<TocEntry> table;
        PtrSize tableByteSize;
        BinaryFileReader blob;
        
    public:
        TableOfContent();
        ~TableOfContent();
        
        bool InitializeFromFile(const char* tocFile);
        void AddEntry(const TocEntry& entry);
        
        TocEntry GetEntry(int32 index) const;
        FORCEINLINE const DynamicArray<TocEntry>& GetTable() const { return table; }
        
        
        template<class T>
        FORCEINLINE T* LookupSymbol(const char* symbolName)
        {
            if (!blob.IsOpen())
            {
                return nullptr;
            }
            
            StringHash id(symbolName);
            for (const auto& entry : table)
            {
                if (id.GetHash() == entry.stringId.GetHash())
                {
                    T* instance = RYUJIN_NEW T();
                    blob.NextFrom<T>(instance, 1, tableByteSize + sizeof(int32) + entry.offset);
                    return instance;
                }
            }
            
            return nullptr;
        }
    };
}
