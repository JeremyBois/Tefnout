#include "Application.hpp"

#include "Tefnout/Event/Event.hpp"
#include "Tefnout/Window/IWindow.hpp"
#include "Tefnout/Window/WindowFactory.hpp"

namespace Tefnout
{

Application *Application::s_Instance = nullptr;

Application::Application()
    : m_running(false), m_pWindow(Window::Create(Window::GenericProperties{}))
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
    };
}

void Application::Close()
{
    m_running = false;
}

void Application::OnWindowEvent(Event::IEvent &event)
{
    TEFNOUT_TRACE("Get {0}", event);

    Event::Information eventInformation = event.GetInformation();

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

} // namespace Tefnout
