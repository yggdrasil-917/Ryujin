
#ifndef VISIBILITY_HPP
#define VISIBILITY_HPP


#include "RyujinCore/Allocators/Allocator.hpp"



namespace Ryujin
{
    class VisibilityData
    {
    private:
        ubyte* blob;
        uint32 numVisibles;
        uint32 capacity;
        
    public:
        VisibilityData();
        ~VisibilityData();
        
        void Allocate(uint32 numEntries);
        
        void SetVisibility(uint32 index, bool bIsVisible);
        bool IsVisible(uint32 index) const;
    };
}



#endif