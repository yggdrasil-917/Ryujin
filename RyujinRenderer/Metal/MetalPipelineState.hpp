#pragma once


#include "../Common/PipelineStateBase.hpp"


#import <Metal/Metal.h>

namespace Ryujin
{
    class RENDER_API MetalPipelineState : public PipelineStateObjectBase
    {
        friend class MetalDevice;
        
    protected:
        id<MTLRenderPipelineState> pipelineState;
        uint64 hash;
        
    public:
        VIRTUAL ~MetalPipelineState() {}
        
        FORCEINLINE id<MTLRenderPipelineState> GetHandle() const { return pipelineState; }
    };
    
    class RENDER_API MetalComputePipelineState : public PipelineStateObjectBase
    {
    protected:
        id<MTLComputePipelineState> handle;
        
    public:
        VIRTUAL ~MetalComputePipelineState() {}
        
        FORCEINLINE id<MTLComputePipelineState> GetHandle() const { return handle; }
    };
}
