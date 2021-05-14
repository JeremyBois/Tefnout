#ifndef __ADDRESS__HPP
#define __ADDRESS__HPP

#include "Tefnout/Core/Platform.hpp"
#include <bits/stdint-uintn.h>
#include <cstdint>

namespace Tefnout
{
namespace Network
{

// @TODO Split Ip in two classes v4 and V6

/**
 * @brief      Supported IpTypes for addresses.
 */
enum class IpType
{
    Invalid,
    v4,
    v6
};

/**
 * @brief      Abstraction over an IP address (v4 or v6).
 *
 */
struct TEFNOUT_API IpAddress
{
  public:
    // v4
    IpAddress();
    explicit IpAddress(std::uint32_t address);
    IpAddress(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d);
    IpAddress(const std::uint8_t address[4]);

    // v6
    IpAddress(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d, std::uint16_t e,
              std::uint16_t f, std::uint16_t g, std::uint16_t h);
    IpAddress(const std::uint16_t address[8]);

    ~IpAddress() = default;

    /**
     * @brief      Gets information about address type. Can be either an IP v4/v6 or
     *             Invalid.
     *
     * @return     The type of the IP address.
     */
    inline IpType GetType() const
    {
        return m_type;
    }

    inline const std::uint8_t* GetIPv4() const
    {
        return m_ip.v4;
    }

    inline const std::uint16_t* GetIPv6() const
    {
        return m_ip.v6;
    }

    /**
     * @brief      Determines if address is a loopback address.
     *
     * @return     True if loopback, False otherwise.
     */
    bool IsLoopback();

    /**
     * @brief      Return IPv4 address as an 32bits integer. Assert if used on another
     *             address type.
     *
     * @return     IPv4 as an integer.
     */
    std::uint32_t AsInt() const;

    friend TEFNOUT_API bool operator==(const IpAddress& lhs, const IpAddress& rhs);
    friend TEFNOUT_API bool operator!=(const IpAddress& lhs, const IpAddress& rhs);

  private:
    IpType m_type;

    // @NOTE Prefers an std::variant over the union ??
    union {
        std::uint8_t v4[4];
        std::uint16_t v6[8];
    } m_ip;

    /// Used when building or extracting from a bitflag
    static constexpr std::uint8_t s_ipv4BitsSize = 8;
};

/**
 * @brief      Abstraction of an address composed of an IP and a Port. Address can be IPv4 or IPv6.
 */
class TEFNOUT_API Address
{
  public:
    Address();
    Address(IpAddress ipAddress, std::uint16_t port);
    ~Address() = default;

    IpAddress GetIp() const;
    std::uint16_t GetPort() const;

    friend TEFNOUT_API bool operator==(const Address& lhs, const Address& rhs);
    friend TEFNOUT_API bool operator!=(const Address& lhs, const Address& rhs);

  private:
    IpAddress m_ip;
    std::uint16_t m_port;
};

} // namespace Network
} // namespace Tefnout

#endif
