#pragma once

#include "RyujinCore/Containers/DynamicArray.hpp"
#include "RyujinCore/Containers/String.hpp"


namespace Ryujin
{
    class IShaderCompiler
    {
    protected:
        DynamicArray<String> compilerErrors;
        String compileOptions;
        String compilerPath;
        String shaderFolder;
        String outputPath;
        String outputPathIR;
        String shaderPreprocessors;
        
    public:
        VIRTUAL void SetupCompilerEnvironment() = 0;
        VIRTUAL bool CompileShaders(const DynamicArray<String>& shadersToCompile) = 0;
        
        VIRTUAL String AssembleShaderSource(const char* shaderFile) const = 0;
        
        FORCEINLINE const char* GetShaderFolder() const { return shaderFolder; }
        FORCEINLINE const char* GetOutputPath() const { return outputPath; }
        FORCEINLINE const char* GetOutputPathIR() const { return outputPathIR; }
        FORCEINLINE const char* GetShaderPreprocessors() const { return shaderPreprocessors; }
    };
}
