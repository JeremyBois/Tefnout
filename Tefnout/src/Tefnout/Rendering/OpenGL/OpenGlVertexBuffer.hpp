#ifndef __OPENGLVERTEXBUFFER__HPP
#define __OPENGLVERTEXBUFFER__HPP

#include "Tefnout/Core/Platform.hpp"
#include "Tefnout/Rendering/IVertexBuffer.hpp"

namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_LOCAL OpenGlVertexBuffer : public IVertexBuffer
{
  public:
    OpenGlVertexBuffer(float *data, uint32_t dataSize);
    virtual ~OpenGlVertexBuffer();

    // IVertexBuffer inteface
    void Bind() const override;
    void UnBind() const override;
    // void SetData(float *data, uint32_t dataSize) override;

  private:
    uint32_t m_id;
};
} // namespace Rendering
} // namespace Tefnout

#endif
