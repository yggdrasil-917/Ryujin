#pragma once


#include "../Renderer/RendererBase.hpp"


namespace Ryujin
{
    enum ShaderStage
    {
        SS_Vertex_Bit = 0x01,
        SS_Fragment_Bit = 0x02,
        SS_Geometry_Bit = 0x04,
        SS_Domain_Bit = 0x08,
        SS_Hull_Bit = 0x10,
        SS_Compute_Bit = 0x20
    };
    
    class ShaderBase
    {
    protected:
        ShaderStage stage;
        const char* name;
        
    public:
        
    };
}
