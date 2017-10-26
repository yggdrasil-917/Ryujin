#pragma once


#include "../Renderer/RendererBase.hpp"


namespace Ryujin
{
    class RENDER_API SwapChainBase
    {
    protected:
        uint16 width;
        uint16 height;
    };
}
