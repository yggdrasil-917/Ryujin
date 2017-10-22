#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP


#include "../Containers/String.hpp"
#include "../Containers/DynamicArray.hpp"

#if PLATFORM_WINDOWS
//#include <windows.h>
//#include <tchar.h>
//#include <strsafe.h>
//#pragma comment(lib, "User32.lib")

#ifdef CreateDirectory
#undef CreateDirectory
#endif

#else

#include <errno.h>
#include <unistd.h>

#include <dirent.h>
#include <time.h>

#define DEFAULT_MODE      (S_IRWXU | S_IRGRP |  S_IXGRP | S_IROTH | S_IXOTH)

#endif

#include <sys/types.h>
#include <sys/stat.h>

#if !PLATFORM_WINDOWS
#include <copyfile.h>
#endif

namespace Ryujin
{
    using FileHandle = FILE*;
    
    enum FileMode
    {
        FM_Read = 0,
        FM_Write,
        FM_Append,
        FM_ReadUpdate,
        FM_WriteUpdate,
        FM_AppendUpdate,
        
        FM_Count
    };
    
    static const char* textModeStr[FileMode::FM_Count] =
    {
        "r",
        "w",
        "a",
        "r+",
        "w+",
        "a+"
    };
    
    static const char* binaryModeStr[FileMode::FM_Count] =
    {
        "rb",
        "wb",
        "ab",
        "rb+",
        "wb+",
        "ab+"
    };
    
    
    class File
    {
    public:
        static bool IsFileNewerThan(const char* fileA, const char* fileB)
        {
#if PLATFORM_WINDOWS
            WIN32_FIND_DATA ffdA, ffdB;
            size_t pathLengthA, pathLengthB;
            HANDLE hFindA = INVALID_HANDLE_VALUE;
            HANDLE hFindB = INVALID_HANDLE_VALUE;
            DWORD errorCode = 0;
            
            pathLengthA = strlen(fileA);
            pathLengthB = strlen(fileB);
            
            if (pathLengthA > RYUJIN_MAX_PATH - 3 || pathLengthB > RYUJIN_MAX_PATH - 3)
            {
                ErrorMsg("Path to directory is too long");
                return false;
            }
            
            // Find the first file in the directory.
            hFindA = FindFirstFile(fileA, &ffdA);
            hFindB = FindFirstFile(fileB, &ffdB);
            if (hFindA == INVALID_HANDLE_VALUE || hFindB == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            
            ULARGE_INTEGER ul1, ul2;
            ul1.LowPart = ffdA.ftLastWriteTime.dwLowDateTime;
            ul1.HighPart = ffdA.ftLastWriteTime.dwHighDateTime;
            ul2.LowPart = ffdB.ftLastWriteTime.dwLowDateTime;
            ul2.HighPart = ffdB.ftLastWriteTime.dwHighDateTime;
            
            return ul1.QuadPart > ul2.QuadPart;
            
#else
            struct stat fstA;
            struct stat fstB;
            bzero(&fstA,sizeof(fstA));
            bzero(&fstB,sizeof(fstB));
            
            if (stat(fileA, &fstA) != 0 || stat(fileB, &fstB) != 0)
            {
                return false;
            }
            
            return fstA.st_mtime > fstB.st_mtime;
#endif
        }
        
        static bool IsFileOlderThanNDays(const char* filepath, const int32 numDays)
        {
            struct stat fstA;
            Memory::MemSet(&fstA, 0, sizeof(fstA));
            
            if (stat(filepath, &fstA) != 0)
            {
                return false;
            }
            
            std::time_t numDaysInSeconds(numDays * 24 * 60 * 60);
            std::time_t currentTimeInSeconds = std::time(NULL);
            if (currentTimeInSeconds - fstA.st_mtime >= numDaysInSeconds)
            {
                return true;
            }
            
            return false;
        }
        
        static bool Exists(const char* file)
        {
#if PLATFORM_WINDOWS
            return false;
#else
            struct stat fileStat;
            bzero(&fileStat, sizeof(fileStat));
            
            return stat(file, &fileStat) == 0;
#endif
        }
        
        static void Delete(const char* filepath)
        {
            std::remove(filepath);
        }
        
        static FileHandle Open(const char* filepath, FileMode mode, bool bIsBinaryFile = false)
        {
            FileHandle handle;
#if PLATFORM_WINDOWS
            fopen_s(&handle, filepath, bIsBinaryFile ? binaryModeStr[mode] : textModeStr[mode]);
#else
            handle = fopen(filepath, bIsBinaryFile ? binaryModeStr[mode] : textModeStr[mode]);
#endif
            
            return handle;
        }
        
        static void Close(FileHandle handle)
        {
            fclose(handle);
        }
        
        static void Write(const void* data, PtrSize elementSize, PtrSize elementCount, FileHandle handle)
        {
            fwrite(data, elementSize, elementCount, handle);
        }
        
        static char* ReadLine(char* data, int32 num, FileHandle handle)
        {
            return fgets(data, num, handle);
        }
        
        static PtrSize Read(void* data, PtrSize elementSize, PtrSize elementCount, FileHandle handle)
        {
            return fread(data, elementSize, elementCount, handle);
        }
        
        static bool Copy(const char* fileNameFrom, const char* fileNameTo)
        {
#if PLATFORM_WINDOWS
			if (CopyFileA(fileNameFrom, fileNameTo, FALSE))
				return true;

			return false;
#else
            copyfile_state_t state = copyfile_state_alloc();
            int32 r = copyfile(fileNameFrom, fileNameTo, state, COPYFILE_ALL);
            int32 bWasCopied = false;
            if (copyfile_state_get(state, COPYFILE_STATE_COPIED, &bWasCopied) == 0 && bWasCopied)
            {
                copyfile_state_free(state);
                return true;
            }
            
            copyfile_state_free(state);
            return false;
#endif
        }
        
        static PtrSize FileSize(FileHandle handle)
        {
            struct stat fileStat;
            memset(&fileStat, 0, sizeof(struct stat));
            
#if PLATFORM_WINDOWS
            if (fstat(_fileno(handle), &fileStat) == -1)
#else
                if (fstat(fileno(handle), &fileStat) == -1)
#endif
                {
                    return 0;
                }
            
            return fileStat.st_size;
        }
        
        static int32 Seek(PtrSize offset, FileHandle handle)
        {
            return fseek(handle, (long)offset, SEEK_CUR);
        }
        
        static int32 SeekFromStart(PtrSize offset, FileHandle handle)
        {
            return fseek(handle, (long)offset, SEEK_SET);
        }
        
        static int32 SeekFromEnd(PtrSize offset, FileHandle handle)
        {
            return fseek(handle, (long)offset, SEEK_END);
        }
    };
    
    
	class Directory
	{
	public:
		static String GetCurrentDirectoryPath()
		{
			#if PLATFORM_WINDOWS
			char path[RYUJIN_MAX_PATH];
            GetCurrentDirectoryA(RYUJIN_MAX_PATH, path);
            return String(path);
			#else
            char buffer[RYUJIN_MAX_PATH];
            char* str = getcwd(buffer, sizeof(buffer));
            return str ? String(buffer) : String();
			#endif
		}

