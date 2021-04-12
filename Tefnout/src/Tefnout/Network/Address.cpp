#include "TefnoutPCH.hpp"

#include "Address.hpp"

#include "Tefnout/Utility/BitsController.hpp"

namespace Tefnout
{
namespace Network
{

// IpAddress

IpAddress::IpAddress() : IpAddress(127, 0, 0, 1)
{
}

IpAddress::IpAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d) : m_ip(0)
{
    using namespace Utility;

    m_ip = Bits::Set(m_ip, static_cast<unsigned int>(a), s_Aoffset);
    m_ip = Bits::Set(m_ip, static_cast<unsigned int>(b), s_Boffset);
    m_ip = Bits::Set(m_ip, static_cast<unsigned int>(c), s_Coffset);
    m_ip = Bits::Set(m_ip, static_cast<unsigned int>(d), s_Doffset);
}

IpAddress::IpAddress(unsigned int ipAddressAsInt) : m_ip(ipAddressAsInt)
{
}

unsigned int IpAddress::AsInt() const
{
    return m_ip;
}

unsigned char IpAddress::GetA() const
{
    return static_cast<unsigned char>(Utility::Bits::Extract(m_ip, s_Aoffset, s_fieldSize));
}

unsigned char IpAddress::GetB() const
{
    return static_cast<unsigned char>(Utility::Bits::Extract(m_ip, s_Boffset, s_fieldSize));
}

unsigned char IpAddress::GetC() const
{
    return static_cast<unsigned char>(Utility::Bits::Extract(m_ip, s_Coffset, s_fieldSize));
}

unsigned char IpAddress::GetD() const
{
    return static_cast<unsigned char>(Utility::Bits::Extract(m_ip, s_Doffset, s_fieldSize));
}

bool operator==(const IpAddress& lhs, const IpAddress& rhs)
{
    return lhs.m_ip == rhs.m_ip;
}

bool operator!=(const IpAddress& lhs, const IpAddress& rhs)
{
    return !(lhs == rhs);
}


// Address

Address::Address() : Address(IpAddress(), 0)
{
}

Address::Address(IpAddress ipAddress, unsigned short port = 0) : m_ip(ipAddress), m_port(port)
{
}

IpAddress Address::GetIp() const
{
    return m_ip;
}

unsigned short Address::GetPort() const
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
