
#include "MetalSwapChain.hpp"
#include "MetalDevice.hpp"

namespace Ryujin
{
    void MetalBackBufferImage::Release()
    {
        if (drawable)
        {
            [drawable release];
        }
    }
    
    void MetalSwapChain::Create(uint16 inWidth, uint16 inHeight, uint32 numImages)
    {
        width = inWidth;
        height = inHeight;
        
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        metalLayer = [CAMetalLayer layer];
        metalLayer.device = device;
        metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        metalLayer.framebufferOnly = false;
    }
    
    MetalBackBufferImage* MetalSwapChain::GetNextImage()
    {
        MetalBackBufferImage* image = RYUJIN_NEW MetalBackBufferImage();
        image->drawable = nil;
        while (image->drawable == nil)
        {
            image->drawable = [metalLayer nextDrawable];
        }
        return image;
    }
}
