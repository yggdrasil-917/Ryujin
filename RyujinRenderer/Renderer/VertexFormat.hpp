
#ifndef VERTEX_FORMAT_HPP
#define VERTEX_FORMAT_HPP


#include "RyujinCore/Containers/FixedArray.hpp"

#include "RendererBase.hpp"

namespace Ryujin
{
    enum class VertexAttr : uint8
    {
        Position,
        Normal,
        UV0,
        UV1,
        Tangent,
        Color,
        
        Count
    };
    
    enum class VertexAttrType : uint8
    {
        Float2,
        Float3,
        Float4,
        
        Short2,
        Short3,
        Short4,
        
        Byte2,
        Byte3,
        Byte4,
        
        Ubyte2,
        Ubyte3,
        Ubyte4,
        
        Count
    };
    
    static int32 GetVertexAttrTypeByteSize(VertexAttrType inType)
    {
        switch (inType)
        {
            case VertexAttrType::Float2:
                return sizeof(float) * 2;
                
            case VertexAttrType::Float3:
                return sizeof(float) * 3;
                
            case VertexAttrType::Float4:
                return sizeof(float) * 4;
                
            case VertexAttrType::Short2:
                return sizeof(int16) * 2;
                
            case VertexAttrType::Short3:
                return sizeof(int16) * 3;
                
            case VertexAttrType::Short4:
                return sizeof(int16) * 4;
                
            default:
                return 0;
        }
    }
    
    
    class RENDER_API VertexLayout
    {
    public:
        class Component
        {
        public:
            VertexAttr attr;
            VertexAttrType type;
            int32 byteOffset;
            
            Component();
            Component(VertexAttr inAttr, VertexAttrType inType, int32 inByteOffset);
        };
        
    private:
        FixedArray<Component, MAX_VERT_ATTRIBS> component;
        int32 numComponents;
        int32 byteSize;
        bool bHasInterleavedVertexData;
        
    public:
        VertexLayout();
        
        VertexLayout& Add(VertexAttr inAttr, VertexAttrType inType, int32 inByteOffset);
        VertexLayout& Add(const Component& inComponent);
        void Finish();
        bool HasAttribute(VertexAttr attrib) const;
        void SetInterleavedVertexDataFlag(bool newValue) { bHasInterleavedVertexData = newValue; }
        void UpdateComponentByteOffset(int32 index, int32 newOffset) { component[index].byteOffset = newOffset; }
        
        void Clear();
        
        FORCEINLINE int32 GetNumComponents() const { return numComponents; }
        FORCEINLINE int32 GetByteSize() const { return byteSize; }
        FORCEINLINE int32 GetComponentByteOffset(int32 index) const { return component[index].byteOffset; }
        FORCEINLINE int32 GetComponentByteSize(int32 index) const { return GetVertexAttrTypeByteSize(component[index].type); }
        FORCEINLINE bool HasInterleavedVertexData() const { return bHasInterleavedVertexData; }
    };
}


#endif
