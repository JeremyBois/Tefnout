#include "catch2/catch.hpp"

#include "Tefnout/ECS/Entity.hpp"
#include <cstdint>

/*
 Unit-Tests for Entity implementation.
*/

TEST_CASE("Entity can be created", "[Entity]")
{
    SECTION("Empty constructor always returns the same value")
    {
        const auto newNull = Tefnout::ECS::Entity{};

        REQUIRE(newNull == Tefnout::ECS::nullEntity);
    }

    SECTION("Non empty constructor let choose a specific ID")
    {
        const std::uint64_t id = 22u;
        const auto newEntity = Tefnout::ECS::Entity{id};

        REQUIRE(newEntity.GetId() == id);

        SECTION("Can compare two entities")
        {
            const auto anotherEntity = Tefnout::ECS::Entity{id};

            REQUIRE(newEntity == anotherEntity);
            REQUIRE(newEntity != Tefnout::ECS::nullEntity);
        }
    }

    SECTION("Can copy Entity")
    {
        const std::uint64_t id = 22u;
        auto newEntity = Tefnout::ECS::Entity{id};

        auto copiedEntity(newEntity);
        REQUIRE(newEntity == copiedEntity);

        auto anotherCopiedEntity = newEntity;
        REQUIRE(newEntity == anotherCopiedEntity);
    }
}
