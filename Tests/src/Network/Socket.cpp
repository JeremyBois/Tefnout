#include "Tefnout/Network/NetworkCore.hpp"
#include "catch2/catch.hpp"

#include "Tefnout/Network/Address.hpp"
#include "Tefnout/Network/Socket.hpp"

#include "Tefnout/Core/Logger.hpp"
#include <cstring>

/*
 Unit-Tests for Socket helper functions.
*/

TEST_CASE("Can create a socket", "[Socket]")
{
    using namespace Tefnout;

    SECTION("Can create an IPv4 ip address")
    {
        const Network::IpAddress ip{};

        REQUIRE(ip.GetA() == 127);
        REQUIRE(ip.GetB() == 0);
        REQUIRE(ip.GetC() == 0);
        REQUIRE(ip.GetD() == 1);

        SECTION("Can create an IPv4 address")
        {
            Network::Address address(ip, 0);

            REQUIRE(address.GetPort() == 0);
            REQUIRE(address.GetIp() == ip);

            SECTION("Can create a Socket and bind it")
            {
                Network::Socket socket{};
                REQUIRE(socket.GetHandle() != SOCKET_ERROR);
                REQUIRE(socket.GetMode() == Network::SocketMode::NonBlocking);

                bool result = socket.Bind(address);
                REQUIRE(result);
            }
        }
    }
}

TEST_CASE("Can send and receive data", "[Socket]")
{
    using namespace Tefnout;

    Network::Address address(Network::IpAddress{}, 30000);
    Network::Socket socket{};
    socket.Bind(address);

    SECTION("Can send data")
    {
        const char data[] = "hello world!";
        const bool result = socket.SendTo(address, data, sizeof(data), 0);
        REQUIRE(result);

        SECTION("Can receive data")
        {
            while (true)
            {
                Network::Address sender;
                unsigned char buffer[256];
                int bytes_read = socket.ReceiveFrom(sender, buffer, sizeof(buffer), 0);
                if (!bytes_read)
                    REQUIRE(std::strcmp((char*)buffer, data) == 0);
                    break;
            }
        }
    }
}
