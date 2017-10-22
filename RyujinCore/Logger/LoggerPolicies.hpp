
#pragma once


#include "../OS/Types.hpp"



namespace Ryujin
{
#define MAX_LOG_BUFFER 1028
    
    struct SourceInfo
    {
        const char* filename;
        int32 lineNumber;
        
        SourceInfo(const char* inFilename, int32 inLineNumber)
        : filename(inFilename),
          lineNumber(inLineNumber)
        {
            
        }
    };
    
    SMART_ENUM(Channel, uint32,
               Info = 1 << 0,
               Memory = 1 << 1,
               Renderer = 1 << 2,
               JobSystem = 1 << 3,
               Profiler = 1 << 4,
               IO = 1 << 5,
               Input = 1 << 6,
               Network = 1 << 7,
               Audio = 1 << 8,
               
               All = 0xffffffff);
    
    enum LogType
    {
        Info = 0,
        Warning,
        Error
    };
    
    struct Criteria
    {
        Channel channel;
        uint32 type;
        const SourceInfo& sourceInfo;
        const char* format;
    };
    
    
    
    struct CORE_API NoFilter
    {
        bool Filter(class Logger* logger, const Criteria& criteria);
    };
    
    struct CORE_API ChannelFilter
    {
        bool Filter(class Logger* logger, const Criteria& criteria);
    };
    
    struct CORE_API VerbosityFilter
    {
        bool Filter(class Logger* logger, const Criteria& criteria);
    };
    
    template<class Policy1, class Policy2>
    struct CORE_API CompositeFilter
    {
        bool Filter(class Logger* logger, const Criteria& criteria)
        {
            return policy1.Filter(logger, criteria) && policy2.Filter(logger, criteria);
        }
        
    private:
        Policy1 policy1;
        Policy2 policy2;
    };
    
    
    
    struct CORE_API SimpleFormat
    {
        void Format(char* buffer, const Criteria& criteria, const char* format, va_list* args);
    };
    
    struct CORE_API ExtendedFormat
    {
        void Format(char* buffer, const Criteria& criteria, const char* format, va_list* args);
    };
    
    
    
    struct CORE_API ConsoleWriter
    {
        bool ShouldOutputToFile() const;
        void Initialize();
        void Write(char* buffer);
    };
    
    struct CORE_API FileWriter
    {
        FILE* file;
        
        ~FileWriter();
        bool ShouldOutputToFile() const;
        void Initialize();
        void Write(char* buffer);
    };
}





