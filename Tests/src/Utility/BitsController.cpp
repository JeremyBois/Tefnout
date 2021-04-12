#include "Tefnout/Core/Logger.hpp"
#include "catch2/catch.hpp"

#include "Tefnout/Utility/BitsController.hpp"
#include <array>
#include <cstdint>

/*
 Unit-Tests for BitsController helper functions.
*/

TEST_CASE("Tests bits manipulation", "[BitsController]")
{
    using namespace Tefnout::Utility;

    // Values to be inserted
    std::array<Tefnout::Utility::Bits::ValueOffsetPair, 3> values{};
    values[0] = {2, 0};
    values[1] = {3, 2};
    values[2] = {4, 8};

    SECTION("Can extract a specific range a bits from a single int64")
    {
        const std::uint64_t container = 1038; // 0b10000001110

        REQUIRE(Bits::Extract(container, 0u, 0u) == 0); // 0b1000000111[0]
        REQUIRE(Bits::Extract(container, 0u, 2u) == 2); // 0b100000011[10]
        REQUIRE(Bits::Extract(container, 2u, 2u) == 3); // 0b1000000[11]10
        REQUIRE(Bits::Extract(container, 8u, 3u) == 4); // 0b[100]00001110
    }

    SECTION("Can set a specific value at a specific position inside a single int64")
    {
        const std::uint64_t container = 1038; // 0b10000001110

        std::uint64_t builtContainer{};

        for (const auto& [value, offset] : values)
        {
            builtContainer = Bits::Set(builtContainer, value, offset);
        }

        REQUIRE(container == builtContainer);
    }

    SECTION("Can store multiple values inside an int64")
    {
        const std::uint64_t container = 1038; // 0b10000001110

        std::uint64_t builtContainer = Bits::Set(values);

        REQUIRE(container == builtContainer);

        SECTION("When using ClearAndSet old value is first cleared")
        {
            builtContainer = Bits::ClearAndSet(builtContainer, 3ul, 8u, 3u);
            REQUIRE(Bits::Extract(builtContainer, 8u, 3u) == 3); // 0b[011]00001110
        }

        SECTION("When using Set old value is NOT first cleared")
        {
            builtContainer = Bits::Set(builtContainer, 3ul, 8u);
            REQUIRE(Bits::Extract(builtContainer, 8u, 3u) == 7); // 0b[111]00001110
        }
    }

    SECTION("Can clear a specfic range of bits")
    {
        const std::uint64_t container = 1038; // 0b10000001110

        REQUIRE(Bits::Clear(container, 8u, 3u) == 14);   // 0b1110
        REQUIRE(Bits::Clear(container, 0u, 2u) == 1036); // 0b10000001100
    }
}

TEST_CASE("Tests bits manipulation for unsigned int", "[BitsController]")
{
    using namespace Tefnout::Utility;

    const unsigned char a = 127;
    const unsigned char b = 33;
    const unsigned char c = 0;
    const unsigned char d = 1;

    SECTION("Can create a packed unsigned int from unsigned char")
    {
        const unsigned char shift = 8;
        const unsigned int result = 0;
        const unsigned int resultWithA =
            Bits::Set(result, static_cast<unsigned int>(a), static_cast<unsigned char>(24));
        REQUIRE(Bits::Extract(resultWithA, static_cast<unsigned char>(24), shift) == a);

        const unsigned int resultWithB =
            Bits::Set(resultWithA, static_cast<unsigned int>(b), static_cast<unsigned char>(16));
        REQUIRE(Bits::Extract(resultWithB, static_cast<unsigned char>(24), shift) == a);
        REQUIRE(Bits::Extract(resultWithB, static_cast<unsigned char>(16), shift) == b);

        const unsigned int resultWithC =
            Bits::Set(resultWithB, static_cast<unsigned int>(c), static_cast<unsigned char>(8));
        REQUIRE(Bits::Extract(resultWithC, static_cast<unsigned char>(24), shift) == a);
        REQUIRE(Bits::Extract(resultWithC, static_cast<unsigned char>(16), shift) == b);
        REQUIRE(Bits::Extract(resultWithC, static_cast<unsigned char>(8), shift) == c);


        const unsigned int resultWithD =
            Bits::Set(resultWithC, static_cast<unsigned int>(d), static_cast<unsigned char>(0));
        REQUIRE(Bits::Extract(resultWithD, static_cast<unsigned char>(24), shift) == a);
        REQUIRE(Bits::Extract(resultWithD, static_cast<unsigned char>(16), shift) == b);
        REQUIRE(Bits::Extract(resultWithD, static_cast<unsigned char>(8), shift) == c);
        REQUIRE(Bits::Extract(resultWithD, static_cast<unsigned char>(0), shift) == d);
    }
}
