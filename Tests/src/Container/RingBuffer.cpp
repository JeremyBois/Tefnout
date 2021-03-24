#include "Tefnout/Core/Logger.hpp"
#include "catch2/catch.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include "Tefnout/Container/RingBuffer.hpp"

/*
 Unit-Tests for Ring buffer implementation.
*/

TEST_CASE("Ring buffer can be created", "[RingBuffer]")
{
    // Create before each section
    auto buffer = Tefnout::Buffer::Ring<int, 5>{};
    std::array<int, 5> values{0, 1, 2, 3, 4};

    REQUIRE(buffer.IsEmpty() == true);
    REQUIRE(buffer.Back() == buffer.Front());
    REQUIRE(buffer.Capacity() == 5);
    REQUIRE(buffer.Size() == 0);
    REQUIRE(buffer.IsPending() == false);

    // Start with TEST_CASE code for each SECTION (setup)
    SECTION("Pushing an item increase the buffer size but not the max size.")
    {
        std::for_each(values.begin(), values.end(), [&buffer](int value) -> void {
            REQUIRE(buffer.Push(value) == Tefnout::Buffer::Response::Ok);
            REQUIRE(buffer.IsPending() == true);
            REQUIRE(buffer.Capacity() == 5);
        });

        REQUIRE(buffer.Size() == 5);
        REQUIRE(buffer.IsEmpty() == false);
        REQUIRE(buffer.IsFull() == true);

        // Section can be nested
        SECTION("Pushing more item than buffer capacity will overflow")
        {
            // With try we prevent the Overflow but still get warning because nothing is added
            REQUIRE(buffer.TryPush(5) == Tefnout::Buffer::Response::Abort);
            REQUIRE(buffer.Size() == 5);
            REQUIRE(buffer.Front() == 0);
            REQUIRE(buffer.Back() == 4);

            // Without Try item is added and overflow occurs
            REQUIRE(buffer.Push(5) == Tefnout::Buffer::Response::Overflow);
            REQUIRE(buffer.Size() == 5);
            REQUIRE(buffer.Front() == 1);
            REQUIRE(buffer.Back() == 5);
        }

        SECTION("Poping an item when buffer is full allows to push a new one")
        {
            REQUIRE(buffer.IsFull() == true);
            REQUIRE(buffer.Size() == 5);
            auto front = buffer.Front();
            REQUIRE(front == 0);

            auto item = buffer.Pop();
            REQUIRE(item == front);
            REQUIRE(buffer.Size() == 4);
            REQUIRE(buffer.IsFull() == false);

            auto new_item = 5;
            buffer.Push(new_item);
            REQUIRE(buffer.Size() == 5);
            REQUIRE(buffer.Back() == new_item);
            REQUIRE(buffer.Front() == 1);
            REQUIRE(buffer.IsFull() == true);
        }
    }
}

TEST_CASE("Ring buffer memory is cyclic", "[RingBuffer]")
{
    std::array<int, 5> finalValues{16, 17, 18, 19, 20};
    auto buffer = Tefnout::Buffer::Ring<int, 5>{};
    auto values = std::array<int, 20>{};
    std::iota(values.begin(), values.end(), 1);

    for (auto &element : values)
    {
        if (buffer.IsFull())
        {
            buffer.Pop();
        }

        buffer.Push(element);
    }

    // Buffer should contains last 5 added elements
    auto result = std::equal(buffer.begin(), buffer.end(), finalValues.begin());
    REQUIRE(result == true);
}

TEST_CASE("Ring buffer can be iterated", "[RingBuffer]")
{
    // Create before each section
    std::vector<int> pending_values{};
    auto buffer = Tefnout::Buffer::Ring<int, 5>{};
    std::array<int, 3> values{0, 1, 2};

    // Add some 3 element to the buffer
    std::for_each(values.begin(), values.end(),
                  [&buffer](int value) -> void { buffer.Push(value); });
    auto bufferSize = buffer.Size();

    REQUIRE(bufferSize == 3);

    SECTION("Forward CONST iterator allows to loop over all pending elements")
    {
        for (const auto i : buffer)
        {
            pending_values.emplace_back(i);
        }

        auto result = std::equal(pending_values.begin(), pending_values.end(), values.begin());
        REQUIRE(result == true);
        REQUIRE(buffer.Size() == bufferSize);
    }
    SECTION("Backward CONST iterator allows to loop over pending elements in reverse order")
    {
        for (auto it = buffer.crbegin(); it != buffer.crend(); it++)
        {
            pending_values.emplace_back(*it);
        }

        auto result = std::equal(pending_values.begin(), pending_values.end(), values.rbegin());
        REQUIRE(result == true);
        REQUIRE(buffer.Size() == bufferSize);
    }

    SECTION("Forward MUTABLE iterator allows to loop over all pending elements from oldest to last "
            "added")
    {
        for (auto it = buffer.begin(); it != buffer.end(); it++)
        {
            pending_values.emplace_back(*it);
        }

        auto result = std::equal(pending_values.begin(), pending_values.end(), values.begin());
        REQUIRE(result == true);
        REQUIRE(buffer.Size() == bufferSize);
    }

    SECTION("Backward MUTABLE iterator allows to loop over all pending elements in reverse order")
    {
        for (auto it = buffer.rbegin(); it != buffer.rend(); it++)
        {
            pending_values.emplace_back(*it);
        }

        auto result = std::equal(pending_values.begin(), pending_values.end(), values.rbegin());
        REQUIRE(result == true);
        REQUIRE(buffer.Size() == bufferSize);
    }
}

TEST_CASE("EmplaceBack allows to avoid moving memory from user stack to storage stack",
          "[RingBuffer]")
{
    struct Dummy
    {
      public:
        Dummy() : myInt{0}, myFloat{1.0f}, myString{"Dummy"}
        {
        }

        Dummy(int aVal) : myInt{aVal}, myFloat{1.0f}, myString{"Dummy"}
        {
        }

        explicit Dummy(int aVal, float bVal, const std::string &cVal) : myInt{aVal}, myFloat{bVal}, myString{cVal}
        {
        }

        int myInt;
        float myFloat;
        std::string myString;
    };

    auto buffer = Tefnout::Buffer::Ring<Dummy, 5>{};

    auto& a = buffer.Emplace();
    auto& b = buffer.Emplace(22);
    auto& c = buffer.Emplace(999, 199.0f, "Emplaced");

    REQUIRE(a.myInt == 0);
    REQUIRE(b.myInt == 22);
    REQUIRE(c.myInt == 999);

    REQUIRE(a.myFloat == 1.0f);
    REQUIRE(b.myFloat == 1.0f);
    REQUIRE(c.myFloat == 199.0f);

    REQUIRE(a.myString == "Dummy");
    REQUIRE(b.myString == "Dummy");
    REQUIRE(c.myString == "Emplaced");
}