		static bool CreateDirectory(const char* path)
		{
        #if PLATFORM_WINDOWS
			return CreateDirectoryA(path, nullptr) == TRUE;
        #else
            char* it = strdup(path);
            char* p = it;
            
            // Do mkdir for each slash until end of string or error
            while (*p != '\0')
            {
                ++p;
                
                // Find first slash or end
                while(*p != '\0' && *p != '/')
                    ++p;
                
                // Remember value from p
                char v = *p;
                
                // Write end of string at p
                *p = '\0';
                
                // Create folder from path to '\0' inserted at p
                if(mkdir(it, DEFAULT_MODE) == -1 && errno != EEXIST)
                {
                    *p = v;
                    free(it);
                    return false;
                }
                
                // Restore path to it's former glory
                *p = v;
            }
            
            free(it);
            return true;
        #endif
		}
        
        static void DeleteDirectory(const char* dir)
        {
#if PLATFORM_WINDOWS
			WIN32_FIND_DATA ffd;
            HANDLE hFind = FindFirstFile(dir, &ffd);
            
            if (INVALID_HANDLE_VALUE == hFind)
            {
                return;
            }
            
            // List all the files in the directory with some info about them.
            do
            {
                DeleteFile(ffd.cFileName);
            } while (FindNextFile(hFind, &ffd) != 0);
            
            FindClose(hFind);
            RemoveDirectory(dir);
#else
            DIR*            dp;
            struct dirent*  ep;
            char            buffer[RYUJIN_MAX_PATH] = {0};
            
            dp = opendir(dir);
            
            while ((ep = readdir(dp)) != NULL)
            {
                if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
                    continue;
                
                sprintf(buffer, "%s/%s", dir, ep->d_name);
                struct stat info;
                if (stat(buffer, &info) != 0)
                {
                    continue; // access denied
                }
                
                if (info.st_mode & S_IFDIR)
                {
                    DeleteDirectory(buffer);
                }
                else
                {
                    unlink(buffer);
                }
            }
            
            closedir(dp);
            rmdir(dir);
#endif
        }

