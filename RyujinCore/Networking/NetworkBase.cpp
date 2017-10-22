#include "NetworkBase.hpp"

#include "../Allocators/Memory.hpp"

namespace Ryujin
{
    NetInfo::NetInfo()
    :   socketDescriptor(0),
        peerPort(0)
    {
        Memory::MemSet(peerIp, 0, sizeof(peerIp));
    }
    
    NetInfo::NetInfo(int32 sd, const char* ip, int32 port)
    :   socketDescriptor(sd),
        peerPort(port)
    {
        if (ip)
        {
            if (strlen(ip) == 0)
            {
                Debugf(Network, "Empty ip string. Falling back to localhost");
                const char* localHost = "localhost";
                strcpy(peerIp, localHost);
            }
            else
            {
                strcpy(peerIp, ip);
            }
        }
        else
        {
            Debugf(Network, "Null ip address pointer. Falling back to localhost");
            const char* localHost = "localhost";
            strcpy(peerIp, localHost);
        }
    }
    
    NetInfo::NetInfo(const NetInfo& other)
    {
        socketDescriptor = other.socketDescriptor;
        peerPort = other.peerPort;
        Memory::MemCpy(peerIp, (void*)&other.peerIp[0], sizeof(peerIp));
    }
    
    NetInfo& NetInfo::operator=(const Ryujin::NetInfo &other)
    {
        socketDescriptor = other.socketDescriptor;
        peerPort = other.peerPort;
        Memory::MemCpy(peerIp, (void*)&other.peerIp[0], sizeof(peerIp));
        return *this;
    }
    
    UDPStreamBase::UDPStreamBase()
    {
        
    }
    
    UDPStreamBase::UDPStreamBase(int32 sd, const char* ip, int32 port)
    {
        netInfo = NetInfo(sd, ip, port);
    }
    
    UDPStreamBase::UDPStreamBase(const UDPStreamBase& other)
    {
        netInfo = other.netInfo;
    }
    
    UDPStreamBase::~UDPStreamBase()
    {
        
    }
    
    TCPStreamBase::TCPStreamBase()
    {
        
    }
    
    TCPStreamBase::TCPStreamBase(int32 sd, const char* ip, int32 port)
    {
        netInfo = NetInfo(sd, ip, port);
    }
    
    TCPStreamBase::TCPStreamBase(const TCPStreamBase& other)
    {
        netInfo = other.netInfo;
    }
    
    TCPStreamBase::~TCPStreamBase()
    {
        
    }
    
    NetworkAcceptorBase::NetworkAcceptorBase(int32 inPort, const char* inAddress)
    :   port(inPort),
        socketDescriptor(0),
        listening(false)
    {
        strcpy(addressString, inAddress);
    }
    
    NetworkAcceptorBase::~NetworkAcceptorBase()
    {
        
    }
}
