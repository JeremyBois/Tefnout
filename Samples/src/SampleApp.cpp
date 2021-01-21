#include <Tefrenout.hpp>


class SampleApp : public Tefrenout::Application
{
public:
    SampleApp() = default;
    ~SampleApp() = default;
};

Tefrenout::Application* Tefrenout::CreateApplication()
{
    return new SampleApp();
}
