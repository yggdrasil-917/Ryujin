
#include "Process.hpp"

#if PLATFORM_MAC

#include "../Containers/String.hpp"
#include "FileSystem.hpp"

#include <mach-o/dyld.h>
#include <libgen.h>


namespace Ryujin
{
    void Process::RunProcess(const char* pathToExecutable, const char* arguments, bool bCaptureOutput, bool bWait)
    {
        String command = String::Printf("%s %s", pathToExecutable, arguments);
        if (bCaptureOutput)
        {
            command += " 2>&1";
        }
        
        FileHandle ret = popen(*command, "r");
//        if (!bWait)
//        {
//            int d = fileno(ret);
//            fcntl(d, F_SETFL, O_NONBLOCK);
//        }
        if (ret && bCaptureOutput)
        {
            char line[1024];
            while (File::ReadLine(line, 1024, ret))
            {
                Debugf(Info, "%s", line);
            }
        }
        else
        {
            Debugf(Info, "Failed to open pipe running %s", *command);
            return;
        }
        int pcloseResult = pclose(ret);
        if (WIFEXITED(pcloseResult))
        {
            returnCode = WEXITSTATUS(pcloseResult);
        }
        else
        {
            returnCode = -1;
            Debugf(Info, "Process exited abnormally");
        }
    }
}

#endif
