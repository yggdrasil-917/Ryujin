
#include "IShaderCompiler.hpp"

#if PLATFORM_MAC
#include "../Metal/MetalShaderCompiler.hpp"
#endif

using namespace Ryujin;

int main(int argc, const char * argv[])
{
    IShaderCompiler* compiler = nullptr;
#if PLATFORM_MAC
    compiler = RYUJIN_NEW MetalShaderCompiler();
#elif PLATFORM_WINDOWS
    bool bisVulkan = false;
    for (int32 i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "vulkan") == 0)
        {
            bIsVulkan = true;
            break;
        }
        else if (strcmp(argv[i], "dx12") == 0)
        {
            bIsVulkan = false;
            break;
        }
    }
    compiler = bisVulkan ? RYUJIN_NEW VulkanShaderCompiler() : RYUJIN_NEW DX12ShaderCompiler();
#endif
    
    RYUJIN_ASSERT(compiler, "Null shader compiler");
    
    compiler->SetupCompilerEnvironment();
    
    DynamicArray<String> shadersToCompile;
    for (int32 i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-shaders") == 0)
        {
            ++i;
            while (i < argc && argv[i][0] != '-')
            {
                shadersToCompile.Add(argv[i]);
                ++i;
            }
            --i;
        }
    }
    
    bool result = compiler->CompileShaders(shadersToCompile);
    
    return result ? 0 : -1;
}
