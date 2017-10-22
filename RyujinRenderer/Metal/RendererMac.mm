
#include "../Renderer/Renderer.hpp"

#include "RyujinCore/Window/Viewport.hpp"
#include "RyujinCore/Window/WindowMac.hpp"

#include "MetalSwapChain.hpp"

namespace Ryujin
{
    void Renderer::PlatformSpecificInitialization()
    {
        RyujinWindow* ryujinWindow = (RyujinWindow*)viewport->GetWindow()->GetWindowHandle();
        RYUJIN_ASSERT(ryujinWindow, "Null window detected");
        
        id<MTLDevice> mtlDevice = device->GetMetalDevice();
        CAMetalLayer* metalLayer = device->GetSwapChain()->GetMetalLayer();

        [ryujinWindow CacheDevice:mtlDevice];

        metalLayer.frame = [ryujinWindow contentView].bounds;
        [[ryujinWindow contentView].layer addSublayer:metalLayer];
    }
}
