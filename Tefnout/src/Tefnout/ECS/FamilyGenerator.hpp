#ifndef __FAMILYGENERATOR__HPP
#define __FAMILYGENERATOR__HPP

#include "Tefnout/Core/Core.hpp"

#include <bits/c++config.h>

namespace Tefnout
{
namespace ECS
{
/**
 * @brief      Static helper that generate unique ID in sequential order.
 * @note       Generated at compilation (@cite
 *             https://skypjack.github.io/2019-02-14-ecs-baf-part-1/).
 */
class TEFNOUT_LOCAL FamilyGenerator
{
  public:
    FamilyGenerator(FamilyGenerator const&) = delete;
    void operator=(FamilyGenerator const&) = delete;

    // Empty template to force compiler to generate a version for
    // each type during compilation
    template <typename> inline static std::size_t TypeIdentifier() noexcept
    {
        // Only called once per type at compile time
        // Value is then always the same
        static const std::size_t value = Generate();
        return value;
    }

  private:
    static std::size_t Generate() noexcept
    {
        // Local static is resolved at compilation
        // and will not be recreated when Generate is called
        // again. So each call will actually only increment it.
        static std::size_t value = 0;
        return value++;
    }
};
} // namespace ECS
} // namespace Tefnout
#endif
