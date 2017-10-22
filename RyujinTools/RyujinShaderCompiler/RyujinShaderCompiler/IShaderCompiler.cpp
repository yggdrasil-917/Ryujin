#include "IShaderCompiler.hpp"

#include "RyujinCore/OS/FileSystem.hpp"


namespace Ryujin
{
//    String IShaderCompiler::AssembleShaderSource(const char *shaderFile)
//    {
//        String shaderSrc;
//        FileHandle file = File::Open(*String::Printf("%s%s", *shaderFolder, shaderFile), FileMode::FM_Read);
//        if (!file)
//        {
//            Debugf(Info, *String::Printf("Failed to open shader %s", shaderFile));
//            return shaderSrc;
//        }
//        
//        char line[1024];
//        while (File::ReadLine(line, 1024, file))
//        {
//            if (line[0] == '#' && line[1] == 'i') // assume it's #include
//            {
//                char headerFile[512];
//                sscanf(line, "#include \"%s\"", headerFile);
//                shaderSrc += AssembleShaderSource(*String::Printf("%s%s", *shaderFolder, headerFile));
//            }
//            else
//            {
//                shaderSrc += line;
//            }
//        }
//        
//        return shaderSrc;
//    }
}
