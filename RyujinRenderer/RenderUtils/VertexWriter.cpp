

#include "VertexWriter.hpp"
#include "RyujinCore/OS/Platform.hpp"
#include "RyujinCore/Math/MathUtils.hpp"
#include "RyujinCore/Allocators/Memory.hpp"

namespace Ryujin
{
    uint8* VertexWriter::Write(uint8 *dst, float x)
    {
        RYUJIN_ASSERT(dst, "Null destination buffer writing vertex data");
        
        float* data = (float*)dst;
        *data++ = x;
        
        return (uint8*)data;
    }
    
    uint8* VertexWriter::Write(uint8 *dst, VertexAttrType type, float x, float y)
    {
        RYUJIN_ASSERT(dst, "Null destination buffer writing vertex data");
        
        switch (type)
        {
            case VertexAttrType::Float2:
            {
                float* data = (float*)dst;
                *data++ = x;
                *data++ = y;
                return (uint8*)data;
            }
                
            case VertexAttrType::Short2:
            {
                int16* data = (int16*)dst;
                int16 ix = MathUtils::Clamp<int16>((int16)x, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                int16 iy = MathUtils::Clamp<int16>((int16)y, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                *data++ = ix;
                *data++ = iy;
                return (uint8*)data;
            }
                
            default:
                RYUJIN_ASSERT(0, "Unknown vertex attribute type encountered");
                return nullptr;
        }
    }
    
    uint8* VertexWriter::Write(uint8 *dst, Ryujin::VertexAttrType type, float x, float y, float z)
    {
        RYUJIN_ASSERT(dst, "Null destination buffer writing vertex data");
        
        switch (type)
        {
            case VertexAttrType::Float3:
            {
                float* data = (float*)dst;
                *data++ = x;
                *data++ = y;
                *data++ = z;
                return (uint8*)data;
            }
                
            case VertexAttrType::Short3:
            {
                int16* data = (int16*)dst;
                int16 ix = MathUtils::Clamp<int16>((int16)x, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                int16 iy = MathUtils::Clamp<int16>((int16)y, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                int16 iz = MathUtils::Clamp<int16>((int16)z, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                *data++ = ix;
                *data++ = iy;
                *data++ = iz;
                return (uint8*)data;
            }
                
            default:
                RYUJIN_ASSERT(0, "Unknown vertex attribute type encountered");
                return nullptr;
        }
    }
    
    uint8* VertexWriter::Write(uint8 *dst, Ryujin::VertexAttrType type, float x, float y, float z, float w)
    {
        RYUJIN_ASSERT(dst, "Null destination buffer writing vertex data");
        
        switch (type)
        {
            case VertexAttrType::Float4:
            {
                float* data = (float*)dst;
                *data++ = x;
                *data++ = y;
                *data++ = z;
                *data++ = w;
                return (uint8*)data;
            }
                
            case VertexAttrType::Short4:
            {
                int16* data = (int16*)dst;
                int16 ix = MathUtils::Clamp<int16>((int16)x, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                int16 iy = MathUtils::Clamp<int16>((int16)y, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                int16 iz = MathUtils::Clamp<int16>((int16)z, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                int16 iw = MathUtils::Clamp<int16>((int16)w, NumericLimit<int16>::Min(), NumericLimit<int16>::Max());
                *data++ = ix;
                *data++ = iy;
                *data++ = iz;
                *data++ = iw;
                return (uint8*)data;
            }
                
            default:
                RYUJIN_ASSERT(0, "Unknown vertex attribute type encountered");
                return nullptr;
        }
    }
    
    uint8* VertexWriter::Write(uint8* dst, uint8* src, PtrSize bytes)
    {
        uint8* data = dst;
        Memory::MemCpy(data, src, bytes);
        data = (data + bytes);
        
        return data;
    }
}
