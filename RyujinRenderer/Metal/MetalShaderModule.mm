#include "MetalShaderModule.hpp"

#include "MetalDevice.hpp"
#include "MetalShader.hpp"

namespace Ryujin
{
    void MetalShaderModule::Create(const void *binaryData, uint32 size)
    {
        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        dispatch_data_t data = dispatch_data_create(binaryData, size, queue, nullptr);
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        
        NSError *err;
        handle = [device newLibraryWithData:data error:&err];
    }
    
    void MetalShaderModule::Create(const char* shaderSource)
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        NSError* err;
        handle = [device newLibraryWithSource:[NSString stringWithUTF8String:shaderSource] options:nil error:&err];
    }
    
    void MetalShaderModule::CreateFromFile(const char* filepath)
    {
        id<MTLDevice> device = MetalDevice::Instance()->GetMetalDevice();
        NSError* err;
        handle = [device newLibraryWithFile:[NSString stringWithUTF8String:filepath] error:&err];
    }
    
    bool MetalShaderModule::InitNewGraphicsShader(class MetalGraphicsShader &shader, const char *functionName)
    {
        NSString* str = [NSString stringWithUTF8String:functionName];
        shader.name = functionName;
        shader.function = [handle newFunctionWithName:str];
        return shader.function != nil;
    }
    
    bool MetalShaderModule::InitNewComputeShader(class MetalComputeShader &shader, const char *functionName)
    {
        shader.function = [handle newFunctionWithName:[NSString stringWithUTF8String:functionName]];
        return shader.function != nil;
    }
}
