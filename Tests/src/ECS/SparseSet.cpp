#include "catch2/catch.hpp"

#include "Tefnout/ECS/ComponentSparseSet.hpp"

#include <limits>

/*
 Unit-Tests for ComponentSparseSet implementation.
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

TEST_CASE("ComponentSparseSet can be created", "[ComponentSparseSet]")
{

    SECTION("With object allocating memory only on the Stack")
    {
        Tefnout::ECS::ComponentSparseSet<Simple> simples{};

        REQUIRE(simples.IsEmpty());
        REQUIRE(simples.Size() == 0);
    }

    SECTION("With object using Stack and Heap memory")
    {
        Tefnout::ECS::ComponentSparseSet<Dynamic> sparseContainer{};

        REQUIRE(sparseContainer.Size() == 0);
    }
}

TEST_CASE("ComponentSparseSet can be printed")
{
    Tefnout::ECS::ComponentSparseSet<Dynamic> sparseContainer{};
    REQUIRE(sparseContainer.Size() == 0);

    // Can construct data directly inside the container neither move nor copy
    std::array<std::pair<Tefnout::ECS::Entity, Dynamic>, 3> collection{};
    collection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Dynamic(4, 4.0f));
    collection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Dynamic(111, 99.0f));
    collection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Dynamic(0, 0.0f));

    for (const auto [entity, data] : collection)
    {
        std::size_t size = sparseContainer.Size();
        sparseContainer.EmplaceBack(entity, data.aInt, data.aFloat);
        REQUIRE(sparseContainer.Contains(entity));
        REQUIRE(sparseContainer.Size() == size + 1);
    }

    std::string result("ComponentSparseSet{(Entity<id=33>, Dynamic{int=0 float=0})(Entity<id=100>, "
                       "Dynamic{int=111 float=99})(Entity<id=11>, Dynamic{int=4 float=4})}");
    REQUIRE(sparseContainer.ToString() == result);
}

TEST_CASE("ComponentSparseSet can be manipulated", "[ComponentSparseSet]")
{
    Tefnout::ECS::ComponentSparseSet<Dynamic> sparseContainer{};
    REQUIRE(sparseContainer.Size() == 0);

    SECTION("Can add element to a ComponentSparseSet")
    {
        const auto pair1 = std::make_pair(Tefnout::ECS::Entity{0}, Dynamic{1, 1.0f});
        const auto pair2 = std::make_pair(Tefnout::ECS::Entity{1}, Dynamic{2, 2.0f});
        const auto pair3 = std::make_pair(Tefnout::ECS::Entity{2}, Dynamic{3, 3.0f});
        const auto pair4 = std::make_pair(Tefnout::ECS::Entity{3}, Dynamic{4, 4.0f});

        // Can COPY data to container
        sparseContainer.PushBack(pair1.first, pair1.second);
        REQUIRE(sparseContainer.Size() == 1);

        // Can MOVE data to container
        sparseContainer.PushBack(pair2.first, Dynamic{});
        REQUIRE(sparseContainer.Size() == 2);

        // Can MOVE to container using EmplaceBack
        sparseContainer.EmplaceBack(pair3.first, pair3.second);
        REQUIRE(sparseContainer.Size() == 3);

        // Can construct inplace inside the container using EmplaceBack
        sparseContainer.EmplaceBack(pair4.first, pair4.second.aInt, pair4.second.aFloat);
        REQUIRE(sparseContainer.Size() == 4);

        SECTION("Data can be added without using a specific Entity ID order")
        {
            // Can construct data directly inside the container neither move nor copy
            std::array<std::pair<Tefnout::ECS::Entity, Dynamic>, 3> collection{};
            collection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Dynamic(5, 5.0f));
            collection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Dynamic(111, 99.0f));
            collection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Dynamic(0, 0.0f));

            for (const auto [entity, data] : collection)
            {
                std::size_t size = sparseContainer.Size();
                sparseContainer.EmplaceBack(entity, data.aInt, data.aFloat);
                REQUIRE(sparseContainer.Contains(entity));
                REQUIRE(sparseContainer.Size() == size + 1);
            }

            SECTION("Can get reference to added data")
            {
                auto data = sparseContainer.Get(collection[0].first);
                REQUIRE(data == collection[0].second);
                REQUIRE(data != collection[1].second);

                data = sparseContainer.Get(collection[1].first);
                REQUIRE(data == collection[1].second);

                data = sparseContainer.Get(collection[2].first);
                REQUIRE(data == collection[2].second);
            }
        }
    }
}

TEST_CASE("ComponentSparseSet components can be removed", "[ComponentSparseSet]")
{
    Tefnout::ECS::ComponentSparseSet<Dynamic> sparseContainer{};
    REQUIRE(sparseContainer.Size() == 0);

    // Can construct data directly inside the container neither move nor copy
    std::array<std::pair<Tefnout::ECS::Entity, Dynamic>, 3> collection{};
    collection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Dynamic(4, 4.0f));
    collection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Dynamic(111, 99.0f));
    collection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Dynamic(0, 0.0f));

    for (const auto [entity, data] : collection)
    {
        std::size_t size = sparseContainer.Size();
        sparseContainer.EmplaceBack(entity, data.aInt, data.aFloat);
        REQUIRE(sparseContainer.Contains(entity));
        REQUIRE(sparseContainer.Size() == size + 1);
    }

    SECTION("Can remove a single data")
    {
        std::size_t previousSize = sparseContainer.Size();

        REQUIRE(sparseContainer.Contains(collection[0].first));
        sparseContainer.Remove(collection[0].first);
        REQUIRE(sparseContainer.Contains(collection[0].first) == false);
        REQUIRE(sparseContainer.Size() == previousSize - 1);
    }

    SECTION("Can clear the whole container")
    {
        std::size_t previousSize = sparseContainer.Size();

        REQUIRE(sparseContainer.Contains(collection[0].first));
        REQUIRE(sparseContainer.Contains(collection[1].first));
        REQUIRE(sparseContainer.Contains(collection[2].first));

        sparseContainer.Clear();
        REQUIRE(sparseContainer.Contains(collection[0].first) == false);
        REQUIRE(sparseContainer.Contains(collection[1].first) == false);
        REQUIRE(sparseContainer.Contains(collection[2].first) == false);
        REQUIRE(sparseContainer.Size() == 0);
    }

    SECTION("Can remove a specific range data")
    {
        std::size_t previousSize = sparseContainer.Size();

        REQUIRE(sparseContainer.Contains(collection[0].first));
        REQUIRE(sparseContainer.Contains(collection[1].first));
        REQUIRE(sparseContainer.Contains(collection[2].first));

        // Start from last added minus one (keep only last added)
        sparseContainer.Remove(++sparseContainer.SparseSet::begin(),
                               sparseContainer.SparseSet::end());
        REQUIRE(sparseContainer.Contains(collection[0].first) == false);
        REQUIRE(sparseContainer.Contains(collection[1].first) == false);
        REQUIRE(sparseContainer.Contains(collection[2].first) == true);
        REQUIRE(sparseContainer.Size() == 1);
    }
}

TEST_CASE("ComponentSparseSet can be updated inplace", "[ComponentSparseSet]")
{
    Tefnout::ECS::ComponentSparseSet<Dynamic> sparseContainer{};
    REQUIRE(sparseContainer.Size() == 0);

    // Can construct data directly inside the container neither move nor copy
    std::array<std::pair<Tefnout::ECS::Entity, Dynamic>, 3> collection{};
    collection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Dynamic(4, 4.0f));
    collection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Dynamic(111, 99.0f));
    collection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Dynamic(0, 0.0f));

    for (const auto [entity, data] : collection)
    {
        std::size_t size = sparseContainer.Size();
        sparseContainer.EmplaceBack(entity, data.aInt, data.aFloat);
        REQUIRE(sparseContainer.Contains(entity));
        REQUIRE(sparseContainer.Size() == size + 1);
    }

    SECTION("Stored data can be updated inplace")
    {
        auto entity = collection[0].first;
        const int increment = 4;
        auto dummyUpdater = [increment](auto& data, const int anotherArg) {
            data.aInt += increment + anotherArg;
        };

        REQUIRE(sparseContainer.Size() == collection.size());
        REQUIRE(sparseContainer.Contains(entity));

        auto updatedData = sparseContainer.Update(entity, dummyUpdater, increment);

        REQUIRE(sparseContainer.Contains(entity));
        REQUIRE(sparseContainer.Size() == collection.size());
        REQUIRE(updatedData != collection[0].second);

        REQUIRE(updatedData.aInt == collection[0].second.aInt + increment + increment);
    }
}

TEST_CASE("ComponentSparseSet can be iterated", "[ComponentSparseSet]")
{
    Tefnout::ECS::ComponentSparseSet<Dynamic> sparseContainer{};
    REQUIRE(sparseContainer.Size() == 0);

    // Can construct data directly inside the container neither move nor copy
    std::array<std::pair<Tefnout::ECS::Entity, Dynamic>, 3> collection{};
    collection[0] = std::make_pair(Tefnout::ECS::Entity{11}, Dynamic(4, 4.0f));
    collection[1] = std::make_pair(Tefnout::ECS::Entity{100}, Dynamic(111, 99.0f));
    collection[2] = std::make_pair(Tefnout::ECS::Entity{33}, Dynamic(0, 0.0f));

    for (const auto [entity, data] : collection)
    {
        std::size_t size = sparseContainer.Size();
        sparseContainer.EmplaceBack(entity, data.aInt, data.aFloat);
        REQUIRE(sparseContainer.Contains(entity));
        REQUIRE(sparseContainer.Size() == size + 1);
    }

    SECTION("Can iterate over DATA in a forward (end to start) manner (mutable ref)")
    {
        std::size_t count = collection.size() - 1;
        for (auto& component : sparseContainer)
        {
            REQUIRE(collection[count].second == component);
            count--;
        }

        count = collection.size() - 1;
        for (auto it = sparseContainer.begin(); it != sparseContainer.end(); it++)
        {
            REQUIRE(collection[count].second == *it);
            count--;
        }
    }

    SECTION("Can iterate over DATA in a forward (end to start) manner (const ref)")
    {
        std::size_t count = collection.size() - 1;
        for (const auto& component : sparseContainer)
        {
            REQUIRE(collection[count].second == component);
            count--;
        }

        count = collection.size() - 1;
        for (auto it = sparseContainer.cbegin(); it != sparseContainer.cend(); it++)
        {
            REQUIRE(collection[count].second == *it);
            count--;
        }
    }

    SECTION("Can iterate over DATA in a backward (start to end) manner (mutable ref)")
    {
        std::size_t count = 0;
        for (auto it = sparseContainer.rbegin(); it != sparseContainer.rend(); it++)
        {
            REQUIRE(collection[count].second == *it);
            count++;
        }
    }

    SECTION("Can iterate over DATA in a backward (start to end) manner (const ref)")
    {
        std::size_t count = 0;
        for (auto it = sparseContainer.crbegin(); it != sparseContainer.crend(); it++)
        {
            REQUIRE(collection[count].second == *it);
            count++;
        }
    }

    SECTION("Can iterate over ENTITIES in a backward and forward manner (mutable ref)")
    {
        // Forward is the default and iterate from END to START
        std::size_t count = collection.size() - 1;
        for (auto it = sparseContainer.SparseSet::begin(); it != sparseContainer.SparseSet::end();
             it++)
        {
            REQUIRE(collection[count].first == *it);
            count--;
        }

        // Backward iterate from START to END
        count = 0;
        for (auto it = sparseContainer.SparseSet::rbegin(); it != sparseContainer.SparseSet::rend();
             it++)
        {
            REQUIRE(collection[count].first == *it);
            count++;
        }
    }
}
