#ifndef __IVERTEXBUFFER__HPP
#define __IVERTEXBUFFER__HPP

#include "Tefnout/Core/Platform.hpp"
#include "Tefnout/Rendering/Layout.hpp"
#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <vector>

namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Interface describing what is a minimal implementation of a concrete Vertex
 *             buffer looks like.
 * @note       This class is and should remain platform agnostic.
 */
class TEFNOUT_API IVertexBuffer
{
  public:
    virtual ~IVertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual const Layout &GetLayout() const = 0;
    virtual bool HasLayout() const = 0;
    virtual void SetLayout(const Layout& layout) = 0;

    virtual void UpdateData(float *data, uint32_t dataSize) = 0;
};
} // namespace Rendering
} // namespace Tefnout

#endif
