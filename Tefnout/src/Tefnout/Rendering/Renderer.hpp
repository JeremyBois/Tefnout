#ifndef __RENDERER__HPP
#define __RENDERER__HPP

#include "Tefnout/Rendering/Camera/ICamera.hpp"
#include "Tefnout/Rendering/IGraphicRenderer.hpp"
#include "Tefnout/Rendering/IShader.hpp"
#include "Tefnout/Rendering/IVertexArray.hpp"

#include <glm/glm.hpp>
#include <memory>

// @TODO
// Use a service manager in place of Static renderer ??
// @TODO

namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Static renderer that can be used to draw to the screen using a specific
 *             graphic API.
 * @see        IGraphicAPI for more information about actual rendering implementation.
 */
class TEFNOUT_API Renderer
{
  public:
    Renderer(Renderer const &) = delete;
    void operator=(Renderer const &) = delete;

    // Primitives
    static void Begin(const std::shared_ptr<Camera::ICamera> &camera);
    static void Push(const std::shared_ptr<IShader> &shader,
                     const std::shared_ptr<IVertexArray> &vertexArray);
    static void End();

    static API GetAPI()
    {
        return IGraphicRenderer::GetAPI();
    }

    // Commands
    static void Init()
    {
        s_graphicRenderer->Init();
    };
    static void Clear()
    {
        s_graphicRenderer->Clear();
    };
    static void ClearColor(const glm::vec4 &color)
    {
        s_graphicRenderer->ClearColor(color);
    };
    static void Draw(const std::shared_ptr<IVertexArray> &vertexArray)
    {
        s_graphicRenderer->Draw(vertexArray);
    };

    // Window callbacks
    // static void OnWindowResize(uint32_t width, uint32_t height);

  private:
    // @TODO Move this to proper container when it becomes bigger
    static constexpr glm::mat4 s_defaultProjectionView = glm::mat4 {1.0f};
    static glm::mat4 s_projectionView;

    static std::unique_ptr<IGraphicRenderer> s_graphicRenderer;

    Renderer();
};
} // namespace Rendering
} // namespace Tefnout

#endif
