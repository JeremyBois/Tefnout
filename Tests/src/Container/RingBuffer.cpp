#include "catch2/catch.hpp"

#include <algorithm>
#include <vector>

#include "Tefnout/Container/RingBuffer.hpp"

/*
 Unit-Tests for Ring buffer implementation.
 */

TEST_CASE("Ring buffer can be create", "[RingBuffer]")
{
    // Create before each section
    auto buffer = Tefnout::Buffer::Ring<int, 5>{};
    std::array<int, 5> values{0, 1, 2, 3, 4};

    REQUIRE(buffer.IsEmpty() == true);
    REQUIRE(buffer.Back() == buffer.Front());
    REQUIRE(buffer.Size() == 0);
    REQUIRE(buffer.IsPending() == false);

    // Start with TEST_CASE code for each SECTION (setup)
    SECTION("Pushing an item increase the buffer size")
    {
        std::for_each(values.begin(), values.end(), [&buffer](int value) -> void {
            REQUIRE(buffer.Push(value) == Tefnout::Buffer::Response::Ok);
            REQUIRE(buffer.IsPending() == true);
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

        SECTION("Poping an item allow to push a new one")
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

    SECTION("Forward MUTABLE iterator allows to loop over all pending elements in reverse order")
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
