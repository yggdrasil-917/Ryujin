#pragma once


#include "../Common/SwapChainBase.hpp"
#include "../Common/TextureBase.hpp"
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>


namespace Ryujin
{
    class RENDER_API MetalBackBufferImage : public TextureBase
    {
        friend class MetalSwapChain;
        
    protected:
        id<CAMetalDrawable> drawable;
        
    public:
        MetalBackBufferImage() {}
        VIRTUAL ~MetalBackBufferImage() {}
        
        MetalBackBufferImage(MetalBackBufferImage&& other)
        {
            drawable = Move(other.drawable);
            other.drawable = nil;
        }
        
        MetalBackBufferImage& operator=(MetalBackBufferImage&& other)
        {
            drawable = Move(other.drawable);
            other.drawable = nil;
            return *this;
        }
        void Release();
        FORCEINLINE id<CAMetalDrawable> GetHandle() const { return drawable; }
    };
    
    class RENDER_API MetalSwapChain : public SwapChainBase
    {
    protected:
        CAMetalLayer* metalLayer;
        
    public:
        VIRTUAL ~MetalSwapChain() {}
        
        void Create(uint16 inWidth, uint16 inHeight, uint32 numImages);
        MetalBackBufferImage* GetNextImage();
        
        FORCEINLINE CAMetalLayer* GetMetalLayer() const { return metalLayer; }
    };
}
