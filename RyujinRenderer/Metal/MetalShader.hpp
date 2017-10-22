#pragma once


#include "../Common/ShaderBase.hpp"
#import <Metal/Metal.h>

#define SHADER_EXT ".metal"

namespace Ryujin
{
    class RENDER_API MetalShader : public ShaderBase
    {
        friend class MetalShaderModule;
        
    protected:
        id<MTLFunction> function;
        
    public:
        VIRTUAL ~MetalShader() {}
        FORCEINLINE id<MTLFunction> GetHandle() const { return function; }
    };
    
    class RENDER_API MetalGraphicsShader : public MetalShader
    {
        
    };
    
    class RENDER_API MetalComputeShader : public MetalShader
    {
        
    };
}
