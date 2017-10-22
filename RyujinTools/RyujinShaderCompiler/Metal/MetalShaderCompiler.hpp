#pragma once


#include "../RyujinShaderCompiler/IShaderCompiler.hpp"


namespace Ryujin
{
    class MetalShaderCompiler : public IShaderCompiler
    {
    public:
        void SetupCompilerEnvironment() OVERRIDE FINAL;
        bool CompileShaders(const DynamicArray<String>& shadersToCompile) OVERRIDE FINAL;
        String AssembleShaderSource(const char* shaderFile) const OVERRIDE FINAL;
    };
}
