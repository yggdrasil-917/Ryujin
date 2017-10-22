#include "UDPStream_BSD.hpp"


#if WITH_BSD_SOCKETS

#include <sys/types.h>       // For data types
#include <sys/socket.h>      // For socket(), connect(), send(), and recv()
#include <netdb.h>           // For gethostbyname()
#include <arpa/inet.h>       // For inet_addr()
#include <unistd.h>          // For close()
#include <netinet/in.h>      // For sockaddr_in

namespace Ryujin
{
    UDPStream_BSD::UDPStream_BSD()
    :   UDPStreamBase()
    {
        
    }
    
    UDPStream_BSD::UDPStream_BSD(int32 sd, struct sockaddr_in* address)
    {
        inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), netInfo.peerIp, sizeof(netInfo.peerIp)-1);
        netInfo.socketDescriptor = sd;
        netInfo.peerPort = ntohs(address->sin_port);
        
        // BIG assumption made here with the ports being the same
        destAddress.sin_family = PF_INET;
        destAddress.sin_port = address->sin_port;
        destAddress.sin_addr.s_addr = address->sin_addr.s_addr;
    }
    
    UDPStream_BSD::UDPStream_BSD(const UDPStream_BSD& other)
    :   UDPStreamBase(other.netInfo.socketDescriptor, other.netInfo.peerIp, other.netInfo.peerPort)
    {
        
    }
    
    UDPStream_BSD::~UDPStream_BSD()
    {
        
    }
    
    int32 UDPStream_BSD::Send(uint8* buffer, uint32 bufferByteSize)
    {
        return sendto(netInfo.socketDescriptor, buffer, bufferByteSize, 0, (struct sockaddr*)&destAddress, sizeof(sockaddr_in));
    }
    
    int32 UDPStream_BSD::Receive(uint8* buffer, uint32 bufferByteSize, int32 timeout)
    {
        socklen_t fromLength = sizeof(sockaddr_in);
        return recvfrom(netInfo.socketDescriptor, buffer, bufferByteSize, 0, (struct sockaddr*)&destAddress, &fromLength);
    }
    
    void UDPStream_BSD::Close()
    {
        if (netInfo.socketDescriptor > 0)
        {
            close(netInfo.socketDescriptor);
        }
    }
}


#endif
