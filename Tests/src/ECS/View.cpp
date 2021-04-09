#include "Tefnout/Core/Logger.hpp"
#include "catch2/catch.hpp"

#include "Tefnout/ECS/ComponentSparseSet.hpp"
#include "Tefnout/ECS/View.hpp"

#include <limits>

/*
 Unit-Tests for SparseSet View implementation.
*/

struct Simple
{
    int aInt = 3;
    float aFloat = 2;

    Simple() = default;

    Simple(int valInt, float valFloat) : aInt{valInt}, aFloat{valFloat}
    {
    }

    friend bool operator==(const Simple& first, const Simple& second)
    {
        return first.aInt == second.aInt && first.aFloat == second.aFloat;
    }
    friend bool operator!=(const Simple& first, const Simple& second)
    {
        return first.aInt != second.aInt && first.aFloat == second.aFloat;
    }

    friend std::ostream& operator<<(std::ostream& os, const Simple& simple)
    {
        return os << "Simple<int=" << simple.aInt << ">- float=<" << simple.aFloat << ">";
    }
};

struct Dynamic
{
    int aInt = std::numeric_limits<int>::max();
    float aFloat = std::numeric_limits<float>::max();

    int* dynamic = new int[33];

    Dynamic() = default;

    ~Dynamic()
    {
        // std::cout << "Deleted" << std::endl;
        delete[] dynamic;
    }

    Dynamic(int valInt, float valFloat) : aInt{valInt}, aFloat{valFloat}
    {
    }

    Dynamic(const Dynamic& other) : aInt(other.aInt), aFloat(other.aFloat)
    {
        std::copy(other.dynamic, other.dynamic + 33, dynamic);
    }

    Dynamic(Dynamic&& other) : Dynamic()
    {
        swap(*this, other);
    }

    // Let the compiler do the copy for us
    Dynamic& operator=(Dynamic other)
    {
        swap(*this, other);

        return *this;
    }

    friend void swap(Dynamic& first, Dynamic& second) noexcept
    {
        using std::swap;

        swap(first.aInt, second.aInt);
        swap(first.aFloat, second.aFloat);
        swap(first.dynamic, second.dynamic);
    }

    friend bool operator==(const Dynamic& first, const Dynamic& second)
    {
        return first.aInt == second.aInt && first.aFloat == second.aFloat;
    }
    friend bool operator!=(const Dynamic& first, const Dynamic& second)
    {
        return first.aInt != second.aInt || first.aFloat != second.aFloat;
    }

    friend std::ostream& operator<<(std::ostream& os, const Dynamic& dynamic)
    {
        return os << "Dynamic{int=" << dynamic.aInt << " float=" << dynamic.aFloat << "}";
    }
};

TEST_CASE("SparseSet view can be created", "[SparseSet View]")
{
    Tefnout::ECS::ComponentSparseSet<Dynamic> dynamicContainer{};
    Tefnout::ECS::ComponentSparseSet<Simple> simpleContainer{};

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
    Tefnout::ECS::ComponentSparseSet<Dynamic> dynamicContainer{};
    Tefnout::ECS::ComponentSparseSet<Simple> simpleContainer{};
    Tefnout::ECS::ComponentSparseSet<int> intContainer{};
    Tefnout::ECS::ComponentSparseSet<float> floatContainer{};
    Tefnout::ECS::ComponentSparseSet<double> doubleContainer{};
    Tefnout::ECS::ComponentSparseSet<std::string> stringContainer{};

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
}
