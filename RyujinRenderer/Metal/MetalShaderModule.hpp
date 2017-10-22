#pragma once


#include "../Common/PipelineStateBase.hpp"
#import <Metal/Metal.h>


namespace Ryujin
{
    class RENDER_API MetalShaderModule
    {
    protected:
        id<MTLLibrary> handle;
        
    public:
        VIRTUAL ~MetalShaderModule() {}
        void Create(const char* shaderSource);
        void CreateFromFile(const char* filepath);
        void Create(const void* binaryData, uint32 size);
        
        bool InitNewGraphicsShader(class MetalGraphicsShader& shader, const char* functionName);
        bool InitNewComputeShader(class MetalComputeShader& shader, const char* functionName);
    };
}
