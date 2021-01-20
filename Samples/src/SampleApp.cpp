#include <Tefranout.hpp>


class SampleApp : public Tefranout::Application
{
public:
    SampleApp() {};
    ~SampleApp() {};
};

Tefranout::Application* Tefranout::CreateApplication()
{
    return new SampleApp();
}
