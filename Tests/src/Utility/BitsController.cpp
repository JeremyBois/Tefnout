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

        REQUIRE(Bits::Extract(container, 0, 0) == 0); // 0b1000000111[0]
        REQUIRE(Bits::Extract(container, 0, 2) == 2); // 0b100000011[10]
        REQUIRE(Bits::Extract(container, 2, 2) == 3); // 0b1000000[11]10
        REQUIRE(Bits::Extract(container, 8, 3) == 4); // 0b[100]00001110
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
            builtContainer = Bits::ClearAndSet(builtContainer, 3, 8, 3);
            REQUIRE(Bits::Extract(builtContainer, 8, 3) == 3); // 0b[011]00001110
        }

        SECTION("When using Set old value is NOT first cleared")
        {
            builtContainer = Bits::Set(builtContainer, 3, 8);
            REQUIRE(Bits::Extract(builtContainer, 8, 3) == 7); // 0b[111]00001110
        }
    }

    SECTION("Can clear a specfic range of bits")
    {
        const std::uint64_t container = 1038; // 0b10000001110

        REQUIRE(Bits::Clear(container, 8, 3) == 14);   // 0b1110
        REQUIRE(Bits::Clear(container, 0, 2) == 1036); // 0b10000001100
    }
}
