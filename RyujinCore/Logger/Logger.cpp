
#include "Logger.hpp"



namespace Ryujin
{
    ConsoleLogger logger;
    FileLogger fileLogger;
    
    Logger::Logger()
    : activeChannels(Channel::All),
      verbosity(0)
    {
        
    }
    
    void Logger::EnableChannel(uint32 channel)
    {
        activeChannels |= channel;
    }
    
    void Logger::DisableChannel(uint32 channel)
    {
        activeChannels &= ~channel;
    }
    
    uint32 Logger::GetActiveChannels() const
    {
        return activeChannels;
    }
    
    void Logger::SetVerbosity(uint32 level)
    {
        verbosity = level;
    }
    
    uint32 Logger::GetVerbosity() const
    {
        return verbosity;
    }
}