		static bool DirectoryExists(const char* path)
		{
			struct stat info;
			if (stat(path, &info) != 0)
			{
				return false; // access denied
			}

			if (info.st_mode & S_IFDIR)
			{
				return true; // is a directory
			}

			return false; // not a directory
		}

		static bool GetFiles(const char* pathToDirectory, DynamicArray<String>& outFiles, const char* fileFilter = "\\*")
		{
#if PLATFORM_WINDOWS
			WIN32_FIND_DATA ffd;
			char dir[RYUJIN_MAX_PATH];
			size_t pathLength;
			HANDLE hFind = INVALID_HANDLE_VALUE;
			DWORD errorCode = 0;

			pathLength = strlen(pathToDirectory);
			if (pathLength > RYUJIN_MAX_PATH - 3)
			{
				ErrorMsg("Path to directory is too long");
				return false;
			}

			strcpy(dir, pathToDirectory);
			strcat(dir, fileFilter);

			// Find the first file in the directory.
			hFind = FindFirstFile(dir, &ffd);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				return false;
			}

			do
			{
				if ( (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 ) // add filename to array if it is not the directory name
				{
					outFiles.Add(ffd.cFileName);
				}
			} while (FindNextFile(hFind, &ffd) != 0);

			errorCode = GetLastError();
			if (errorCode != ERROR_NO_MORE_FILES)
			{
				String errorMsg = "FindNextFile error: " + errorCode;
				ErrorMsg(*errorMsg);
				return false;
			}

			FindClose(hFind);

			return true;
#else
            DIR *dp;
            struct dirent *ep;
            dp = opendir(pathToDirectory);
            
            if (dp != NULL)
            {
                ep = readdir(dp);
                while (ep)
                {
                    if (ep->d_type != DT_DIR)
                    {
                        outFiles.Add(ep->d_name);
                    }
                    ep = readdir(dp);
                }
                
                closedir (dp);
            }
            else
            {
                ErrorMsg(*String::Printf("Couldn't open the directory %s %s", *GetCurrentDirectoryPath(), pathToDirectory));
                return false;
            }
            
            return true;
#endif
		}
        
        static bool CopyAllFiles(String sorc, String dest)
        {
#if PLATFORM_WINDOWS
			DynamicArray<String> files;
			if (GetFiles(sorc, files) == false)
				return false;

			for (const String& file : files)
			{
				String src = String::Printf("%s/%s", *sorc, *file);
				String dst = String::Printf("%s/%s", *dest, *file);
				File::Copy(src, dst);
			}

			return true;
#else
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir (*sorc)) != NULL)
            {
                while ((ent = readdir (dir)) != NULL)
                { /* print all the files and directories within directory */
                    if (ent->d_name != String("."))
                    {          //REMOVE PWD
                        if (ent->d_name != String(".."))
                        {     //REMOVE PARENT DIR
                            String src = sorc + "/" + ent->d_name;
                            String dst = dest + "/" + ent->d_name;
                            
                            File::Copy(*src, *dst);
                        }
                    }
                }
                
                closedir (dir);
            }
            else
            {
                return false;
            }
#endif
            
            return true;
        }
	};
}


#endif
