#include "Tefnout/Core/Debug.hpp"
#include "Tefnout/Core/Logger.hpp"
#include "Tefnout/Network/Address.hpp"
#include "Tefnout/Network/NetworkCore.hpp"
#include "TefnoutPCH.hpp"

#include "NetworkUtility.hpp"
#include "Socket.hpp"
#include <netinet/in.h>

namespace Tefnout
{
namespace Network
{

Socket::Socket() : Socket(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
{
    TEFNOUT_ASSERT(m_handle != INVALID_SOCKET, "Failed to create {0}.", "socket");
}

Socket::Socket(SOCKET socketId) : m_handle(socketId), m_mode(SocketMode::Blocking)
{
    // Default to non blocking mode because
    // real time and waiting do not works well together
    if (m_handle != INVALID_SOCKET)
    {
        SetMode(SocketMode::NonBlocking);
    }
}

// Move semantics
Socket::Socket(Socket&& other) noexcept : m_handle(INVALID_SOCKET), m_mode(SocketMode::Blocking)
{
    swap(*this, other);
}

Socket& Socket ::operator=(Socket&& other) noexcept
{
    swap(*this, other);
    return *this;
}

void swap(Socket& lhs, Socket& rhs) noexcept
{
    using std::swap;

    swap(lhs.m_handle, rhs.m_handle);
    swap(lhs.m_mode, rhs.m_mode);
}

Socket::~Socket()
{
    Close();
}

bool Socket::Bind(const Address& address)
{
    // Avoid port lower than 1024 and higher than 50000
    sockaddr_in sockAdressV4;
    sockAdressV4.sin_family = AF_INET;
    sockAdressV4.sin_addr.s_addr = INADDR_ANY;
    sockAdressV4.sin_port = htons(address.GetPort());

    // Link socket to communication port
    if (bind(m_handle, (const sockaddr*)&sockAdressV4, sizeof(sockAdressV4)) != 0)
    {
        TEFNOUT_WARN("Socket binding failed with error {0}", GetLastErrorCode());
        Close();
        return false;
    }

    // Find informations
    std::string hostname;
    std::string service;
    if (GetAddressAndService(&sockAdressV4, hostname, service))
    {
        TEFNOUT_TRACE("Socket bind to port {0} and address {1}", service, hostname);
    }

    return true;
}

void Socket::Close()
{
    closesocket(m_handle);
    m_handle = INVALID_SOCKET;
}

bool Socket::SetMode(SocketMode newMode)
{
    int iResult = SOCKET_ERROR;

#if TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_WINDOWS
    // https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ioctlsocket
    DWORD modeHandle = (newMode == SocketMode::Blocking) ? 0 : 1;
    iResult = ioctlsocket(m_handle, FIONBIO, &modeHandle);
#elif TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_MAC || TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_UNIX
    // Get current flags in use
    const int flags = fcntl(m_handle, F_GETFL, 0);
    if (flags != SOCKET_ERROR)
    {
        // Set or clear blocking flag without affecting others
        iResult =
            fcntl(m_handle, F_SETFL,
                  (newMode == SocketMode::Blocking) ? flags ^ O_NONBLOCK : flags | O_NONBLOCK);
    }
#endif
    if (iResult == SOCKET_ERROR)
    {
        TEFNOUT_WARN("Socket mode cannot be updated ({0}) due to error {1}", newMode,
                     GetLastErrorCode);
        return false;
    }
    else
    {
        m_mode = newMode;
        return true;
    }
}

bool Socket::SendTo(const Address& address, const void* packetData, const unsigned int packetSize,
                    int flags)
{
    sockaddr_in addrTo;
    memset(&addrTo, 0, sizeof(addrTo));
    addrTo.sin_family = AF_INET;
    addrTo.sin_addr.s_addr = htonl(address.GetIp().AsInt());
    addrTo.sin_port = htons(address.GetPort());

    int bytesSent = sendto(m_handle, (char*)packetData, packetSize, flags, (sockaddr*)&addrTo,
                           sizeof(sockaddr_in));

    if (bytesSent != static_cast<int>(packetSize))
    {
        TEFNOUT_WARN("Failed to send data (Handle={0}) due to error {1}", m_handle,
                     GetLastErrorCode);
        return false;
    }
    else
    {
        return true;
    }
}

bool Socket::ReceiveFrom(Address& address, void* packet_data, const unsigned int packet_maxSize,
                         int flags)
{
    // Init IPv4 address
    sockaddr_in addrFrom;
    socklen_t fromLength = sizeof(addrFrom);

    // Try to get data from any sender
    int bytesReceived = recvfrom(m_handle, (char*)packet_data, packet_maxSize, flags,
                                 (sockaddr*)&addrFrom, &fromLength);
    if (bytesReceived == SOCKET_ERROR)
    {
        return false;
    }

    // Set sender address
    const IpAddress senderIp = IpAddress(GetIPv4Address(&addrFrom));
    address = Address(senderIp, GetPort((sockaddr*)&addrFrom));

    return true;
}

} // namespace Network
} // namespace Tefnout
