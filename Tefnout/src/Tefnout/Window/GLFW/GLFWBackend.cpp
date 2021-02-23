#include "GLFWBackend.hpp"

#include "Tefnout/Core/Logger.hpp"

#include "Tefnout/Event/KeyEvent.hpp"
#include "Tefnout/Event/MouseEvent.hpp"
#include "Tefnout/Event/WindowEvent.hpp"

#include "Tefnout/Input/KeyCode.hpp"
#include "Tefnout/Input/MouseCode.hpp"

#include "Tefnout/Rendering/GraphicFactory.hpp"
#include "Tefnout/Window/IWindow.hpp"
#include <memory>

// https://www.glfw.org/docs/latest/quick_guide.html#quick_include
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Tefnout
{
namespace Window
{

// At start there is no window created ...
uint8_t GLFWBackend::s_instanceCount = 0;

GLFWBackend::GLFWBackend(const GenericProperties &properties)
    : m_pGlfwWindow(nullptr), m_information(Description{}), m_openGlHints(OpenGlHints{}),
      m_pContext(nullptr)
{
    Init(properties);
}

GLFWBackend::~GLFWBackend()
{
    Clear();
}

void GLFWBackend::SetEventCallback(
    const std::function<void(std::shared_ptr<Event::IEvent>)> &callback)
{
    m_information.Callback = callback;
}

void *GLFWBackend::GetImplementation() const
{
    return m_pGlfwWindow;
}

void GLFWBackend::OnRender()
{
    m_pContext->OnRender();
}

void GLFWBackend::OnUpdate()
{
    m_pContext->OnUpdate();

    // Event pool (keyboard, mouse, ...)
    glfwPollEvents();
}

void GLFWBackend::Init(const GenericProperties &properties)
{
    // Store properties in exposed information
    m_information.Title = properties.Title;
    m_information.Height = properties.Height;
    m_information.Width = properties.Width;

    TEFNOUT_INFO("Creating a new GLFW window (Title={0} - Width={1} - Height={2})",
                 m_information.Title, m_information.Width, m_information.Height);

    if (s_instanceCount == 0)
    {
        // Init should be called first
        if (!glfwInit())
        {
            TEFNOUT_CRITICAL("Cannot initialize GLFW. Required");
        }

        // @TODO Should be done only for OpenGL
        // Specify we use OpenGL 4.6 core version
        // http://www.glfw.org/docs/latest/window.html#window_hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_openGlHints.Major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_openGlHints.Minor);
    }

    // Create the new window
    m_pGlfwWindow = glfwCreateWindow(m_information.Width, m_information.Height,
                                     m_information.Title.c_str(), nullptr, nullptr);

    if (nullptr == m_pGlfwWindow)
    {
        TEFNOUT_CRITICAL("Cannot create GLFW window. Required");
        return;
    }

    glfwSetInputMode(m_pGlfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Make our window data available in callbacks
    glfwSetWindowUserPointer(m_pGlfwWindow, &m_information);

    m_pContext = Tefnout::Rendering::Create(m_pGlfwWindow);
    m_pContext->Init(properties);

    // Setup all callbacks
    SetupCallbacks();
}

void GLFWBackend::SetupCallbacks()
{
    // Dispatch window events from GLFW to Engine
    glfwSetWindowCloseCallback(m_pGlfwWindow, [](GLFWwindow *window) {
        // Get registered structure using glfwSetWindowUserPointer
        Description &data = *static_cast<Description *>(glfwGetWindowUserPointer(window));

        data.Callback(std::make_shared<Event::WindowClosed>());
    });

    glfwSetWindowSizeCallback(m_pGlfwWindow, [](GLFWwindow *window, int width, int height) {
        Description &data = *static_cast<Description *>(glfwGetWindowUserPointer(window));

        // First update our information
        data.Height = height;
        data.Width = width;

        data.Callback(std::make_shared<Event::WindowResized>(data.Width, data.Height));
    });

    glfwSetWindowFocusCallback(m_pGlfwWindow, [](GLFWwindow *window, int focused) {
        Description &data = *static_cast<Description *>(glfwGetWindowUserPointer(window));

        // Only one window for now so just update our window description
        data.Focused = focused;
    });

    //     glfwSetCursorEnterCallback

    // Dispatch key event from GLFW to Engine
    glfwSetKeyCallback(
        m_pGlfwWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            Description &data = *static_cast<Description *>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS: {
                data.Callback(std::make_shared<Event::KeyPressed>(Input::KeycodeFromGLFW(key)));
                break;
            }
            case GLFW_RELEASE: {
                data.Callback(std::make_shared<Event::KeyReleased>(Input::KeycodeFromGLFW(key)));
                break;
            }
            case GLFW_REPEAT: {
                data.Callback(std::make_shared<Event::KeyHeld>(Input::KeycodeFromGLFW(key)));
                break;
            }
            }
        });

    // Dispatch mouse events from GLFW to Engine
    glfwSetCursorPosCallback(m_pGlfwWindow, [](GLFWwindow *window, double xpos, double ypos) {
        Description &data = *static_cast<Description *>(glfwGetWindowUserPointer(window));

        data.Callback(std::make_shared<Event::MouseMoved>(Event::MousePosition(xpos, ypos)));
    });

    glfwSetMouseButtonCallback(
        m_pGlfwWindow, [](GLFWwindow *window, int button, int action, int mods) {
            Description &data = *static_cast<Description *>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS: {
                data.Callback(
                    std::make_shared<Event::MouseButtonPressed>(Input::MousecodeFromGLFW(button)));
                break;
            }
            case GLFW_RELEASE: {
                data.Callback(
                    std::make_shared<Event::MouseButtonReleased>(Input::MousecodeFromGLFW(button)));
                break;
            }
            }
        });

    glfwSetScrollCallback(m_pGlfwWindow, [](GLFWwindow *window, double xoffset, double yoffset) {
        Description &data = *static_cast<Description *>(glfwGetWindowUserPointer(window));

        data.Callback(std::make_shared<Event::MouseScrolled>(Event::MouseOffset(xoffset, yoffset)));
    });
}

void GLFWBackend::Clear()
{
    glfwDestroyWindow(m_pGlfwWindow);
    --s_instanceCount;

    if (s_instanceCount < 1)
    {
        // Only call it for the last window
        glfwTerminate();
    }
}

} // namespace Window
} // namespace Tefnout
