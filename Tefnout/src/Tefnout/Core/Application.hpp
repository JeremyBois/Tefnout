#ifndef __APPLICATION__HPP
#define __APPLICATION__HPP

#include "Core.hpp"
#include "Tefnout/Container/RingBuffer.hpp"
#include "Tefnout/Container/Stack.hpp"
#include "Tefnout/Core/ApplicationLayer.hpp"
#include "Tefnout/Event/Event.hpp"
#include "Tefnout/Window/IWindow.hpp"

#include <memory>

namespace Tefnout
{
/**
 * @brief      Engine manager class.
 */
class TEFNOUT_API Application
{
  public:
    Application();
    virtual ~Application();

    inline Window::IWindow &GetWindow()
    {
        return *m_pWindow;
    }

    inline Stack<ApplicationLayer> &GetLayerStack()
    {
        return m_layersStack;
    }

    void Run();
    void Close();
    void OnWindowEvent(std::shared_ptr<Event::IEvent> event);

  private:
    bool m_running;
    std::unique_ptr<Window::IWindow> m_pWindow;
    Buffer::Ring<std::shared_ptr<Event::IEvent>, 50> m_windowEvents;
    Stack<ApplicationLayer> m_layersStack;

    static Application *s_Instance;

    void HandleEvents();
};

Tefnout::Application *CreateApplication();

} // namespace Tefnout

#endif
