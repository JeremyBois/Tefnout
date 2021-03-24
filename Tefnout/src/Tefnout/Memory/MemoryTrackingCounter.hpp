#ifndef __MEMORYTRACKINGCOUNTER__HPP
#define __MEMORYTRACKINGCOUNTER__HPP

#include "Tefnout/Core/Core.hpp"
#include "Tefnout/Core/Logger.hpp"

namespace Tefnout
{
namespace Memory
{
/**
 * @brief      Simple tracking policy keeping track of total number of allocation and de-allocation.
 */
class TEFNOUT_LOCAL MemoryTrackingCounter
{
  public:
    ~MemoryTrackingCounter()
    {
        TEFNOUT_DEBUG("Allocations = {0} - De-Allocations = {1}",
                      GetAllocationCount(), GetDeallocationCount());
    }
    /**
     * @brief      Called on memory allocation.
     *
     * @param      <unnamed>  Allocated memory origin pointer
     * @param[in]  <unnamed>  Size of allocated memory
     * @param[in]  <unnamed>  Size of data in allocated memory
     * @param[in]  <unnamed>  Information about the allocation
     */
    inline void OnAllocation(void *, std::size_t, std::size_t, const char *)
    {
        ++m_allocations;
    }
    /**
     * @brief      Called on memory de-allocation.
     *
     * @param      <unnamed>  De-allocated memory origin pointer
     */
    inline void OnFree(void *)
    {
        ++m_deallocations;
    }

    inline std::uint64_t GetActiveAllocations()
    {
        return m_allocations - m_deallocations;
    }

    inline std::uint64_t GetAllocationCount()
    {
        return m_allocations;
    }

    inline std::uint64_t GetDeallocationCount()
    {
        return m_deallocations;
    }

  private:
    std::uint64_t m_allocations = 0;
    std::uint64_t m_deallocations = 0;
};
} // namespace Memory
} // namespace Tefnout

#endif
