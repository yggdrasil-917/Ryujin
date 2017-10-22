
#include "Events.hpp"


namespace Ryujin
{
    void PumpEvents()
    {
#if PLATFORM_WINDOWS
        PumpEvents_Windows();
#elif PLATFORM_MAC
        PumpEvents_Mac();
#elif PLATFORM_LINUX
        PumpEvents_Linux();
#endif
    }
    
    bool PollEvents(RyujinEvent& inEvent)
    {
        if (RyujinEventQueue::Instance()->events.Size() == 0)
            return false;
        
        return RyujinEventQueue::Instance()->events.GetFirst(inEvent);
    }
    
    void PushEvent(EngineEvent inEngineEvent, WindowEventID inWindowEvent, int32 inData1, int32 inData2)
    {
        RyujinEvent event;
        event.engineEvent = inEngineEvent;
        event.windowEventID = inWindowEvent;
        event.Data1 = inData1;
        event.Data2 = inData2;
        RyujinEventQueue::Instance()->events.Add(event);
    }
    
    void PushKeyboardEvent(EngineEvent inEngineEvent, int32 key, KeyMod mod)
    {
        RyujinEvent event;
        event.engineEvent = inEngineEvent;
        event.Data1 = key;
        event.Data2 = (int32)mod;
        RyujinEventQueue::Instance()->events.Add(event);
    }
    
    void PushWindowEvent(WindowEventID inWindowEvent, int32 inData1, int32 inData2)
    {
        RyujinEvent event;
        event.engineEvent = EngineEvent::WindowEvent;
        event.windowEventID = inWindowEvent;
        event.Data1 = inData1;
        event.Data2 = inData2;
        RyujinEventQueue::Instance()->events.Add(event);
    }
    
    void PopEvent()
    {
        RyujinEventQueue::Instance()->events.RemoveFront();
    }
}
