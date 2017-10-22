#pragma once

#include "NetworkBase.hpp"

#if WITH_BSD_SOCKETS

#include "TCPStream_BSD.hpp"
#include "TCPConnector_BSD.hpp"
#include "TCPAcceptor_BSD.hpp"

#include "UDPStream_BSD.hpp"

namespace Ryujin
{
    using TCPStream = TCPStream_BSD;
    using TCPAcceptor = TCPAcceptor_BSD;
    using TCPConnector = TCPConnector_BSD;
}

#endif
