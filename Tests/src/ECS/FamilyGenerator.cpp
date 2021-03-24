#include "catch2/catch.hpp"

#include "Tefnout/ECS/FamilyGenerator.hpp"

/*
 Unit-Tests for FamilyGenerator implementation.
*/

TEST_CASE("FamilyGenerator ID are unique to a type", "[FamilyGenerator]")
{

    auto a = Tefnout::ECS::FamilyGenerator::GetIdentifier<int>();
    auto aBis = Tefnout::ECS::FamilyGenerator::GetIdentifier<int>();
    auto aBisBis = Tefnout::ECS::FamilyGenerator::GetIdentifier<int>();
    auto b = Tefnout::ECS::FamilyGenerator::GetIdentifier<float>();
    auto c = Tefnout::ECS::FamilyGenerator::GetIdentifier<long>();

    // Different types
    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(b != c);

    // Same type
    REQUIRE(a == aBis);
    REQUIRE(a == aBisBis);
}
