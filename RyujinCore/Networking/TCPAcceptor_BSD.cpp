
#include "TCPAcceptor_BSD.hpp"
#include "TCPStream_BSD.hpp"

#include "../Allocators/Memory.hpp"

#if WITH_BSD_SOCKETS

#include <unistd.h>          // For close()

namespace Ryujin
{
    TCPAcceptor_BSD::TCPAcceptor_BSD(int32 inPort, const char* inAddress)
    :   NetworkAcceptorBase(inPort, inAddress)
    {
        
    }
    
    TCPAcceptor_BSD::~TCPAcceptor_BSD()
    {
        if (socketDescriptor)
        {
            Close();
        }
    }
    
    int32 TCPAcceptor_BSD::Start()
    {
        if (listening)
        {
            return 0;
        }
        
        socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
        
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = PF_INET;
        addr.sin_port = htons(port);
        if (addressString[0] != '\0')
        {
            inet_pton(PF_INET, addressString, &(addr.sin_addr));
        }
        else
        {
            addr.sin_addr.s_addr = INADDR_ANY;
        }
        
        int32 optval = 1;
        setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
        
        int32 result = bind(socketDescriptor, (struct sockaddr*)&addr, sizeof(addr));
        if (result != 0)
        {
            Debugf(Network, "bind() failed: %d", errno);
            return result;
        }
        result = listen(socketDescriptor, 5);
        if (result != 0)
        {
            Debugf(Network, "listen() failed: %d", errno);
            return result;
        }
        
        listening = true;
        return result;
    }
    
    NetworkStreamBase* TCPAcceptor_BSD::Accept()
    {
        if (listening == false)
        {
            return nullptr;
        }
        
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        memset(&addr, 0, sizeof(addr));
        int32 sd = ::accept(socketDescriptor, (struct sockaddr*)&addr, &len);
        if (sd < 0)
        {
            Debugf(Network, "accept() failed");
            return nullptr;
        }
        
        return RYUJIN_NEW TCPStream_BSD(sd, &addr);
    }
    
    void TCPAcceptor_BSD::Close()
    {
        if (socketDescriptor > 0)
        {
            close(socketDescriptor);
        }
    }
}

#endif
