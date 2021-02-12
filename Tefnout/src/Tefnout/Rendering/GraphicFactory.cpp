#include "GraphicFactory.hpp"

#if defined _WIN32 || defined __GNUC__
#include "Tefnout/Rendering/OpenGL/OpenGlContext.hpp"
#endif

#include <GLFW/glfw3.h>

namespace Tefnout
{
namespace Rendering
{

std::unique_ptr<IGraphicContext> Create(GLFWwindow *pWindow)
{
#if defined _WIN32 || defined __GNUC__
    return std::make_unique<OpenGlContext>(pWindow);
#else
    TEFNOUT_ERROR("Rendering not supported on this platform.");
    return nullptr;
#endif
}
} // namespace Rendering
} // namespace Tefnout
