#ifndef __OPENGLINDEXBUFFER__HPP
#define __OPENGLINDEXBUFFER__HPP

#include "Tefnout/Core/Platform.hpp"

#include "Tefnout/Rendering/IIndexBuffer.hpp"


namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_LOCAL OpenGlIndexBuffer : public IIndexBuffer
{
  public:
    OpenGlIndexBuffer(uint32_t *indexes, uint32_t count);
    virtual ~OpenGlIndexBuffer();

    void Bind() const override;
    void UnBind() const override;
    inline uint32_t GetCount() const override {return m_count;};

    // void SetData(uint32_t *indexes, uint32_t count) override;

  private:
    uint32_t m_id;
    uint32_t m_count;
};
} // namespace Rendering
} // namespace Tefnout

#endif
