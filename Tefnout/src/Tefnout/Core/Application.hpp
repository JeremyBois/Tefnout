#ifndef __APPLICATION__HPP
#define __APPLICATION__HPP

#include "Platform.hpp"
#include "Tefnout/Event/Event.hpp"
#include "Tefnout/Window/IWindow.hpp"

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

    void Run();
    void Close();
    void OnWindowEvent(Event::IEvent &event);

  private:

    bool m_running;
    std::unique_ptr<Window::IWindow> m_pWindow;

    static Application *s_Instance;
};

Tefnout::Application *CreateApplication();

} // namespace Tefnout

#endif
