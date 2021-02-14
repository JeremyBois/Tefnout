#ifndef __IVERTEXBUFFER__HPP
#define __IVERTEXBUFFER__HPP

#include "Tefnout/Core/Platform.hpp"
#include <stdint.h>

namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_API IVertexBuffer
{
  public:
    virtual ~IVertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    // virtual void SetData(float *data, uint32_t dataSize) = 0;
};
} // namespace Rendering
} // namespace Tefnout

#endif
