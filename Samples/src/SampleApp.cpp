#include "Tefnout.hpp"
#include "Tefnout/Container/RingBuffer.hpp"
#include "Tefnout/Core/Logger.hpp"


class SampleApp : public Tefnout::Application
{
public:
    SampleApp() = default;
    ~SampleApp() = default;
};

Tefnout::Application* Tefnout::CreateApplication()
{
    // Test ring buffer
    auto buffer = Buffer::Ring<int, 5> {};

    cout << "Add until size limit" << endl;
    buffer.Push(0);
    buffer.Push(1);
    buffer.Push(2);
    buffer.Push(3);
    buffer.Push(4);
    // Should overflow
    cout << "Add more than capacity --> should Overflow" << endl;
    cout << (buffer.TryPush(5) == Buffer::Response::Overflow) << endl;

    cout << "Test front / back" << endl;
    cout << buffer.Front() << endl;
    cout << buffer.Back() << endl;
    cout << endl;

    cout << "Loop over (non-const) from 0 to 4" << endl;
    for (auto i : buffer)
    {
        cout << "for: " << i << "\n";
    }
    cout << endl;

    cout << "Loop over (const with iterator) from 0 to 4" << endl;
    for (auto it = buffer.cbegin(); it != buffer.cend(); it++)
    {
        std::cout << *it << '\n';
    }
    cout << endl;

    // cout << "Loop in reverse order 4 to 0" << endl;
    // for (auto it = buffer.rbegin(); it != buffer.rend(); it++)
    // {
    //     std::cout << *it << '\n';
    // }
    // cout << endl;

    // Should not overflow
    cout << buffer.Pop() << endl;
    buffer.Push(5);

    return new SampleApp();
}
