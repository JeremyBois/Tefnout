#include "TefnoutPCH.hpp"

#include "Entity.hpp"
#include "Tefnout/Core/Debug.hpp"

#include <limits>

namespace Tefnout
{
namespace ECS
{

Entity::Entity() : m_id(std::numeric_limits<entity_type>::max())
{
}

Entity::Entity(entity_type id) : m_id(id)
{
    TEFNOUT_ASSERT(id != std::numeric_limits<entity_type>::max(),
                   "New ID equals Entity::s_null. {0}",
                   "<id> must not be equals to std::numeric_limits<entity_type>::max()");
}
} // namespace ECS
} // namespace Tefnout
