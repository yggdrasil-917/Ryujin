#include "Application.hpp"
#include "FileSystem.hpp"

#include <pwd.h>


namespace Ryujin
{
    String* AppInfo::appDir = nullptr;
    String* AppInfo::exeDir = nullptr;
    String* AppInfo::resourceDir = nullptr;
    String* AppInfo::cookedDir = nullptr;
    String* AppInfo::exeName = nullptr;
    String* AppInfo::profilingDir = nullptr;
    String* AppInfo::appDataDir = nullptr;


    const char* AppInfo::GetApplicationDir()
    {
        if (appDir)
        {
            return *(*appDir);
        }

        appDir = RYUJIN_NEW String(GetExeDir());

        return *appDir;
    }

    const char* AppInfo::GetExeDir()
    {
        if (exeDir)
        {
            return *(*exeDir);
        }

        char exe[1024];
        int32 ret;

        ret = readlink("/proc/self/exe",exe,sizeof(exe)-1);
        if(ret ==-1)
        {
            return nullptr;
        }
        exe[ret] = 0;
        int32 i = 0;
        int32 lastSlashIndex = -1;
        while (exe[i])
        {
            if (exe[i] == '/')
            {
                lastSlashIndex = i;
            }
            ++i;
        }

        exeDir = lastSlashIndex != -1 ? RYUJIN_NEW String(String::SubStr(exe, (lastSlashIndex+1))) : RYUJIN_NEW String(exe);

        return *exeDir;
    }

    const char* AppInfo::GetResourcesDir()
    {
        if (resourceDir)
        {
            return *(*resourceDir);
        }

        resourceDir = RYUJIN_NEW String("../../../Content/");

        return *resourceDir;
    }

    const char* AppInfo::GetCookedContentDir()
    {
        if (cookedDir)
        {
            return *(*cookedDir);
        }

        cookedDir = RYUJIN_NEW String(String::Printf("%sCookedLinux/", GetResourcesDir()));

        return *cookedDir;
    }
    
    const char* AppInfo::GetDevCookedContentDir()
    {
        return "../../../Content/CookedLinux/";
    }

    const char* AppInfo::GetExeName()
    {
        if (exeName)
        {
            return *(*exeName);
        }

        char exe[1024];
        int32 ret;

        ret = readlink("/proc/self/exe",exe,sizeof(exe)-1);
        if(ret ==-1)
        {
            return nullptr;
        }
        exe[ret] = 0;
        int32 i = 0;
        int32 lastSlashIndex = -1;
        while (exe[i])
        {
            if (exe[i] == '/')
            {
                lastSlashIndex = i;
            }
            ++i;
        }

        String fullExePath(exe);
        exeName = lastSlashIndex != -1 ? RYUJIN_NEW String(*fullExePath + (lastSlashIndex+1)) : RYUJIN_NEW String(fullExePath);
        return *exeName;
    }

    const char* AppInfo::GetProfilingDir()
    {
        if (profilingDir)
        {
            return *(*profilingDir);
        }

        profilingDir = RYUJIN_NEW String(String::Printf("%sProfiler/", GetAppDataDir()));

        if (!Directory::DirectoryExists(*profilingDir))
        {
            Directory::CreateDirectory(*profilingDir);
        }

        return *profilingDir;
    }

    const char* AppInfo::GetAppDataDir()
    {
        if (appDataDir)
        {
            return *(*appDataDir);
        }

        const char *homedir;

        if ((homedir = getenv("HOME")) == nullptr)
        {
            homedir = getpwuid(getuid())->pw_dir;
        }

        if (!homedir)
            return nullptr;

        appDataDir = RYUJIN_NEW String(homedir);
        (*appDataDir) += "/Ryujin/";
        return *appDataDir;
    }
}
