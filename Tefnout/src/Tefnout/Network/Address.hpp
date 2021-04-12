#ifndef __ADDRESS__HPP
#define __ADDRESS__HPP

#include "Tefnout/Core/Platform.hpp"

namespace Tefnout
{
namespace Network
{

/**
 * @brief      Abstraction over an IPv4 address.
 *
 * @note       Representation is as follow - 127.0.0.1 --> A.B.C.D
 */
struct TEFNOUT_API IpAddress
{
  public:
    static constexpr unsigned char s_Aoffset = 24;
    static constexpr unsigned char s_Boffset = 16;
    static constexpr unsigned char s_Coffset = 8;
    static constexpr unsigned char s_Doffset = 0;
    static constexpr unsigned char s_fieldSize = 8;

    IpAddress();
    IpAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
    explicit IpAddress(unsigned int ipAddressAsInt);
    ~IpAddress() = default;

    unsigned char GetA() const;
    unsigned char GetB() const;
    unsigned char GetC() const;
    unsigned char GetD() const;

    /**
     * @brief      Return address bitmask.
     *
     * @return     Address bitmask.
     */
    unsigned int AsInt() const;


    friend bool operator==(const IpAddress& lhs, const IpAddress& rhs);
    friend bool operator!=(const IpAddress& lhs, const IpAddress& rhs);

  private:
    unsigned int m_ip;
};

/**
 * @brief      Abstraction of an address composed of an IP and a Port.
 */
class TEFNOUT_API Address
{
  public:
    Address();
    Address(IpAddress ipAddress, unsigned short port);
    ~Address() = default;

    IpAddress GetIp() const;
    unsigned short GetPort() const;

    friend bool operator==(const Address& lhs, const Address& rhs);
    friend bool operator!=(const Address& lhs, const Address& rhs);

  private:
    IpAddress m_ip;
    unsigned short m_port;
};

} // namespace Network
} // namespace Tefnout

#endif
