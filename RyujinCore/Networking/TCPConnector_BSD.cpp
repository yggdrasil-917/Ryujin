
#include "TCPConnector_BSD.hpp"
#include "TCPStream_BSD.hpp"

#include "../Allocators/Memory.hpp"

#if WITH_BSD_SOCKETS

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Ryujin
{
    int32 TCPConnector_BSD::ResolveHostName(const char *host, struct ::in_addr *addr)
    {
        struct addrinfo *res;
        
        int32 result = getaddrinfo(host, nullptr, nullptr, &res);
        if (result == 0)
        {
            memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct ::in_addr));
            freeaddrinfo(res);
        }
        
        return result;
    }
    
    NetworkStreamBase* TCPConnector_BSD::Connect(int32 port, const char* server)
    {
        struct sockaddr_in address;
        
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (ResolveHostName(server, &(address.sin_addr)) != 0)
        {
            inet_pton(PF_INET, server, &(address.sin_addr));
        }
        
        int32 sd = socket(AF_INET, SOCK_STREAM, 0);
        if (::connect(sd, (struct sockaddr*)&address, sizeof(address)) != 0)
        {
            Debugf(Network, "Failed to connect socket: %d", errno);
            return nullptr;
        }
        
        return RYUJIN_NEW TCPStream_BSD(sd, &address);
    }
}

#endif
