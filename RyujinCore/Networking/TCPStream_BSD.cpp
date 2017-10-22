#include "TCPStream_BSD.hpp"


#if WITH_BSD_SOCKETS

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in

namespace Ryujin
{
    TCPStream_BSD::TCPStream_BSD()
    :   TCPStreamBase()
    {
        
    }
    
    TCPStream_BSD::TCPStream_BSD(int32 sd, struct sockaddr_in* address)
    {
        inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), netInfo.peerIp, sizeof(netInfo.peerIp)-1);
        netInfo.socketDescriptor = sd;
        netInfo.peerPort = ntohs(address->sin_port);
    }
    
    TCPStream_BSD::TCPStream_BSD(const TCPStream_BSD& other)
    :   TCPStreamBase(other.netInfo.socketDescriptor, other.netInfo.peerIp, other.netInfo.peerPort)
    {
        
    }
    
    TCPStream_BSD::~TCPStream_BSD()
    {
        
    }
    
    bool TCPStream_BSD::WaitForReadEvent(int32 timeout)
    {
        fd_set sdset;
        struct timeval tv;
        
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        FD_ZERO(&sdset);
        FD_SET(netInfo.socketDescriptor, &sdset);
        if (select(netInfo.socketDescriptor+1, &sdset, NULL, NULL, &tv) > 0)
        {
            return true;
        }
        
        return false;
    }
    
    int32 TCPStream_BSD::Send(uint8* buffer, uint32 bufferByteSize)
    {
        return ::send(netInfo.socketDescriptor, (void*)buffer, bufferByteSize, 0);
    }
    
    int32 TCPStream_BSD::Receive(uint8* buffer, uint32 bufferByteSize, int32 timeout)
    {
        if (timeout <= 0)
        {
            return ::recv(netInfo.socketDescriptor, buffer, bufferByteSize, 0);
        }
        
        if (WaitForReadEvent(timeout) == true)
        {
            return ::recv(netInfo.socketDescriptor, buffer, bufferByteSize, 0);
        }
        
        return -1;
    }
    
    void TCPStream_BSD::Close()
    {
        close(netInfo.socketDescriptor);
    }
}

#endif
