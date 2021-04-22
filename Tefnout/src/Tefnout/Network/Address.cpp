#include "Tefnout/Core/Debug.hpp"
#include "TefnoutPCH.hpp"

#include "Address.hpp"

#include "Tefnout/Utility/BitsController.hpp"

#include <cstdint>
#include <cstring>

namespace Tefnout
{
namespace Network
{

// IpAddress
IpAddress::IpAddress() : m_type(IpType::v4), m_ip{{127, 0, 0, 1}}
{
}

IpAddress::IpAddress(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
    : m_type(IpType::v4), m_ip{{a, b, c, d}}
{
}

IpAddress::IpAddress(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d,
                     std::uint16_t e, std::uint16_t f, std::uint16_t g, std::uint16_t h)
    : m_type(IpType::v6), m_ip{}
{
    m_ip.v6[0] = a;
    m_ip.v6[1] = b;
    m_ip.v6[2] = c;
    m_ip.v6[3] = d;
    m_ip.v6[4] = e;
    m_ip.v6[5] = f;
    m_ip.v6[6] = g;
    m_ip.v6[7] = h;
}

IpAddress::IpAddress(const std::uint16_t address[8]) : m_type(IpType::v6), m_ip{}
{
    for (int i = 0; i < 8; ++i)
        m_ip.v6[i] = address[i];
}

IpAddress::IpAddress(const std::uint8_t address[4]) : m_type(IpType::v4), m_ip{}
{
    for (int i = 0; i < 4; ++i)
        m_ip.v4[i] = address[i];
}

IpAddress::IpAddress(std::uint32_t address) : m_type(IpType::v4), m_ip{}
{
    using namespace Utility;

    m_ip.v4[0] = Bits::Extract(address, static_cast<std::uint8_t>(24u), s_ipv4BitsSize);
    m_ip.v4[1] = Bits::Extract(address, static_cast<std::uint8_t>(16u), s_ipv4BitsSize);
    m_ip.v4[2] = Bits::Extract(address, static_cast<std::uint8_t>(8u), s_ipv4BitsSize);
    m_ip.v4[3] = Bits::Extract(address, static_cast<std::uint8_t>(0u), s_ipv4BitsSize);
}

bool IpAddress::IsLoopback()
{
    // clang-format off
    switch (m_type)
    {
    case IpType::v4:
        return (m_ip.v4[0] == 127 &&
                m_ip.v4[1] == 0   &&
                m_ip.v4[2] == 0   &&
                m_ip.v4[3] == 1
                );
    case IpType::v6:
        return (m_ip.v6[0] == 0 &&
                m_ip.v6[1] == 0 &&
                m_ip.v6[2] == 0 &&
                m_ip.v6[3] == 0 &&
                m_ip.v6[4] == 0 &&
                m_ip.v6[5] == 0 &&
                m_ip.v6[6] == 0 &&
                m_ip.v6[7] == 0x0001);
    default:
        return false;
    }
    // clang-format on
}

std::uint32_t IpAddress::AsInt() const
{
    TEFNOUT_ASSERT(m_type == IpType::v4, "Cannot build a network representation {0}",
                   "of an address only for IPv4 address.");

    std::uint32_t result{0u};
    {
        using namespace Utility;
        Bits::Set(result, m_ip.v4[0], s_ipv4BitsSize);
        Bits::Set(result, m_ip.v4[1], s_ipv4BitsSize);
        Bits::Set(result, m_ip.v4[2], s_ipv4BitsSize);
        Bits::Set(result, m_ip.v4[3], s_ipv4BitsSize);
    }

    return result;
}

bool operator==(const IpAddress& lhs, const IpAddress& rhs)
{
    if (lhs.m_type != rhs.m_type)
    {
        return false;
    }
    if (lhs.m_type == IpType::v4)
    {
        // Test memory directly because type is trivial
        return std::memcmp(lhs.m_ip.v4, rhs.m_ip.v4, sizeof(lhs.m_ip.v4)) == 0;
    }
    else if (lhs.m_type == IpType::v6)
    {
        // Test memory directly because type is trivial
        return std::memcmp(lhs.m_ip.v6, rhs.m_ip.v6, sizeof(lhs.m_ip.v6)) == 0;
    }
    else
    {
        return false;
    }
}

bool operator!=(const IpAddress& lhs, const IpAddress& rhs)
{
    return !(lhs == rhs);
}

// Address

Address::Address() : Address(IpAddress(), 0)
{
}

Address::Address(IpAddress ipAddress, std::uint16_t port = 0) : m_ip(ipAddress), m_port(port)
{
}

IpAddress Address::GetIp() const
{
    return m_ip;
}

std::uint16_t Address::GetPort() const
{
    return m_port;
}

bool operator==(const Address& lhs, const Address& rhs)
{
    return lhs.m_ip == rhs.m_ip && lhs.m_port == rhs.m_port;
}

bool operator!=(const Address& lhs, const Address& rhs)
{
    return !(lhs == rhs);
}

} // namespace Network
} // namespace Tefnout
