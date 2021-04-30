#include "Tefnout/Core/Logger.hpp"
#include "catch2/catch.hpp"

#include "Tefnout/ECS/View.hpp"
#include "Tefnout/ECS/Warehouse.hpp"

#include "DataStructure.hpp"

/*
 Unit-Tests for SparseSet View implementation.
*/

TEST_CASE("SparseSet view can be created", "[SparseSet View]")
{
    Tefnout::ECS::Warehouse<Dynamic> dynamicContainer{};
    Tefnout::ECS::Warehouse<Simple> simpleContainer{};

    // Only 3 entities have a Simple component
    std::array<std::pair<Tefnout::ECS::Entity, Dynamic>, 3> dynamicCollection{};
    dynamicCollection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Dynamic(11, 11.0f));
    dynamicCollection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Dynamic(22, 22.0f));
    dynamicCollection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Dynamic(33, 33.0f));
    for (int i = 0; i < dynamicCollection.size(); ++i)
    {
        std::size_t sizeD = dynamicContainer.Size();
        const auto& d = dynamicCollection[i];
        dynamicContainer.EmplaceBack(d.first, d.second.aInt, d.second.aFloat);
        REQUIRE(dynamicContainer.Contains(d.first));
        REQUIRE(dynamicContainer.Size() == sizeD + 1);
    }

    // Only 2 entities have a Simple component
    std::array<std::pair<Tefnout::ECS::Entity, Simple>, 2> simpleCollection{};
    simpleCollection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Simple(50, 5.0f));
    simpleCollection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Simple(111, 99.0f));
    for (int i = 0; i < simpleCollection.size(); ++i)
    {
        std::size_t sizeS = simpleContainer.Size();
        const auto& s = simpleCollection[i];
        simpleContainer.EmplaceBack(s.first, s.second.aInt, s.second.aFloat);
        REQUIRE(simpleContainer.Contains(s.first));
        REQUIRE(simpleContainer.Size() == sizeS + 1);
    }

    SECTION("Can create a view")
    {
        const auto view = Tefnout::ECS::View<Simple, Dynamic>();
    }

    SECTION("Can create a view from two sparseSet")
    {
        const auto view = Tefnout::ECS::View(dynamicContainer, simpleContainer);
    }

    SECTION("Can iterate over a view")
    {
        const auto view = Tefnout::ECS::View(dynamicContainer, simpleContainer);

        std::size_t counter = 0;

        for (const auto entity : view)
        {
            REQUIRE(dynamicContainer.Contains(entity));
            REQUIRE(simpleContainer.Contains(entity));
            ++counter;
        }

        // Only iterate over entities that have both components
        REQUIRE(counter == std::min({simpleContainer.Size(), dynamicContainer.Size()}));
    }
}

TEST_CASE("SparseSet view can have any number of collections", "[SparseSet View]")
{
    Tefnout::ECS::Warehouse<Dynamic> dynamicContainer{};
    Tefnout::ECS::Warehouse<Simple> simpleContainer{};
    Tefnout::ECS::Warehouse<int> intContainer{};
    Tefnout::ECS::Warehouse<float> floatContainer{};
    Tefnout::ECS::Warehouse<double> doubleContainer{};
    Tefnout::ECS::Warehouse<std::string> stringContainer{};

    // Only 3 entities have a Simple component
    std::array<std::pair<Tefnout::ECS::Entity, Dynamic>, 3> dynamicCollection{};
    dynamicCollection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Dynamic(11, 11.0f));
    dynamicCollection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Dynamic(22, 22.0f));
    dynamicCollection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Dynamic(33, 33.0f));
    for (int i = 0; i < dynamicCollection.size(); ++i)
    {
        std::size_t sizeD = dynamicContainer.Size();
        const auto& d = dynamicCollection[i];
        dynamicContainer.EmplaceBack(d.first, d.second.aInt, d.second.aFloat);
        REQUIRE(dynamicContainer.Contains(d.first));
        REQUIRE(dynamicContainer.Size() == sizeD + 1);
    }

    // Only 2 entities have a Simple component
    std::array<std::pair<Tefnout::ECS::Entity, Simple>, 2> simpleCollection{};
    simpleCollection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Simple(50, 5.0f));
    simpleCollection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Simple(111, 99.0f));
    for (int i = 0; i < simpleCollection.size(); ++i)
    {
        std::size_t sizeS = simpleContainer.Size();
        const auto& s = simpleCollection[i];
        simpleContainer.EmplaceBack(s.first, s.second.aInt, s.second.aFloat);
        REQUIRE(simpleContainer.Contains(s.first));
        REQUIRE(simpleContainer.Size() == sizeS + 1);
    }

    // Only one for others
    const auto sharedEntity = simpleCollection[0].first;
    intContainer.EmplaceBack(sharedEntity, 999);
    floatContainer.EmplaceBack(sharedEntity, 6.6f);
    doubleContainer.EmplaceBack(sharedEntity, 1.1);
    stringContainer.EmplaceBack(sharedEntity, "Ohohoh");

    SECTION("View size depends on the maximum number of shared entities")
    {
        const auto view = Tefnout::ECS::View(dynamicContainer, simpleContainer, intContainer,
                                             floatContainer, doubleContainer, stringContainer);

        std::size_t counter = 0;

        for (const auto entity : view)
        {
            REQUIRE(dynamicContainer.Contains(entity));
            REQUIRE(simpleContainer.Contains(entity));
            REQUIRE(intContainer.Contains(entity));
            REQUIRE(floatContainer.Contains(entity));
            REQUIRE(doubleContainer.Contains(entity));
            REQUIRE(stringContainer.Contains(entity));
            ++counter;
        }

        // Only iterate over entities that have both components
        REQUIRE(counter == 1);
    }

    SECTION("Can access components from a view")
    {
        const auto view = Tefnout::ECS::View(dynamicContainer, simpleContainer, intContainer,
                                             floatContainer, doubleContainer, stringContainer);

        SECTION("Can access all components from the view")
        {
            for (const auto entity : view)
            {
                auto [dynamicComp, simpleComp, intComp, floatComp, doubleComp, stringComp] =
                    view.Components(entity);

                REQUIRE(dynamicContainer.Get(entity) == dynamicComp);
                REQUIRE(simpleContainer.Get(entity) == simpleComp);
                REQUIRE(intContainer.Get(entity) == intComp);
                REQUIRE(floatContainer.Get(entity) == floatComp);
                REQUIRE(doubleContainer.Get(entity) == doubleComp);
                REQUIRE(stringContainer.Get(entity) == stringComp);
            }
        }

        SECTION("Can access a specific component from the view")
        {
            for (const auto entity : view)
            {
                auto [dynamicComp] = view.Components<Dynamic>(entity);

                REQUIRE(dynamicContainer.Get(entity) == dynamicComp);
            }
        }

        SECTION("Can access specific components list from the view")
        {
            for (const auto entity : view)
            {
                auto [dynamicComp, intComp, doubleComp, stringComp] =
                    view.Components<Dynamic, int, double, std::string>(entity);

                REQUIRE(dynamicContainer.Get(entity) == dynamicComp);
                REQUIRE(intContainer.Get(entity) == intComp);
                REQUIRE(doubleContainer.Get(entity) == doubleComp);
                REQUIRE(stringContainer.Get(entity) == stringComp);
            }
        }
    }
}
