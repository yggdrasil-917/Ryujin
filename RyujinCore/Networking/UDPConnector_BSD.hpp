#pragma once


#include "NetworkBase.hpp"


namespace Ryujin
{
    class UDPConnector_BSD : public NetworkConnectorBase
    {
    protected:
        int32 ResolveHostName(const char* host, struct ::in_addr* addr) OVERRIDE FINAL;
        
    public:
        NetworkStreamBase* Connect(int32 port, const char* server) OVERRIDE FINAL;
    };
}
