#ifndef __IGRAPHICCONTEXT__HPP
#define __IGRAPHICCONTEXT__HPP

#include "Tefnout/Core/CorePCH.hpp"
#include "Tefnout/Window/IWindow.hpp"


namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_LOCAL IGraphicContext
{
  public:
    virtual ~IGraphicContext() = default;

    virtual void Init(Window::GenericProperties window_properties) = 0;
    virtual void Clear() = 0;
    virtual void OnUpdate(){};
    virtual void OnRender() = 0;
};
} // namespace Rendering
} // namespace Tefnout

#endif
