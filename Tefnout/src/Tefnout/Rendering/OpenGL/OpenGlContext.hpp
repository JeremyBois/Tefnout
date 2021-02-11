#ifndef __OPENGLCONTEXT__HPP
#define __OPENGLCONTEXT__HPP

#include "TefnoutPCH.hpp"

#include "Tefnout/Rendering/IGraphicContext.hpp"
#include "Tefnout/Window/IWindow.hpp"

struct GLFWwindow;

namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_LOCAL OpenGlContext : public IGraphicContext
{
  public:
    OpenGlContext(GLFWwindow* pWindow);
    virtual ~OpenGlContext();

    // IGraphicContex interface
    virtual void Init(Window::GenericProperties window_properties) override;
    virtual void Clear() override;
    virtual void OnUpdate() override;
    virtual void OnRender() override;

private:
    GLFWwindow *m_pGlfwWindow;

    void SetupCallbacks();
};
} // namespace Rendering
} // namespace Tefnout
#endif
