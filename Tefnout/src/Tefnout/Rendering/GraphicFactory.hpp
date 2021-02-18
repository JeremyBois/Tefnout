#ifndef __GRAPHICFACTORY__HPP
#define __GRAPHICFACTORY__HPP

#include "Tefnout/Core/Core.hpp"
#include "Tefnout/Rendering/IGraphicContext.hpp"


// @UNSURE
// Move this to rendering factory file ?
// @UNSURE



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
