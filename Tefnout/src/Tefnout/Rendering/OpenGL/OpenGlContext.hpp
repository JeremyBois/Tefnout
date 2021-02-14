#ifndef __OPENGLCONTEXT__HPP
#define __OPENGLCONTEXT__HPP

#include "Tefnout/Rendering/IGraphicContext.hpp"
#include "Tefnout/Window/IWindow.hpp"

// @TEMP
#include "Tefnout/Rendering/IShader.hpp"
//


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

     // @TEMP
    uint32_t m_vertexArray;
    uint32_t m_vertexBuffer;
    uint32_t m_indexBuffer;
    std::unique_ptr<Rendering::IShader> m_pShader;
    //

    void SetupCallbacks();
};
} // namespace Rendering
} // namespace Tefnout
#endif
