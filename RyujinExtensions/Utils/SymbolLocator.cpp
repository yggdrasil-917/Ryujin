

#include "SymbolLocator.hpp"


namespace Ryujin
{
    TocEntry::TocEntry()
    :
    offset(0)
    {
        
    }
    
    TocEntry::TocEntry(const StringHash& inStringId, const PtrSize inOffset, const PtrSize inEntryByteSize)
    :
    stringId(inStringId),
    offset(inOffset),
    entryByteSize(inEntryByteSize)
    {
        
    }
    
    TableOfContent::TableOfContent()
    :
    tableByteSize(0)
    {
        
    }
    
    TableOfContent::~TableOfContent()
    {
        blob.Close();
    }
    
    bool TableOfContent::InitializeFromFile(const char *tocFile)
    {
        if (!blob.Open(tocFile))
        {
            return false;
        }
        
        int32 count = 0;
        blob.Next<int32>(&count, 1);
        if (count > 0)
        {
            tableByteSize = count * sizeof(TocEntry);
            TocEntry* entries = static_cast<TocEntry*>(Memory::Malloc(tableByteSize));
            blob.Next<TocEntry>(entries, count);
            table.Copy(entries, count);
            Memory::Free(entries);
        }
        
        return true;
    }
    
    void TableOfContent::AddEntry(const TocEntry &entry)
    {
        table.Add(entry);
        tableByteSize += sizeof(TocEntry);
    }
    
    TocEntry TableOfContent::GetEntry(int32 index) const
    {
        return table[index];
    }
}
