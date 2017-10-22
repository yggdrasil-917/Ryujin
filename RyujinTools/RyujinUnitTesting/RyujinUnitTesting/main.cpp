#include "RyujinUnitTesting.hpp"
#include "RyujinCore/CoreUtils/Timer.hpp"
#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/OS/Application.hpp"
#include "RyujinCore/OS/FileSystem.hpp"

using namespace Ryujin;

int main(int argc, const char** argv)
{
    // Delete old logs
    String fileLogDir = String::Printf("%sLogs/", AppInfo::GetAppDataDir());
    DynamicArray<String> outFiles;
    if (Directory::GetFiles(*fileLogDir, outFiles))
    {
        for (auto& file : outFiles)
        {
            if (!file.StartsWith("RyujinUnitTesting"))
            {
                continue;
            }
            String filepath = fileLogDir + file;
            if (File::IsFileOlderThanNDays(*filepath, 1))
            {
                Debugf(Info, "Deleting log file %s", *file);
                File::Delete(*filepath);
            }
        }
    }
    
    UnitTestRunner unitTests;
    
    Timer timer(true);
    unitTests.RunTests();
    timer.Stop();
    Debugf(Info, "RyujinUnitTesting ran in %.4f seconds", timer.ElapsedSeconds());
    
    UnitTestRegistry::CleanRegistry();
    
    return 0;
}
