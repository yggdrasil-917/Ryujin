
#include "Visibility.hpp"

#include "RyujinCore/Allocators/GlobalAllocators.hpp"
#include "RyujinCore/Math/MathUtils.hpp"

namespace Ryujin
{
    VisibilityData::VisibilityData()
    :
    blob(nullptr),
    numVisibles(0),
    capacity(0)
    {
        
    }
    
    VisibilityData::~VisibilityData()
    {
        if (blob)
        {
            DefaultEngineAllocator::Instance().Free(blob);
            blob = nullptr;
        }
        numVisibles = 0;
        capacity = 0;
    }
    
    void VisibilityData::Allocate(uint32 numEntries)
    {
        uint32 numBytes = MathUtils::Max<uint32>(numEntries / 8, PLATFORM_ALIGNMENT);
        if (numBytes == capacity)
            return;
        
        ubyte* newBlob = static_cast<ubyte*>(DefaultEngineAllocator::Instance().Allocate(numBytes));
        if (blob)
        {
            Memory::MemCpy(newBlob, blob, capacity);
            DefaultEngineAllocator::Instance().Free(blob);
        }
        
        blob = newBlob;
        capacity = numBytes;
    }
    
    void VisibilityData::SetVisibility(uint32 index, bool bIsVisible)
    {
        uint32 outer = index / sizeof(ubyte);
        uint32 bitPosition = index % sizeof(ubyte);
        
        if (bIsVisible)
        {
            blob[outer] |= 1u << bitPosition;
        }
        else
        {
            blob[outer] &= ~(1u << bitPosition);
        }
    }
    
    bool VisibilityData::IsVisible(uint32 index) const
    {
        uint32 outer = index / sizeof(ubyte);
        uint32 bitPosition = index % sizeof(ubyte);
        
        return ( (blob[outer] >> bitPosition) & 0x1 ) != 0;
    }
}