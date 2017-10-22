#pragma once


#include "NetworkBase.hpp"


#if WITH_BSD_SOCKETS

#include <arpa/inet.h>


namespace Ryujin
{
    class TCPStream_BSD : public TCPStreamBase
    {
    protected:
        bool WaitForReadEvent(int32 timeout) OVERRIDE FINAL;
        
    public:
        TCPStream_BSD();
        TCPStream_BSD(int32 sd, struct sockaddr_in* address);
        TCPStream_BSD(const TCPStream_BSD& other);
        ~TCPStream_BSD();
        
        
        int32 Send(uint8* buffer, uint32 bufferByteSize) OVERRIDE FINAL;
        int32 Receive(uint8* buffer, uint32 bufferByteSize, int32 timeout) OVERRIDE FINAL;
        void Close() OVERRIDE FINAL;
    };
}

#endif
