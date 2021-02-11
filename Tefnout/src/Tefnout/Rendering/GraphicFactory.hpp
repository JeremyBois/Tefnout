#ifndef __GRAPHICFACTORY__HPP
#define __GRAPHICFACTORY__HPP

#include "TefnoutPCH.hpp"

#include "Tefnout/Rendering/IGraphicContext.hpp"

// Avoid public include of "GLFW/glfw3.h"
struct GLFWwindow;

namespace Tefnout
{
namespace Rendering
{
    TEFNOUT_LOCAL std::unique_ptr<IGraphicContext> Create(GLFWwindow *pWindow);
}

} // namespace Tefnout

#endif
