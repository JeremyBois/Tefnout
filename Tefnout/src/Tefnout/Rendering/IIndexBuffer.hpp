#ifndef __IINDEXBUFFER__HPP
#define __IINDEXBUFFER__HPP

#include "Tefnout/Core/Platform.hpp"
#include <stdint.h>

namespace Tefnout
{
namespace Rendering
{
class TEFNOUT_API IIndexBuffer
{
  public:
    virtual ~IIndexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
    virtual uint32_t GetCount() const = 0;

    // virtual void SetData(uint32_t *indexes, uint32_t count) = 0;
};
} // namespace Rendering
} // namespace Tefnout

#endif
