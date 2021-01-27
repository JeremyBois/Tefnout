#include "Tefnout.hpp"

#include "Test.hpp"

class SampleApp : public Tefnout::Application
{
public:
    SampleApp() = default;
    ~SampleApp() = default;
};

Tefnout::Application* Tefnout::CreateApplication()
{
    // @TODO Create a separate Test target ??
    ::Test::RingBuffer_TEST();

    return new SampleApp();
}


