#include "Application.hpp"

#include "Tefnout/Event/Event.hpp"
#include "Tefnout/Window/IWindow.hpp"
#include "Tefnout/Window/WindowFactory.hpp"
#include <memory>

namespace Tefnout
{

Application *Application::s_Instance = nullptr;

Application::Application()
    : m_running(false), m_pWindow(Window::Create(Window::GenericProperties{})), m_windowEvents()
{
    s_Instance = this;

    using namespace std::placeholders;
    m_pWindow->SetEventCallback(std::bind(&Application::OnWindowEvent, this, _1));
    m_running = m_pWindow != nullptr ? true : false;
}
Application::~Application()
{
}

void Application::Run()
{
    while (m_running)
    {
        m_pWindow->OnUpdate();
        m_pWindow->OnRender();

        HandleEvents();
    };
}

void Application::Close()
{
    m_running = false;
}

void Application::OnWindowEvent(std::shared_ptr<Event::IEvent> event)
{
    // TEFNOUT_DEBUG("Get {0}", *event);

    // Delay event handling
    m_windowEvents.Push(std::move(event));  // Avoid temporary copy
}

void Application::HandleEvents()
{
    while (m_windowEvents.IsPending())
    {
        auto event = m_windowEvents.Pop();
        Event::Information eventInformation = event->GetInformation();

        // TEFNOUT_DEBUG("Handle {0}", *event);

        switch (eventInformation.Type)
        {
        case Event::Kind::WindowClosed: {
            Close();
            break;
        }
        default:
            break;
        }
    }
}

} // namespace Tefnout
