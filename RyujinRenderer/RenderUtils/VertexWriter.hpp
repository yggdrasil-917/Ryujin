
#pragma once


#include "../Renderer/VertexFormat.hpp"




namespace Ryujin
{
    class RENDER_API VertexWriter
    {
    public:
        static uint8* Write(uint8* dst, float x);
        static uint8* Write(uint8* dst, VertexAttrType type, float x, float y);
        static uint8* Write(uint8* dst, VertexAttrType type, float x, float y, float z);
        static uint8* Write(uint8* dst, VertexAttrType type, float x, float y, float z, float w);
        static uint8* Write(uint8* dst, uint8* src, PtrSize bytes);
    };
}

