#ifndef __NETWORKCORE__HPP
#define __NETWORKCORE__HPP

#include "Tefnout/Core/Core.hpp"

// clang-format off
#if TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_WINDOWS
    #include "Ws2tcpip.h"
    #include <winsock2.h>

    // Match Linux only types
    typedef int socklen_t;

#elif TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_MAC || TEFNOUT_PLATFORM == TEFNOUT_PLATFORM_UNIX
    #include <arpa/inet.h>
    #include <fcntl.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>

    // Match Windows only types
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
#else
    #error "Unknown TEFNOUT_PLATFORM"
#endif
// clang-format on

namespace Tefnout
{
namespace Network
{

/**
 * @brief      A RAII networking session. Init sockets on creation and clear them on
 *             destruction.
 *
 * @note       Cannot be copied only moved.
 */
class TEFNOUT_LOCAL NetworkSession
{
  public:
    NetworkSession();
    ~NetworkSession();

    NetworkSession(const NetworkSession& otherSocket) = delete;
    NetworkSession& operator=(NetworkSession const& otherSocket) = delete;

  private:
    /**
     * @brief      Initialize the socket layer.
     *
     * @return     Operation result.
     */
    bool InitSockets();

    /**
     * @brief      Shutdown the socket layer.
     */
    void ShutdownSockets();
};

} // namespace Network
} // namespace Tefnout

#endif
