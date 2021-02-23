#include "TefnoutPCH.hpp"

#include "OpenGlGraphicRenderer.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Tefnout
{
namespace Rendering
{

OpenGlGraphicRenderer::OpenGlGraphicRenderer()
{
}

OpenGlGraphicRenderer::~OpenGlGraphicRenderer()
{
}

void OpenGlGraphicRenderer::Init()
{
}

void OpenGlGraphicRenderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGlGraphicRenderer::ClearColor(const glm::vec4 &clearColor)
{
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void OpenGlGraphicRenderer::Draw(const std::shared_ptr<IVertexArray> &vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT,
                   nullptr);
}

} // namespace Rendering
} // namespace Tefnout
