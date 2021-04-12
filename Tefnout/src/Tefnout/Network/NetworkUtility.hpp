#ifndef __NETWORKUTILITY__HPP
#define __NETWORKUTILITY__HPP

#include "NetworkCore.hpp"
#include "Tefnout/Network/Address.hpp"

namespace Tefnout
{
namespace Network
{

/**
 * @brief      Gets the last socket error code.
 *
 * @return     The last error code.
 */
TEFNOUT_API int GetLastErrorCode();

/**
 * @brief      Gets IPv4 address as an integer (bitmask).
 *
 * @param[in]  pSocketAdress  The socket adress
 *
 * @return     IPv4 address.
 */
TEFNOUT_API IpAddress GetIPv4Address(const sockaddr* pSocketAdress);

/**
 * @brief      Gets IPv4 address as an integer (bitmask).
 *
 * @param[in]  pIPv4Address  The socket adress
 *
 * @return     IPv4 address.
 */
TEFNOUT_API IpAddress GetIPv4Address(const sockaddr_in* pIPv4Address);

/**
 * @brief      Gets IPv6 address.
 *
 * @param[in]  pIPv6Address  The socket adress
 *
 * @return     IPv6 address.
 */
TEFNOUT_LOCAL const std::uint8_t* GetIPv6Address(const sockaddr_in6* pIPv6Address);

/**
 * @brief      Gets IPv6 address.
 *
 * @param[in]  pSocketAddress  The socket adress
 *
 * @return     IPv6 address.
 */
TEFNOUT_LOCAL std::uint8_t* GetIPv6Address(const sockaddr* pSocketAddress);

/**
 * @brief      Extract port from IPv4 or IPv6 @p pSocketAdress.
 *
 * @param[in]  pSocketAdress  The socket address (IPv4 or IPv6).
 *
 * @return     The port assigned to this address.
 */
TEFNOUT_API unsigned short GetPort(const sockaddr* pSocketAdress);

/**
 * @brief      Gets the hostname and service name as numeric values based on @p pSocketAddress.
 *
 * @param[in]  pSocketAddress  The socket address
 * @param      hostname      Used to store found hostname
 * @param      service       Used to store found service
 *
 * @return     Operation result
 */
TEFNOUT_API bool GetAddressAndService(const sockaddr* pSocketAddress, std::string& hostname,
                          std::string& service);

/**
 * @brief      Gets the hostname and service name as numeric values based on @p pSocketAddress.
 *
 * @param[in]  pSocketAddress  The socket IPv4 address
 * @param      hostname      Used to store found hostname
 * @param      service       Used to store found service
 *
 * @return     Operation result
 */
TEFNOUT_API bool GetAddressAndService(const sockaddr_in* pSocketAddress, std::string& hostname,
                          std::string& service);

} // namespace Network
} // namespace Tefnout

#endif
