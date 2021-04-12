#include "Tefnout/Network/Address.hpp"
#include "TefnoutPCH.hpp"

#include "NetworkUtility.hpp"

#include "Socket.hpp"
#include <string>
#include <sys/socket.h>

namespace Tefnout
{
namespace Network
{

int GetLastErrorCode()
{
#if TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_WINDOWS
    return WSAGetLastError();
#elif TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_MAC || TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_UNIX
    return errno;
#endif
}

IpAddress GetIPv4Address(const sockaddr* pSocketAdress)
{
    TEFNOUT_ASSERT(pSocketAdress->sa_family == AF_INET, "Should be an {0} address", "IPv4");
    return IpAddress(ntohl(((sockaddr_in*)pSocketAdress)->sin_addr.s_addr));
}

IpAddress GetIPv4Address(const sockaddr_in* pIPv4Address)
{
    return IpAddress(ntohl(pIPv4Address->sin_addr.s_addr));
}

const std::uint8_t* GetIPv6Address(const sockaddr_in6* pIPv6Address)
{
    return pIPv6Address->sin6_addr.s6_addr;
}

std::uint8_t* GetIPv6Address(const sockaddr* pSocketAddress)
{
    TEFNOUT_ASSERT(pSocketAddress->sa_family == AF_INET6, "Should be an {0} address", "IPv6");
    return ((sockaddr_in6*)pSocketAddress)->sin6_addr.s6_addr;
}

unsigned short GetPort(const sockaddr* pSocketAdress)
{
    if (pSocketAdress->sa_family == AF_INET)
    {
        // IPv4
        return ntohs(((sockaddr_in*)pSocketAdress)->sin_port);
    }
    else if (pSocketAdress->sa_family == AF_INET6)
    {
        // IPv6
        return ntohs(((sockaddr_in6*)pSocketAdress)->sin6_port);
    }

    TEFNOUT_ASSERT(false, "Should be an {0} address", "IPv4 or IPv6");
    return 0u;
}

bool GetAddressAndService(const sockaddr* pSocketAddress, std::string& hostname,
                         std::string& service)
{
    char hostBuffer[NI_MAXHOST];
    char serviceBuffer[NI_MAXSERV];

    int dwRetval = getnameinfo(pSocketAddress, sizeof(struct sockaddr),
                               hostBuffer, sizeof(hostBuffer), serviceBuffer, sizeof(serviceBuffer),
                               NI_NUMERICHOST | NI_NUMERICSERV);

    if (dwRetval != 0)
    {
        TEFNOUT_WARN("getnameinfo failed with error {0}", GetLastErrorCode());
    }
    else
    {
        hostname = std::string(hostBuffer);
        service = std::string(serviceBuffer);
    }

    return (dwRetval == 0) ? true : false;
}

bool GetAddressAndService(const sockaddr_in* pSocketAddress, std::string& hostname, std::string& service)
{
    return GetAddressAndService((const sockaddr*)pSocketAddress, hostname, service);
}

} // namespace Network
} // namespace Tefnout
