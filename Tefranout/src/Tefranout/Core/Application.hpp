#ifndef __APPLICATION__HPP
#define __APPLICATION__HPP

#include "Tefranout_macro.hpp" // Needed for macro definition

// main entry declaration
// int main(int argc, char **argv);

namespace Tefranout
{
class TEFRANOUT_API Application
{
  public:
    Application() = default;
    virtual ~Application() = default;

    void Run();

    // private:
    //   friend int ::main(int argc, char **argv);
};

Tefranout::Application *CreateApplication();

} // namespace Tefranout

#endif
