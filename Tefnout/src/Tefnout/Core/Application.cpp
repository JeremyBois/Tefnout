#include "Application.hpp"

#include "Tefnout/Core/ApplicationLayer.hpp"
#include "Tefnout/Event/Event.hpp"
#include "Tefnout/Rendering/Renderer.hpp"
#include "Tefnout/Window/IWindow.hpp"
#include "Tefnout/Window/WindowFactory.hpp"
#include <memory>

namespace Tefnout
{

Application *Application::s_Instance = nullptr;

Application::Application()
    : m_running(false), m_pWindow(Window::Create(Window::GenericProperties{})), m_windowEvents(),
      m_layersStack()
{
    s_Instance = this;

    using namespace std::placeholders;
    m_pWindow->SetEventCallback(std::bind(&Application::OnWindowEvent, this, _1));
    m_running = m_pWindow != nullptr ? true : false;

    // Init Renderer and associated concrete API
    Rendering::Renderer::Init();
}
Application::~Application()
{
}

void Application::Run()
{
    while (m_running)
    {
        // Update
        for (auto& layer : m_layersStack)
        {
            layer->OnUpdate();
        }
        m_pWindow->OnUpdate();

        // Rendering
        for (auto& layer : m_layersStack)
        {
            layer->OnRender();
        }
        m_pWindow->OnRender();

        // Events
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
    m_windowEvents.Push(std::move(event)); // Avoid temporary copy
}

void Application::HandleEvents()
{
    while (m_windowEvents.IsPending())
    {
        auto event = m_windowEvents.Pop();
        Event::Information eventInformation = event->GetInformation();

        // TEFNOUT_DEBUG("Handle {0}", *event);

        // First let application handle event
        switch (eventInformation.Type)
        {
        case Event::Kind::WindowClosed: {
            Close();
            event->MarkAsHandled();
            break;
        }
        default:
            break;
        }

        // Iterate from last to first to let last layer handle event first
        for (auto it = m_layersStack.rbegin(); it != m_layersStack.rend(); ++it)
        {
            if (eventInformation.Status == Event::State::Pending)
            {
                (*it)->OnEvent(event);
            }
            else
            {
                break;
            }
        }
    }
}

} // namespace Tefnout
