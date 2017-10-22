
#include "VertexFormat.hpp"
#include "RyujinCore/OS/Platform.hpp"

#include <algorithm>

namespace Ryujin
{
    VertexLayout::Component::Component()
    :
    attr(VertexAttr::Count),
    type(VertexAttrType::Count),
    byteOffset(0)
    {
        
    }
    
    VertexLayout::Component::Component(VertexAttr inAttr, VertexAttrType inType, int32 inByteOffset)
    :
    attr(inAttr),
    type(inType),
    byteOffset(inByteOffset)
    {
        
    }
    
    VertexLayout::VertexLayout()
    :
    numComponents(0),
    byteSize(0),
    bHasInterleavedVertexData(true)
    {
        
    }
    
    VertexLayout& VertexLayout::Add(VertexAttr inAttr, VertexAttrType inType, int32 inByteOffset)
    {
        return Add(Component(inAttr, inType, inByteOffset));
    }
    
    VertexLayout& VertexLayout::Add(const VertexLayout::Component &inComponent)
    {
        RYUJIN_ASSERT(numComponents < MAX_VERT_ATTRIBS, "VertexLayout is full");
        RYUJIN_ASSERT(inComponent.attr != VertexAttr::Count && inComponent.type != VertexAttrType::Count, "Invalid vertex layout component");
        
        component[numComponents] = inComponent;
        byteSize += GetVertexAttrTypeByteSize(inComponent.type);
        
        ++numComponents;
        
        return *this;
    }
    
    void VertexLayout::Finish()
    {
        std::sort(component.begin(), component.begin() + numComponents,
                  [](const Component& a, const Component& b)
                  {
                      return a.byteOffset < b.byteOffset;
                  });
    }
    
    bool VertexLayout::HasAttribute(VertexAttr attrib) const
    {
        for (int32 i = 0; i < numComponents; ++i)
        {
            if (component[i].attr == attrib)
            {
                return true;
            }
        }
        
        return false;
    }
    
    void VertexLayout::Clear()
    {
        numComponents = 0;
        byteSize = 0;
    }
}
