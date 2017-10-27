
#ifndef PROCESS_HPP
#define PROCESS_HPP


#include "Platform.hpp"



namespace Ryujin
{
    class CORE_API Process
    {
    private:
        int32 returnCode;
        
    public:
        
        void RunProcess(const char* pathToExecutable, const char* arguments, bool bCaptureOutput, bool bWait = true);
        
        FORCEINLINE int32 GetExitCode() const { return returnCode; }
    };
}


#endif
