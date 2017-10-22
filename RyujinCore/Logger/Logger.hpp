
#pragma once

#include <cstdarg>

#include "LoggerPolicies.hpp"

namespace Ryujin
{
    
    class CORE_API Logger
    {
    protected:
        uint32 activeChannels;
        uint32 verbosity;
        
    public:
        Logger();
        VIRTUAL ~Logger(void) {}
        
        void EnableChannel(uint32 channel);
        void DisableChannel(uint32 channel);
        uint32 GetActiveChannels() const;
        
        void SetVerbosity(uint32 level);
        uint32 GetVerbosity() const;
        
        VIRTUAL void Log(Channel channel, uint32 type, const SourceInfo& SourceInfo, const char* format, ...) = 0;
    };
    
    template <class FilterPolicy, class FormatPolicy, class WritePolicy>
    class CORE_API LoggerImpl : public Logger
    {
    public:
        LoggerImpl()
        : Logger()
        {
            writer.Initialize();
        }
        
        ~LoggerImpl()
        {
            
        }
        
        void Log(Channel channel, uint32 type, const SourceInfo& sourceInfo, const char* format, ...) OVERRIDE FINAL
        {
            Criteria criteria = { channel, type, sourceInfo, format };
            if (filter.Filter(this, criteria))
            {
                char buffer[MAX_LOG_BUFFER];
                va_list args;
                va_start(args, format);
                formatter.Format(buffer, criteria, format, &args);
                va_end(args);
                writer.Write(buffer);
            }
        }
        
    private:
        FilterPolicy filter;
        FormatPolicy formatter;
        WritePolicy writer;
    };
    
    
    using ConsoleLogger = LoggerImpl<CompositeFilter<ChannelFilter, VerbosityFilter>, SimpleFormat, ConsoleWriter>;
    using FileLogger = LoggerImpl<NoFilter, ExtendedFormat, FileWriter>;
    
    extern CORE_API ConsoleLogger logger;
	extern CORE_API FileLogger fileLogger;



	// Had to do this macro bs to get clang compiling
#define CHANNEL_NAME(x) x
#define CHANNEL(c) Channel::CHANNEL_NAME(c)


#define Debugf(channel, format, ...) \
    { logger.Log(CHANNEL(channel), LogType::Info, SourceInfo(__FUNCTION__, __LINE__), format, ##__VA_ARGS__); \
      fileLogger.Log(CHANNEL(channel), LogType::Info, SourceInfo(__FUNCTION__, __LINE__), format, ##__VA_ARGS__); }

#define Warnf(channel, format, ...) \
    { logger.Log(CHANNEL(channel), LogType::Warning, SourceInfo(__FUNCTION__, __LINE__), format, ##__VA_ARGS__); \
      fileLogger.Log(CHANNEL(channel), LogType::Warning, SourceInfo(__FUNCTION__, __LINE__), format, ##__VA_ARGS__); }

#define Errorf(channel, format, ...) \
    { logger.Log(CHANNEL(channel), LogType::Error, SourceInfo(__FUNCTION__, __LINE__), format, ##__VA_ARGS__); \
      fileLogger.Log(CHANNEL(channel), LogType::Error, SourceInfo(__FUNCTION__, __LINE__), format, ##__VA_ARGS__); }
    
    
#define EnableConsoleLoggerChannels(x) logger.EnableChannel(x)
#define DisableConsoleLoggerChannels(x) logger.DisableChannel(x)
}










