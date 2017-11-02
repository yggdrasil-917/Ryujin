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
		PixelFormat format;
		bool bIsCubemap;
		bool bHasMips;
        
    public:
        VIRTUAL ~TextureBase() {}
        
        FORCEINLINE uint16 GetWidth() const { return width; }
        FORCEINLINE uint16 GetHeight() const { return height; }
        FORCEINLINE uint16 GetDepth() const { return depth; }
		FORCEINLINE uint32 GetMipCount() const { return bHasMips ? MathUtils::Log2(MathUtils::Max(width, height)) + 1 : 1; }
    };
}
