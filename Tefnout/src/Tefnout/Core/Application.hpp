#ifndef __APPLICATION__HPP
#define __APPLICATION__HPP

#include "Platform.hpp"

namespace Tefnout
{
class TEFNOUT_API Application
{
  public:
    Application() = default;
    virtual ~Application() = default;

    void Run();
};

Tefnout::Application *CreateApplication();

} // namespace Tefnout

#endif
