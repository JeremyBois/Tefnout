#include "Tefnout/Rendering/RenderingFactory.hpp"
#include "TefnoutPCH.hpp"

#include "Renderer.hpp"

namespace Tefnout
{
namespace Rendering
{
// Initial value for the graphic rendering API
std::unique_ptr<IGraphicRenderer> Renderer::s_graphicRenderer = CreateGraphicRenderer();

// Default to do nothing matrix
glm::mat4 Renderer::s_projectionView = Renderer::s_defaultProjectionView;

void Renderer::Begin(const std::shared_ptr<Camera::ICamera> &camera)
{
    s_projectionView = camera->GetProjectionViewMatrix();
}

void Renderer::Push(const std::shared_ptr<IShader> &shader,
                    const std::shared_ptr<IVertexArray> &vertexArray)
{
    shader->Bind();
    shader->SetMat4("uProjectionView", s_projectionView);

    vertexArray->Bind();
    Renderer::Draw(vertexArray);
}

void Renderer::End()
{
    s_projectionView = s_defaultProjectionView;
}

} // namespace Rendering
} // namespace Tefnout
