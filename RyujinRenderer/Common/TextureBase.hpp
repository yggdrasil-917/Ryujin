#pragma once



#include "../Renderer/RendererBase.hpp"
#include "ResourceDescriptors.hpp"


namespace Ryujin
{
    class RENDER_API TextureBase
    {
    protected:
        uint16 width;
        uint16 height;
        uint16 depth;
        uint16 arraySize;
        bool bIsCubemap;
        PixelFormat format;
        
    public:
        VIRTUAL ~TextureBase() {}
        
        FORCEINLINE uint16 GetWidth() const { return width; }
        FORCEINLINE uint16 GetHeight() const { return height; }
        FORCEINLINE uint16 GetDepth() const { return depth; }
    };
}
