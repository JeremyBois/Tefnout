#include "Tefnout.hpp"


class SampleApp : public Tefnout::Application
{
public:
    SampleApp() = default;
    ~SampleApp() = default;
};

Tefnout::Application* Tefnout::CreateApplication()
{
    return new SampleApp();
}
