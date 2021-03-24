#ifndef __ORTHOGRAPHICCAMERA__HPP
#define __ORTHOGRAPHICCAMERA__HPP

#include "ICamera.hpp"

#include "Tefnout/Core/Platform.hpp"
#include "glm/glm.hpp"

namespace Tefnout
{
namespace Rendering
{

namespace Camera
{
/**
 * @brief      Container for orthographic camera frustrum boundaries.
 */
struct TEFNOUT_API FrustrumBounds
{
    const float Left;
    const float Right;
    const float Bottom;
    const float Top;

    explicit FrustrumBounds(float left, float right, float bottom, float top)
        : Left(left), Right(right), Bottom(bottom), Top(top)
    {
    }
};

/**
 * @brief      Container for clipping near and far distances.
 */
struct TEFNOUT_API ClippingLimits
{
    const float Near;
    const float Far;

    ClippingLimits(float near, float far) : Near(near), Far(far)
    {
    }
};

/**
 * @brief      An orthographic camera maps 3D coordinates directly to the 2D screen
 *             without accounting for perspective.
 */
class TEFNOUT_API Orthographic : public ICamera
{
  public:
    static constexpr glm::vec3 s_Zaxis = {0.0f, 0.0f, 1.0f};

    Orthographic(const FrustrumBounds bounds, const ClippingLimits clippingLimits);
    /**
     * @brief      Constructs a Orthographic camera with clipping planes defined as below
     *              - Near is one unit behind
     *              - Far is one unit ahead
     *
     * @param[in]  bounds  Camera
     */
    Orthographic(const FrustrumBounds bounds);

    virtual ~Orthographic() = default;

    // Getters
    inline glm::mat4 GetViewMatrix() const override
    {
        return m_view;
    }
    inline glm::mat4 GetprojectionMatrix() const override
    {
        return m_projection;
    }
    inline glm::mat4 GetProjectionViewMatrix() const override
    {
        return m_projectionView;
    }

    // Setters
    void SetPosition(glm::vec3 position);
    void SetRotationZ(float degrees);

  private:
    FrustrumBounds m_bounds;
    ClippingLimits m_clipping;

    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_projectionView;

    float m_rotationZ;
    glm::vec3 m_position;

    void UpdateMatrices();
};

} // namespace Camera
} // namespace Rendering
} // namespace Tefnout

#endif
