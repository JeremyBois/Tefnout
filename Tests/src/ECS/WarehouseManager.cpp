#include "Tefnout/Core/Logger.hpp"
#include "Tefnout/ECS/Entity.hpp"
#include "catch2/catch.hpp"

#include "Tefnout/ECS/WarehouseManager.hpp"

#include "DataStructure.hpp"


/*
 Unit-Tests for WarehouseManager implementation.
*/


TEST_CASE("Warehouse manager can be created", "[WarehouseManager]")
{
    auto mgr = Tefnout::ECS::WarehouseManager();

    SECTION("Cannot get a component from a warehouse if not yet added")
    {
        REQUIRE_THROWS(mgr.Get<Simple>(Tefnout::ECS::Entity{11}));
    }

    SECTION("Cannot remove a component from a warehouse when no yet a warehouse for this type")
    {
        REQUIRE_THROWS(mgr.Remove<Dynamic>(Tefnout::ECS::Entity{11}));
    }
}


TEST_CASE("Can manipulate wrapped warehouses from the manager", "[WarehouseManager]")
{
    auto mgr = Tefnout::ECS::WarehouseManager();

    std::array<std::pair<Tefnout::ECS::Entity, Simple>, 3> collection{};
    collection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Simple(4, 4.0f));
    collection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Simple(111, 99.0f));
    collection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Simple(0, 0.0f));

    SECTION("Can add a component to a warehouse when no yet a warehouse for this type")
    {
        auto& component1 = mgr.Add<Simple>(collection[0].first, 1, 11.0f);
        REQUIRE(mgr.Get<Simple>(collection[0].first) == component1);

        SECTION("Can add a component to a warehouse when already a warehouse for this type")
        {
            auto& component2 = mgr.Add<Simple>(collection[1].first, collection[1].second);
            REQUIRE(mgr.Get<Simple>(collection[1].first) == component2);
        }

        SECTION("Can remove a component from an existing warehouse")
        {
            REQUIRE_NOTHROW(mgr.Get<Simple>(collection[0].first));
            REQUIRE_NOTHROW(mgr.Remove<Simple>(collection[0].first));
        }
    }
}
