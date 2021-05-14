#ifndef __ENTITY__HPP
#define __ENTITY__HPP

#include "Tefnout/Core/Platform.hpp"

#include <cstdint>
#include <ostream>

namespace Tefnout
{
namespace ECS
{

/**
 * @brief      A small wrapper arround an int64.
 */
struct TEFNOUT_API Entity
{
  public:
    using entity_type = std::uint64_t;

    Entity();
    explicit Entity(entity_type id);
    ~Entity() = default;

    inline entity_type GetId() const
    {
        return m_id;
    }

    friend TEFNOUT_API bool operator==(const Entity& first, const Entity& second)
    {
        return first.m_id == second.m_id;
    }
    friend TEFNOUT_API bool operator!=(const Entity& first, const Entity& second)
    {
        return first.m_id != second.m_id;
    }
    friend TEFNOUT_API bool operator<(const Entity& first, const Entity& second)
    {
        return first.m_id < second.m_id;
    }

  private:
    entity_type m_id;
};

inline std::ostream& operator<<(std::ostream& os, const Entity& entity)
{
    return os << "Entity<id=" << entity.GetId() << ">";
}

/// Placeholder Entity used to detect uninitialized slot in sparseArray
inline const Entity nullEntity = Entity{};

} // namespace ECS
} // namespace Tefnout

#endif
