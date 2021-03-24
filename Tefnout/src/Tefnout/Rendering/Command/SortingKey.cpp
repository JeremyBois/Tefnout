#include "TefnoutPCH.hpp"

#include "SortingKey.hpp"

namespace Tefnout
{
namespace Rendering
{
namespace Command
{

SortingKey BuildSortingKey(std::uint32_t materialId, std::uint32_t depth, DrawCall drawType,
                           std::uint8_t viewportLayerId, std::uint8_t viewportId,
                           std::uint8_t targetId)
{
    using namespace Utility;

    SortingKey key = 0;

    key |= Bits::Set(key, materialId, SortingKeyLayout::Material.Offset);
    key |= Bits::Set(key, depth, SortingKeyLayout::Depth.Offset);
    key |= Bits::Set(key, static_cast<std::uint8_t>(drawType), SortingKeyLayout::DrawCall.Offset);
    key |= Bits::Set(key, viewportLayerId, SortingKeyLayout::ViewportLayer.Offset);
    key |= Bits::Set(key, viewportId, SortingKeyLayout::Viewport.Offset);
    key |= Bits::Set(key, targetId, SortingKeyLayout::Target.Offset);

    return key;
}
} // namespace Command
} // namespace Rendering
} // namespace Tefnout
