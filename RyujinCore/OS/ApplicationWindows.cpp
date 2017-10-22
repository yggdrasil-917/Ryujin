#include "Application.hpp"

#include "FileSystem.hpp"

#include "Shlwapi.h"
#include <shlobj.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Shlwapi.lib")

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

		char path[RYUJIN_MAX_PATH] = { 0 };
		if (GetModuleFileName(nullptr, path, RYUJIN_MAX_PATH))
		{
			PathRemoveFileSpec(path);
			appDir = RYUJIN_NEW String(path);
			return *(*appDir);
		}

		return "";
	}

	const char* AppInfo::GetExeDir()
	{
		if (exeDir)
		{
			return *(*exeDir);
		}

		char path[RYUJIN_MAX_PATH] = { 0 };
		if (GetModuleFileName(nullptr, path, RYUJIN_MAX_PATH))
		{
			PathRemoveFileSpec(path);
			exeDir = RYUJIN_NEW String(path);
			return *(*exeDir);
		}

		return "";
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

		cookedDir = RYUJIN_NEW String(String::Printf("%sCookedWindows/", GetResourcesDir()));

		return *cookedDir;
	}
    
    const char* AppInfo::GetDevCookedContentDir()
    {
        return "../../../Content/CookedPC/";
    }

	const char* AppInfo::GetExeName()
	{
		if (exeName)
		{
			return *(*exeName);
		}

		char path[RYUJIN_MAX_PATH] = { 0 };
		if (GetModuleFileName(nullptr, path, RYUJIN_MAX_PATH))
		{
			char* p = PathFindFileName(path);
			*(PathFindExtension(p)) = 0;
			exeName = RYUJIN_NEW String(p);
			return *(*exeName);
		}

		return "";
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

		char path[RYUJIN_MAX_PATH] = { 0 };
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);
		if (result == S_OK)
		{
			appDataDir = RYUJIN_NEW String(path);
			(*appDataDir) += "/Ryujin/";
			if (!Directory::DirectoryExists(*appDataDir))
			{
				Directory::CreateDirectory(*appDataDir);
			}
			return *appDataDir;
		}

		return "";
	}
}
