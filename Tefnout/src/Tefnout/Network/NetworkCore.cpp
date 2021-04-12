#include "TefnoutPCH.hpp"

#include "NetworkCore.hpp"

namespace Tefnout
{
namespace Network
{

NetworkSession::NetworkSession()
{
    InitSockets();
}

NetworkSession::~NetworkSession()
{
    ShutdownSockets();
}

bool NetworkSession::InitSockets()
{
#if TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_WINDOWS
    WSAData data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        WSACleanup();
        return false;
    }
    TEFNOUT_DEBUG("Windows socket layer correctly initialized");
    return true;
#else
    TEFNOUT_DEBUG("Unix socket layer correctly initialized");
    return true;
#endif
}

void NetworkSession::ShutdownSockets()
{
#if TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_WINDOWS
    WSACleanup();
#endif
}
} // namespace Network
} // namespace Tefnout
