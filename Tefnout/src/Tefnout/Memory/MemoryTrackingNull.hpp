#ifndef __MEMORYTRACKINGNULL__HPP
#define __MEMORYTRACKINGNULL__HPP

#include "Tefnout/Core/Core.hpp"

namespace Tefnout
{
namespace Memory
{
/**
 * @brief      Dummy tracking policy that does nothing
 */
class TEFNOUT_LOCAL MemoryTrackingNull
{
  public:
    /**
     * @brief      Called on memory allocation.
     *
     * @param      <unnamed>  Allocated memory origin pointer
     * @param[in]  <unnamed>  Size of allocated memory
     * @param[in]  <unnamed>  Size of data in allocated memory
     * @param[in]  <unnamed>  Information about the allocation
     */
    inline void OnAllocation(void *, std::size_t, std::size_t, const char *) const
    {
    }
    /**
     * @brief      Called on memory de-allocation.
     *
     * @param      <unnamed>  De-allocated memory origin pointer
     */
    inline void OnFree(void *) const
    {
    }
};
} // namespace Memory
} // namespace Tefnout
#endif
