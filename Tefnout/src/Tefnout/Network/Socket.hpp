#ifndef __SOCKET__HPP
#define __SOCKET__HPP

#include "NetworkCore.hpp"
#include "Tefnout/Network/Address.hpp"

namespace Tefnout
{
namespace Network
{

/**
 * @brief      Wrapper arround socket modes.
 */
enum class TEFNOUT_API SocketMode
{
    Blocking = 0,   // recvfrom will wait
    NonBlocking = 1 // recvfrom will not wait
};

/**
 * @brief      A small wrapper around a multi-platform UDP socket.
 *
 * @note       Cannot be copied only moved.
 */
class TEFNOUT_API Socket
{
  public:
    Socket();
    Socket(SOCKET socketId);
    ~Socket();

    // Copy is not allowed to avoid closing wrapped socket
    Socket(const Socket& otherSocket) = delete;
    Socket& operator=(Socket const& otherSocket) = delete;

    // Copy and swap idiom
    Socket& operator=(Socket&& other) noexcept;
    Socket(Socket&& other) noexcept;
    friend void swap(Socket& lhs, Socket& rhs) noexcept;

    inline SocketMode GetMode()
    {
        return m_mode;
    }
    inline SOCKET GetHandle()
    {
        return m_handle;
    }

    void Close();
    bool SetMode(SocketMode newMode);

    bool Bind(const Address& address);
    bool SendTo(const Address& address, const void* packetData, const unsigned int packetSize,
                int flags);
    bool ReceiveFrom(Address& address, void* packet_data, const unsigned int packet_maxSize,
                     int flags);

  private:
    SOCKET m_handle = INVALID_SOCKET;
    SocketMode m_mode;
};

} // namespace Network
} // namespace Tefnout

#endif
