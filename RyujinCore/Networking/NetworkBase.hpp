#pragma once



#include "../Logger/Logger.hpp"


#if PLATFORM_MAC || PLATFORM_LINUX

#ifndef WITH_BSD_SOCKETS
#define WITH_BSD_SOCKETS 1
#endif
#define WITH_WINDOWS_SOCKETS 0

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#elif PLATFORM_WINDOWS

#define WITH_BSD_SOCKETS 0
#define WITH_WINDOWS_SOCKETS 1

#endif



namespace Ryujin
{
    struct NetInfo
    {
        int32 socketDescriptor;
        char peerIp[32];
        int32 peerPort;
        
        NetInfo();
        NetInfo(int32 sd, const char* ip, int32 port);
        NetInfo(const NetInfo& other);
        
        NetInfo& operator=(const NetInfo& other);
    };
    
    class NetworkStreamBase
    {
    protected:
        NetInfo netInfo;
        
    public:
        NetworkStreamBase() {}
        VIRTUAL ~NetworkStreamBase() {}
        
        VIRTUAL int32 Send(uint8* buffer, uint32 bufferByteSize) = 0;
        VIRTUAL int32 Receive(uint8* buffer, uint32 bufferByteSize, int32 timeout) = 0;
        VIRTUAL void Close() = 0;
        
        
        FORCEINLINE const char* GetPeerIp() const { return netInfo.peerIp; }
        FORCEINLINE int32 GetPeerPort() const { return netInfo.peerPort; }
    };
    
    class UDPStreamBase : public NetworkStreamBase
    {
    public:
        UDPStreamBase();
        UDPStreamBase(int32 sd, const char* ip, int32 port);
        UDPStreamBase(const UDPStreamBase& other);
        VIRTUAL ~UDPStreamBase();
        
    };
    
    class TCPStreamBase : public NetworkStreamBase
    {
    protected:
        VIRTUAL bool WaitForReadEvent(int32 timeout) = 0;
        
    public:
        TCPStreamBase();
        TCPStreamBase(int32 sd, const char* ip, int32 port);
        TCPStreamBase(const TCPStreamBase& other);
        VIRTUAL ~TCPStreamBase();
    };
    
    
    class NetworkConnectorBase
    {
    protected:
        VIRTUAL int32 ResolveHostName(const char* host, struct ::in_addr* addr) = 0;
        
    public:
        VIRTUAL NetworkStreamBase* Connect(int32 port, const char* server) = 0;
    };
    
    
    class NetworkAcceptorBase
    {
    protected:
        char addressString[32];
        int32 port;
        int32 socketDescriptor;
        bool listening;
        
    public:
        NetworkAcceptorBase() = delete;
        NetworkAcceptorBase(int32 inPort, const char* inAddress = "");
        VIRTUAL ~NetworkAcceptorBase();
        
        VIRTUAL int32 Start() = 0;
        VIRTUAL NetworkStreamBase* Accept() = 0;
        VIRTUAL void Close() = 0;
    };
}
