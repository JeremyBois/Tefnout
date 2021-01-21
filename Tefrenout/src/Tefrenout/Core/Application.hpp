#ifndef __APPLICATION__HPP
#define __APPLICATION__HPP

#include "Tefrenout_macro.hpp" // Needed for macro definition

namespace Tefrenout
{
class TEFRENOUT_API Application
{
  public:
    Application() = default;
    virtual ~Application() = default;

    void Run();
};

Tefrenout::Application *CreateApplication();

} // namespace Tefrenout

#endif
