#pragma once


#include "NetworkBase.hpp"



namespace Ryujin
{
    class UDPAcceptor_BSD : public NetworkAcceptorBase
    {
    public:
        UDPAcceptor_BSD() = delete;
        UDPAcceptor_BSD(int32 inPort, const char* inAddress = "");
        ~UDPAcceptor_BSD();
        
        int32 Start() OVERRIDE FINAL;
        NetworkStreamBase* Accept() OVERRIDE FINAL;
        void Close() OVERRIDE FINAL;
    };
}
