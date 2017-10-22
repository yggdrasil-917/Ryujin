#include "MetalShaderCompiler.hpp"

#include "RyujinCore/Threading/JobSystem.hpp"
#include "RyujinCore/OS/Process.hpp"
#include "RyujinCore/OS/FileSystem.hpp"

namespace Ryujin
{
    void MetalShaderCompiler::SetupCompilerEnvironment()
    {
        String wd = Directory::GetCurrentDirectoryPath();
        compilerPath = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/usr/bin";
        outputPath = String::Printf("%s/../Shaders/", *wd);
        outputPathIR = String::Printf("%s/../ShadersIR/", *wd);
        shaderPreprocessors = "#define COMPILING_METAL 1\n#include <metal_stdlib>\n#include <simd/simd.h>\nusing namespace metal;\n";
        shaderFolder = String::Printf("%s/../../../RyujinRenderer/Shaders/", *wd);
    }
    
    String MetalShaderCompiler::AssembleShaderSource(const char* shaderFile) const
    {
//        static String headersToIgnore[] = { "#include <metal_stdlib>\n", "#include <simd/simd.h>\n" };
        String shaderSrc;
        FileHandle file = File::Open(shaderFile, FileMode::FM_Read);
        if (!file)
        {
            Debugf(Info, *String::Printf("Failed to open shader %s", shaderFile));
            return shaderSrc;
        }
        
        char line[1024];
        while (File::ReadLine(line, 1024, file))
        {
            if (line[0] == '#' && line[1] == 'i' && line[2] == 'n') // assume it's a #include
            {
                char headerFile[512];
                sscanf(line, "#include \"%[^\"]\"", headerFile);
//                bool bAppendFile = true;
//                for (int32 i = 0; i < countof(headersToIgnore); ++i)
//                {
//                    if (headersToIgnore[i] == line)
//                    {
//                        shaderSrc += line;
//                        bAppendFile = false;
//                        break;
//                    }
//                }
//                if (bAppendFile)
                    shaderSrc += AssembleShaderSource(*String::Printf("%s%s", GetShaderFolder(), headerFile));
            }
            else
            {
                shaderSrc += line;
            }
        }
        File::Close(file);
        
        return shaderSrc;
    }
    
    static bool CompileToIRAndLink(const String& shaderFile, const MetalShaderCompiler* compiler)
    {
        String metalShaderFile = shaderFile;
        metalShaderFile.Replace(shaderFile.Length() - 4, 6, ".metal");
        String shaderPath = String::Printf("%s%s", compiler->GetShaderFolder(), *shaderFile);
        String shaderFileIR = shaderFile;
        shaderFileIR.Replace(shaderFileIR.Length() - 4, 4, ".air");
        String shaderPathIR = String::Printf("%s%s", compiler->GetOutputPathIR(), *shaderFileIR);
        
        FileHandle file = File::Open(*shaderPath, FileMode::FM_Read);
        if (!file)
        {
            Debugf(Info, "Shader file not found %s", *shaderFile);
            return false;
        }
        
        String shaderSrc = compiler->GetShaderPreprocessors();
        shaderSrc += compiler->AssembleShaderSource(*shaderPath);
        String metalShaderPath = String::Printf("%s%s", compiler->GetOutputPathIR(), *metalShaderFile);
        FileHandle write = File::Open(*metalShaderPath, FileMode::FM_Write);
        if (!write)
        {
            Debugf(Info, "Failed to open file for writing");
            return false;
        }
        File::Write(*shaderSrc, sizeof(char), shaderSrc.Length(), write);
        File::Close(write);
        
        Process shaderCompiler;
        String arguments = String::Printf("-sdk macosx metal %s -Wno-unused-command-line-argument -arch air64 -c -emit-llvm -gline-tables-only -ffast-math -o %s -mmacosx-version-min=10.12 %s",
                                          compiler->GetOutputPathIR(), *shaderPathIR, *metalShaderPath);
        shaderCompiler.RunProcess("xcrun", *arguments, true);
        int32 code = shaderCompiler.GetExitCode();
        if (code == 1) //xcrun returns 1 if it failed and 0 otherwise
        {
            return false;
        }
        
        // Link and create the metallib file
        String metalLibFile = shaderFile;
        metalLibFile.Replace(shaderFile.Length() - 4, 9, ".metallib");
        metalLibFile = String::Printf("%s%s", compiler->GetOutputPath(), *metalLibFile);
        Process shaderLinker;
        arguments = String::Printf("-sdk macosx metallib -split-module %s -o %s", *shaderPathIR, *metalLibFile);
        shaderLinker.RunProcess("xcrun", *arguments, true);
        code = shaderLinker.GetExitCode();
        if (code == 1)
        {
            return false;
        }
        
        return true;
    }
    
    bool MetalShaderCompiler::CompileShaders(const DynamicArray<String> &shadersToCompile)
    {
        JobSystem jobSystem;
        jobSystem.Initialize(Threading::NumConcurrentThreadsSupported() - 1);
        
        const JobId root = jobSystem.ParallelFor<String>(shadersToCompile.GetTypedData(), shadersToCompile.GetSize(), this,
                                                        [](uint8* data, uint32 count, void* other)
                                                        {
                                                            String compileToIR = "xcrun -sdk macosx metal";
                                                            String compileToLib = "xcrun -sdk macosx metallib";
                                                            String* a = (String*)data;
                                                            MetalShaderCompiler* compiler = (MetalShaderCompiler*)other;
                                                            for (uint32 i = 0; i < count; ++i)
                                                            {
                                                                // Convert rsf file to .metal extension and make a metallib file
                                                                CompileToIRAndLink(a[i], compiler);
                                                            }
                                                        },
                                                        [](uint32 count, uint32 elementSize) -> bool
                                                        {
                                                            return count > 2;
                                                        });
        
        jobSystem.Schedule(root);
        jobSystem.Wait(root);
        
        jobSystem.Shutdown();
        
        return true;
    }
}
