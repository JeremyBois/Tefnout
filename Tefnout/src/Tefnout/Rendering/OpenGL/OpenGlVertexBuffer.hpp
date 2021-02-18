#ifndef __OPENGLVERTEXBUFFER__HPP
#define __OPENGLVERTEXBUFFER__HPP

#include "Tefnout/Core/Platform.hpp"
#include "Tefnout/Rendering/IVertexBuffer.hpp"
#include "Tefnout/Rendering/Layout.hpp"

namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_LOCAL OpenGlVertexBuffer : public IVertexBuffer
{
  public:
    OpenGlVertexBuffer(float *data, uint32_t dataSize);
    virtual ~OpenGlVertexBuffer();

    // Disable copy
    OpenGlVertexBuffer &operator=(OpenGlVertexBuffer other) = delete;
    OpenGlVertexBuffer(const OpenGlVertexBuffer &other) = delete;

    // IVertexBuffer inteface
    void Bind() const override;
    void UnBind() const override;
    inline const Layout& GetLayout() const override { return m_layout; }
    inline bool HasLayout() const override {return m_layout.GetStrideSize() != 0;}
    inline void SetLayout(const Layout& layout) override { m_layout = layout; }

    void UpdateData(float *data, uint32_t dataSize) override;

  private:
    uint32_t m_id;
    Layout m_layout;

};
} // namespace Rendering
} // namespace Tefnout

#endif
