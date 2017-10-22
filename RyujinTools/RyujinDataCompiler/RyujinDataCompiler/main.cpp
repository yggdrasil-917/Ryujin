

#include "RyujinDataCompiler.hpp"

using namespace Ryujin;


int main(int argc, const char * argv[])
{
    RyujinDataCompiler compiler;
    for (int32 i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-files") == 0)
        {
            ++i;
            while (i < argc && argv[i][0] != '-')
            {
                compiler.AddFileToCompile(argv[i]);
                ++i;
            }
            --i;
        }
    }
    
    compiler.CompileFiles();
    return 0;
}
