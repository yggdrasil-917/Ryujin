#pragma once

#include "NetworkBase.hpp"



namespace Ryujin
{
    class TCPAcceptor_BSD : public NetworkAcceptorBase
    {
    public:
        TCPAcceptor_BSD() = delete;
        TCPAcceptor_BSD(int32 inPort, const char* inAddress = "");
        ~TCPAcceptor_BSD();
        
        int32 Start() OVERRIDE FINAL;
        NetworkStreamBase* Accept() OVERRIDE FINAL;
        void Close() OVERRIDE FINAL;
    };
}
