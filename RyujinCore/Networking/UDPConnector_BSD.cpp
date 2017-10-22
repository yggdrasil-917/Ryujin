#include "UDPConnector_BSD.hpp"


#if WITH_BSD_SOCKETS

#include "UDPStream_BSD.hpp"
#include "../Allocators/Memory.hpp"


namespace Ryujin
{
    int32 UDPConnector_BSD::ResolveHostName(const char *host, struct ::in_addr *addr)
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
    
    NetworkStreamBase* UDPConnector_BSD::Connect(int32 port, const char* server)
    {
        struct sockaddr_in address;
        
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        if (ResolveHostName(server, &(address.sin_addr)) != 0)
        {
            inet_pton(PF_INET, server, &(address.sin_addr));
        }
        
        int32 sd = socket(AF_INET, SOCK_DGRAM, 0);
        return RYUJIN_NEW UDPStream_BSD(sd, &address);
    }
}

#endif
