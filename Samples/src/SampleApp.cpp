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
    buffer.Push(0);
    // buffer.Push(1);
    // buffer.Push(2);
    // buffer.Push(3);
    // buffer.Push(4);
    // Should overflow
    // buffer.Push(5);

    cout << buffer.Front() << endl;
    cout << buffer.Back() << endl;
    // cout << buffer.Pop() << endl;
    cout << endl;

    cout << (*buffer.begin()) << endl;
    cout << (*buffer.end()) << endl;
    cout << ((*buffer.begin()) == (*buffer.end())) << endl;
    cout << endl;

    // Should not overflow
    // buffer.Push(5);

    for (auto i : buffer)
        cout << "for: " << i << "\n" << endl;

    return new SampleApp();
}
