#ifndef __ICAMERA__HPP
#define __ICAMERA__HPP

#include "Tefnout/Core/Platform.hpp"
#include <glm/fwd.hpp>

// @TODO
// Add a flag to avoid recomputing matrix each time.
// @TODO

namespace Tefnout
{
namespace Rendering
{
namespace Camera
{
/**
 * @brief      Interface describing what is a minimal implementation of a concrete
 *             Camera looks like.
 * @note       This class is and should remain platform agnostic.
 */
class TEFNOUT_API ICamera
{
public:
    virtual ~ICamera() = default;

    virtual glm::mat4 GetViewMatrix() const = 0;
    virtual glm::mat4 GetprojectionMatrix() const = 0;
    virtual glm::mat4 GetProjectionViewMatrix() const = 0;
};
} // namespace Camera
} // namespace Rendering
} // namespace Tefnout

#endif
