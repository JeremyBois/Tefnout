#ifndef __BOUNDSCHECKINGNULL__HPP
#define __BOUNDSCHECKINGNULL__HPP

#include "Tefnout/Core/Core.hpp"

namespace Tefnout
{
namespace Memory
{
/**
 * @brief      A dummy bounds checker that do nothing more than declaring the
 *             BoundsChecker policy required interface.
 */
class TEFNOUT_LOCAL BoundsCheckingNull
{
  public:
    static const std::size_t s_sizeFront = 4;
    static const std::size_t s_sizeBack = 0;
};
} // namespace Memory
} // namespace Tefnout

#endif
