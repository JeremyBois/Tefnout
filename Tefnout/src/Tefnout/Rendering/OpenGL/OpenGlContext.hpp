#ifndef __OPENGLCONTEXT__HPP
#define __OPENGLCONTEXT__HPP

#include "Tefnout/Rendering/Camera/ICamera.hpp"
#include "Tefnout/Rendering/IVertexArray.hpp"
#include "Tefnout/Rendering/IGraphicContext.hpp"
#include "Tefnout/Rendering/IIndexBuffer.hpp"
#include "Tefnout/Rendering/IVertexBuffer.hpp"
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

    // Disable copy
    OpenGlContext &operator=(OpenGlContext other) = delete;
    OpenGlContext(const OpenGlContext &other) = delete;
    // Disable move
    OpenGlContext &operator=(OpenGlContext &&other) = delete;
    OpenGlContext(OpenGlContext &&other) = delete;

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
