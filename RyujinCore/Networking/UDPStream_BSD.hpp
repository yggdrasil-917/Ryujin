#pragma once


#include "NetworkBase.hpp"

#if WITH_BSD_SOCKETS

#include <arpa/inet.h>

namespace Ryujin
{
    class UDPStream_BSD : public UDPStreamBase
    {
    private:
        sockaddr_in destAddress;
        
    public:
        UDPStream_BSD();
        UDPStream_BSD(int32 sd, struct sockaddr_in* address);
        UDPStream_BSD(const UDPStream_BSD& other);
        ~UDPStream_BSD();
        
        
        int32 Send(uint8* buffer, uint32 bufferByteSize) OVERRIDE FINAL;
        int32 Receive(uint8* buffer, uint32 bufferByteSize, int32 timeout) OVERRIDE FINAL;
        void Close() OVERRIDE FINAL;
        
    };
}

#endif
