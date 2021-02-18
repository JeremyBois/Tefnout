#ifndef __IVERTEXARRAY__HPP
#define __IVERTEXARRAY__HPP

#include "Tefnout/Rendering/IIndexBuffer.hpp"
#include "Tefnout/Rendering/IVertexBuffer.hpp"
#include <memory>
namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Interface for an array (a container used to bind together IIndexBuffer and
 *             IVertexBuffer) that provide minimal behavior that must implement a concrete
 *             class.
 */
class TEFNOUT_API IVertexArray
{
  public:
    virtual ~IVertexArray() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    virtual const std::vector<std::shared_ptr<IVertexBuffer>> &GetVertexBuffers() const = 0;
    virtual const std::shared_ptr<IIndexBuffer> &GetIndexBuffer() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer> &vertexBuffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer> &indexBuffer) = 0;
};
} // namespace Rendering
} // namespace Tefnout

#endif
