
#include "Application.hpp"

#import <Foundation/Foundation.h>
#include <mach-o/dyld.h>

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
        
        appDir = new String([[[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/"] UTF8String]);
        return *(*appDir);
    }
    
    const char* AppInfo::GetExeDir()
    {
        if (exeDir)
        {
            return *(*exeDir);
        }
        
        exeDir = new String([[[[[NSBundle mainBundle] executablePath] stringByDeletingLastPathComponent] stringByAppendingString:@"/"] UTF8String]);
        return *(*exeDir);
    }
    
    const char* AppInfo::GetResourcesDir()
    {
        if (resourceDir)
        {
            return *(*resourceDir);
        }
        
        resourceDir = new String([[[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"] UTF8String]);
        return *(*resourceDir);
    }
    
    const char* AppInfo::GetCookedContentDir()
    {
        if (cookedDir)
        {
            return *(*cookedDir);
        }
        
        cookedDir = new String([[[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/CookedMac/"] UTF8String]);
        return *(*cookedDir);
    }
    
    const char* AppInfo::GetDevCookedContentDir()
    {
        return "../../../Content/CookedMac/";
    }
    
    const char* AppInfo::GetExeName()
    {
        if (exeName)
        {
            return *(*exeName);
        }
        
        const char* bundleName = [[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleName"] UTF8String];
        if (bundleName)
        {
            exeName = new String(bundleName);
        }
        else
        {
            char path[1024];
            uint32 size = sizeof(path);
            _NSGetExecutablePath(path, &size);
            String temp(path);
            DynamicArray<String> words;
            temp.ParseIntoArray(words, "/");
            exeName = new String(*words[words.GetSize()-1]);
        }
        return *(*exeName);
    }
    
    const char* AppInfo::GetProfilingDir()
    {
        if (profilingDir)
        {
            return *(*profilingDir);
        }
        
        profilingDir = new String(GetAppDataDir());
        (*profilingDir) += "Profiling/";
        return *(*profilingDir);
    }
    
    const char* AppInfo::GetAppDataDir()
    {
        if (appDataDir)
        {
            return *(*appDataDir);
        }
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
        NSString *applicationSupportDirectory = [paths firstObject];
        
        appDataDir = new String([[applicationSupportDirectory stringByAppendingString:@"/Ryujin/"] UTF8String]);
        return *(*appDataDir);
    }
}
