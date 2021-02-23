#ifndef __OPENGLGRAPHICRENDERER__HPP
#define __OPENGLGRAPHICRENDERER__HPP

#include "Tefnout/Rendering/IGraphicRenderer.hpp"
namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_LOCAL OpenGlGraphicRenderer : public IGraphicRenderer
{
  public:
    OpenGlGraphicRenderer();
    virtual ~OpenGlGraphicRenderer();

    // IGraphicRenderer interface
    void Init() override;
    void Clear() override;
    void ClearColor(const glm::vec4 &clearColor) override;
    void Draw(const std::shared_ptr<IVertexArray> &vertexArray) override;
};
} // namespace Rendering
} // namespace Tefnout

#endif
