#ifndef __OPENGLVERTEXARRAY__HPP
#define __OPENGLVERTEXARRAY__HPP

#include "Tefnout/Core/Platform.hpp"
#include "Tefnout/Rendering/IVertexArray.hpp"
#include <memory>
#include <vector>

namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Concrete implementation of API agnostic IVertexArray for the OpenGL specification.
 */
class TEFNOUT_LOCAL OpenGlVertexArray : public IVertexArray
{
  public:
    OpenGlVertexArray();
    virtual ~OpenGlVertexArray();

    // Disable copy
    OpenGlVertexArray &operator=(OpenGlVertexArray other) = delete;
    OpenGlVertexArray(const OpenGlVertexArray &other) = delete;

    void Bind() const override;
    void UnBind() const override;

    const std::vector<std::shared_ptr<IVertexBuffer>> &GetVertexBuffers() const override {return m_vertexBuffers;}
    const std::shared_ptr<IIndexBuffer> &GetIndexBuffer() const override {return m_indexBuffer;}

    void AddVertexBuffer(const std::shared_ptr<IVertexBuffer> &vertexBuffer) override;
    void SetIndexBuffer(const std::shared_ptr<IIndexBuffer> &indexBuffer) override;

  private:
    uint32_t m_id;
    uint32_t m_vertexAttributeIndex;
    std::vector<std::shared_ptr<IVertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IIndexBuffer> m_indexBuffer;
};
} // namespace Rendering
} // namespace Tefnout

#endif
