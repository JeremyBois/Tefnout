#include "Test.hpp"

#include "Tefnout/Container/RingBuffer.hpp"

#include <bits/c++config.h>
#include <cassert>
#include <iostream>

namespace Test
{
void RingBuffer_TEST()
{
    // Test ring buffer
    std::cout << "Create a Ring buffer of size 5" << std::endl;
    auto buffer = Tefnout::Buffer::Ring<int, 5>{};

    // Test Push
    std::cout << "Push until size limit is reached" << std::endl;
    buffer.Push(0);
    buffer.Push(1);
    buffer.Push(2);
    buffer.Push(3);
    buffer.Push(4);
    // Should overflow
    std::cout << "Pushing again should Overflow" << std::endl;
    std::cout << "Using TryPush push only if overflow will not occurs and warns" << std::endl;
    std::cout << "Overflow occurs = " << (buffer.TryPush(5) == Tefnout::Buffer::Response::Overflow) << std::endl;
    assert(buffer.Size() == (std::size_t(5)));

    std::cout << "Test front / back" << std::endl;
    std::cout << buffer.Front() << std::endl;
    std::cout << buffer.Back() << std::endl;
    std::cout << std::endl;

    std::cout << "Forward loop (mutable) --> 0 to 4" << std::endl;
    for (auto i : buffer)
    {
        std::cout << i << "\n";
    }

    std::cout << "Backward loop (mutable) --> 4 to 0" << std::endl;
    for (auto it = buffer.rbegin(); it != buffer.rend(); it++)
    {
        std::cout << *it << '\n';
    }


    std::cout << "Forward loop (const) --> 0 to 4" << std::endl;
    for (auto it = buffer.cbegin(); it != buffer.cend(); it++)
    {
        std::cout << *it << '\n';
    }

    std::cout << "Backward loop (const) --> 4 to 0" << std::endl;
    for (auto it = buffer.crbegin(); it != buffer.crend(); it++)
    {
        std::cout << *it << '\n';
    }
    std::cout << std::endl;

    // Test Pop / Push starting from overflow risk
    std::cout << "Pop when full works"
         << "(Is full = " << buffer.IsFull() << ")" << std::endl;
    std::cout << "Pop value = " << buffer.Pop() << std::endl;
    assert(buffer.Size() == (std::size_t(4)));
    std::cout << "Pushing should now not overflow"
         << "(Is full = " << buffer.IsFull() << ")" << std::endl;
    buffer.Push(5);
    assert(buffer.Size() == (std::size_t(5)));
}
} // namespace Test
