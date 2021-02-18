#ifndef __IWINDOW__HPP
#define __IWINDOW__HPP

#include "Tefnout/Core/Core.hpp"
#include "Tefnout/Event/Event.hpp"
#include <functional>


namespace Tefnout
{
namespace Window
{
struct TEFNOUT_LOCAL GenericProperties
{
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    explicit GenericProperties(std::string title = "Tefnout Engine", uint32_t width = 1280,
                               uint32_t height = 720)
        : Title(title), Width(width), Height(height)
    {
    }
};
class TEFNOUT_API IWindow
{
  public:
    virtual ~IWindow() = default;

    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;

    virtual void SetEventCallback(const std::function<void(Event::IEvent &)> &callback) = 0;
    virtual void *GetImplementation() const = 0;
};
} // namespace Window
} // namespace Tefnout
#endif
