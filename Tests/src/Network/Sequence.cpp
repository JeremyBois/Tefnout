#include "catch2/catch.hpp"

#include "Tefnout/Network/Sequence.hpp"

#include "Tefnout/Core/Logger.hpp"
#include <cstdint>
#include <limits>

/*
 Unit-Tests for Network sequence helper functions.
*/

TEST_CASE("Sequence number can be compared to each other", "[Sequence]")
{
    using namespace Tefnout::Network;

    SECTION("Works when there is no overflow")
    {
        SECTION("Works for 8 bits unsigned int")
        {
            std::uint8_t seq1 = 2;
            std::uint8_t seq2 = 0;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 16 bits unsigned int")
        {
            std::uint16_t seq1 = 222;
            std::uint16_t seq2 = 0;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 32 bits unsigned int")
        {
            std::uint32_t seq1 = 223332;
            std::uint32_t seq2 = 0;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 64 bits unsigned int")
        {
            std::uint64_t seq1 = 22333242342342332;
            std::uint64_t seq2 = 0;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }
    }

    SECTION("Works with numbers far from each others")
    {
        SECTION("Works for 8 bits unsigned int")
        {
            std::uint8_t seq2 = 0;
            std::uint8_t seq1 = std::numeric_limits<decltype(seq2)>::max() / 2;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 16 bits unsigned int")
        {
            std::uint16_t seq2 = 0;
            std::uint16_t seq1 = std::numeric_limits<decltype(seq2)>::max() / 2;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 32 bits unsigned int")
        {
            std::uint32_t seq2 = 0;
            std::uint32_t seq1 = std::numeric_limits<decltype(seq2)>::max() / 2;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 64 bits unsigned int")
        {
            std::uint64_t seq2 = 0;
            std::uint64_t seq1 = std::numeric_limits<decltype(seq2)>::max() / 2;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }
    }

    SECTION("Works even with overflow")
    {
        SECTION("Works for 8 bits unsigned int")
        {
            std::uint8_t seq1 = 22;
            std::uint8_t seq2 = std::numeric_limits<decltype(seq2)>::max() - 1;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 16 bits unsigned int")
        {
            std::uint16_t seq1 = 22;
            std::uint16_t seq2 = std::numeric_limits<decltype(seq2)>::max() - 1;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 32 bits unsigned int")
        {
            std::uint32_t seq1 = 22;
            std::uint32_t seq2 = std::numeric_limits<decltype(seq2)>::max() - 1;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }

        SECTION("Works for 64 bits unsigned int")
        {
            std::uint64_t seq1 = 22;
            std::uint64_t seq2 = std::numeric_limits<decltype(seq2)>::max() - 1;

            REQUIRE(IsGreater(seq1, seq2));
            REQUIRE(!IsGreater(seq2, seq1));
        }
    }
}
