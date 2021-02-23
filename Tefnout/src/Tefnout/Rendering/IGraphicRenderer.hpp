#ifndef __IGRAPHICAPI__HPP
#define __IGRAPHICAPI__HPP

#include "Tefnout/Rendering/IVertexArray.hpp"

#include <glm/fwd.hpp>
#include <memory>

namespace Tefnout
{
namespace Rendering
{

/**
 * @brief      An enum that represent all Graphic API allowed in the engine.
 */
enum class TEFNOUT_API API : uint8_t
{
    None = 0, // Not yet implemented
    OpenGl = 1
};

/**
 * @brief      Interface describing what is a minimal implementation of a concrete
 *             Rendering API looks like.
 * @note       This class is and should remain platform agnostic.
 */
class TEFNOUT_API IGraphicRenderer
{
  public:
    virtual ~IGraphicRenderer() = default;

    virtual void Init() = 0;
    virtual void Clear() = 0;
    virtual void ClearColor(const glm::vec4 &clearColor) = 0;

    virtual void Draw(const std::shared_ptr<IVertexArray> &vertexArray) = 0;

    static API GetAPI()
    {
        return s_API;
    };

  private:
    // Hard coded for now
    static const API s_API = API::OpenGl;
};
} // namespace Rendering
} // namespace Tefnout
#endif
