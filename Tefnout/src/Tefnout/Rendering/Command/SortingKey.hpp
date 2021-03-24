#ifndef __SORTINGKEY__HPP
#define __SORTINGKEY__HPP

#include "Tefnout/Utility/BitsController.hpp"

#include <cstdint>


/*
 @REFERENCE
    - http://realtimecollisiondetection.net/blog/?p=86
 @REFERENCE
 */


namespace Tefnout
{
namespace Rendering
{
namespace Command
{

/// Engine type used for sorting keys
using SortingKey = std::uint64_t;

/**
 * @brief      A sorting key layout element.
 */
struct Element
{
    std::uint8_t Size = 1;
    std::uint8_t Offset = 0;
};

/**
 * @brief      Allowed draw types for a specific geometry.
 */
enum class DrawCall : std::uint8_t
{
    Opaque = 0,
    Transparent = 1,
    Additive = 2,
    Subtractive = 3,
};

/**
 * @brief      Least to most significant bits assignation for geometry bit mask
 */
struct SortingKeyLayout
{
  public:
    // Material (shader + textures + constants)
    static constexpr Element Material{20, 0};

    // Depth
    static constexpr Element Depth{20, 20};

    // DrawCall
    static constexpr Element DrawCall{2, 40};

    // Viewport layer
    static constexpr Element ViewportLayer{3, 42};

    // Viewport (camera number, split screen, texture, mirrors, ...)
    static constexpr Element Viewport{5, 45};

    // Full-screen or HUD or FX
    static constexpr Element Target{3, 50};
};

/**
 * @brief      Builds a sorting key using ids for each component.
 *
 * @param[in]  materialId       The material identifier
 * @param[in]  depth            The depth
 * @param[in]  drawType         The draw type
 * @param[in]  viewportLayerId  The viewport layer identifier
 * @param[in]  viewportId       The viewport identifier
 * @param[in]  targetId         The target identifier
 *
 * @return     The generated sorting key.
 */
SortingKey BuildSortingKey(std::uint32_t materialId, std::uint32_t depth, DrawCall drawType,
                           std::uint8_t viewportLayerId, std::uint8_t viewportId,
                           std::uint8_t targetId);

} // namespace Command
} // namespace Rendering
} // namespace Tefnout

#endif
