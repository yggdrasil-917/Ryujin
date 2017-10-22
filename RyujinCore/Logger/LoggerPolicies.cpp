

#include "LoggerPolicies.hpp"
#include "Logger.hpp"
#include "../Containers/String.hpp"
#include "../CoreUtils/Timer.hpp"

#include "../OS/Application.hpp"
#include "../OS/FileSystem.hpp"

namespace Ryujin
{
    uint64 logTimestamp = Time::TimeSinceEpoch();
    
    bool NoFilter::Filter(Logger *logger, const Criteria &criteria)
    {
        return true;
    }
    
    bool ChannelFilter::Filter(Logger* logger, const Criteria& criteria)
    {
        return (logger->GetActiveChannels() & criteria.channel) != 0;
    }
    
    bool VerbosityFilter::Filter(Logger* logger, const Criteria& criteria)
    {
        return logger->GetVerbosity() <= criteria.type;
    }
    
    
    void SimpleFormat::Format(char* buffer, const Criteria& criteria, const char* format, va_list* args)
    {
        uint64 currentTimestamp = Time::TimeSinceEpoch();
        
        sprintf(buffer, "[%.2f] %s: ", (currentTimestamp - logTimestamp) / 1000.0f, criteria.channel.ToString());
        int32 offset = (int32)strlen(buffer);
        
        vsnprintf(buffer + offset, MAX_LOG_BUFFER, format, *args);
    }
    
    void ExtendedFormat::Format(char* buffer, const Criteria& criteria, const char* format, va_list* args)
    {
        uint64 currentTimestamp = Time::TimeSinceEpoch();
        
        sprintf(buffer, "[%.2f] %s(%d) %s: ", (currentTimestamp - logTimestamp) / 1000.0f,
                criteria.sourceInfo.filename, criteria.sourceInfo.lineNumber,
                criteria.channel.ToString());
        
        int32 offset = (int32)strlen(buffer);
        
        vsnprintf(buffer + offset, MAX_LOG_BUFFER, format, *args);
    }
    
    
    
    void ConsoleWriter::Write(char *buffer)
    {
        printf("%s\n", buffer);
#if PLATFORM_WINDOWS
		OutputDebugString(buffer);
		OutputDebugString("\n");
#endif
    }
    
    void ConsoleWriter::Initialize()
    {
        
    }
    
    bool ConsoleWriter::ShouldOutputToFile() const
    {
        return false;
    }
    
    FileWriter::~FileWriter()
    {
        if (file)
        {
            fclose(file);
            file = nullptr;
        }
    }
    
    void FileWriter::Write(char *buffer)
    {
        if (file)
        {
            fprintf(file, "%s\n", buffer);
        }
    }
    
    void FileWriter::Initialize()
    {
		String fileLogDir = String::Printf("%sLogs", AppInfo::GetAppDataDir());
		if (!Directory::DirectoryExists(*fileLogDir))
		{
			Directory::CreateDirectory(*fileLogDir);
		}

		fileLogDir += String::Printf("/%s_%s.log", AppInfo::GetExeName(), *String::CurrentDate());// "/Ryujin.log";
        file = fopen(fileLogDir, "w");
    }
    
    bool FileWriter::ShouldOutputToFile() const
    {
        return true;
    }
}