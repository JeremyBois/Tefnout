#include "OpenGlContext.hpp"
#include "Tefnout/Core/Logger.hpp"
#include "Tefnout/Window/IWindow.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <sstream>

namespace Tefnout
{
namespace Rendering
{

OpenGlContext::OpenGlContext(GLFWwindow *pWindow) : m_pGlfwWindow(pWindow)
{
    if (nullptr == pWindow)
    {
        TEFNOUT_CRITICAL("Window handle is not defined.");
        assert(nullptr != pWindow);
    }
}

OpenGlContext::~OpenGlContext()
{
    Clear();
}

void OpenGlContext::Init(Window::GenericProperties window_properties)
{
    // Move this to a specific OPENGL context initializer
    glfwMakeContextCurrent(m_pGlfwWindow);

    // GLAD: Load all OpenGL function pointers
    // We pass GLAD the function to load the adress of the OpenGL function pointers which is
    // OS-specific glfwGetProcAddress find the correct one based on the OS
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        TEFNOUT_CRITICAL("Failed to initialize GLAD. Required for OpenGL context");
        return;
    }

    // Get basic information about used OpenGL
    TEFNOUT_INFO("OpenGL information: \n\t Vendor - {0}\n\t Renderer - {1}\n\t Version {2}\n\t "
                 "Shading Language {3}",
                 glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION),
                 glGetString(GL_SHADING_LANGUAGE_VERSION));

    std::stringstream ss;
    ss << glGetStringi(GL_EXTENSIONS, 0);
    for (int i = 1; i < GL_NUM_EXTENSIONS; ++i)
    {
        ss << " - " << glGetStringi(GL_EXTENSIONS, i);
    }
    TEFNOUT_TRACE("OpenGL extensions:\n\t {0}", ss.str());

    // Vsync enable
    glfwSwapInterval(1);

    // Define the drawing area based on the window coordinates
    // y start at the bottom of the viewport
    // Note that processed coordinates in OpenGL are between -1 and 1
    // so we effectively map from the range (-1 to 1) to (0, m_width) and (0, m_height).
    glViewport(0, 0, window_properties.Width, window_properties.Height);

    // Enable culling and set it to front mode.
    // Vertices must be defines in counter-clockwise order for Backface culling
    // to work properly.
    // https://stackoverflow.com/questions/8142388/in-what-order-should-i-send-my-vertices-to-opengl-for-culling#8142461
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Add depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    SetupCallbacks();
}

void OpenGlContext::SetupCallbacks()
{
    // Adjust drawing area based on window area on each resize
    glfwSetFramebufferSizeCallback(m_pGlfwWindow,
                                   [](GLFWwindow *pWindow, int width, int height) // NOLINT
                                   {
                                       glViewport(0, 0, width, height);
                                       TEFNOUT_TRACE("Framebuffer resized, viewport updated.");
                                   });
}

void OpenGlContext::Clear()
{
}

void OpenGlContext::OnUpdate()
{
}

void OpenGlContext::OnRender()
{
    glClearColor(0.149f, 0.545f, 0.823f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap buffer
    glfwSwapBuffers(m_pGlfwWindow);
}

} // namespace Rendering
} // namespace Tefnout
