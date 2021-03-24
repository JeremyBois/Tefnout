#include "TefnoutPCH.hpp"

#include "OrthographicCamera.hpp"

#include "glm/ext/matrix_transform.hpp"
#include <glm/ext/matrix_clip_space.hpp>

namespace Tefnout
{
namespace Rendering
{
namespace Camera
{

// @NOTE
// Default to glm right handed coordinates
// https://github.com/g-truc/glm/blob/master/glm/detail/setup.hpp
// @NOTE

Orthographic::Orthographic(const FrustrumBounds bounds, const ClippingLimits clippingLimits)
    : m_bounds{bounds}, m_clipping{clippingLimits}, m_view{1.0f},
      m_projection(glm::ortho(m_bounds.Left, m_bounds.Right, m_bounds.Bottom, m_bounds.Top,
                              m_clipping.Near, m_clipping.Far)),
      m_projectionView{m_projection * m_view}, m_rotationZ{0.0f}, m_position{0.0f}
{
}


Orthographic::Orthographic(const FrustrumBounds bounds)
    : Orthographic(bounds, {-1.0f, 1.0f})
{
}

void Orthographic::SetPosition(glm::vec3 position)
{
    m_position = position;
    UpdateMatrices();
}
void Orthographic::SetRotationZ(float degrees)
{
    m_rotationZ = glm::radians(degrees);
    UpdateMatrices();
}

void Orthographic::UpdateMatrices()
{
    // // Compute camera transform (view inverse) then reverse it
    // // T * R * S --> Scale then Rotate then translate the camera
    // auto invViewT = glm::translate(glm::mat4{1.0f}, m_position);
    // auto invViewRT = rotate(invViewT, m_rotationZ, s_Zaxis);
    // m_view = glm::inverse(invViewRT);

    // Faster to avoid the inverse calculation as below
    // R^-1 == -R
    // T^-1 == -T
    // (T * R)^-1 == -R * -T
    auto viewR = glm::rotate(glm::mat4{1.0f}, -m_rotationZ, s_Zaxis);
    auto m_view = glm::translate(viewR, -m_position);

    // Compute the resulting matric as below
    m_projectionView = m_projection * m_view;
}

} // namespace Camera
} // namespace Rendering
} // namespace Tefnout
