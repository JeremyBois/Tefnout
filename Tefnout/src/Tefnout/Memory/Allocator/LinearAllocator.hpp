#ifndef __LINEARALLOCATOR__HPP
#define __LINEARALLOCATOR__HPP

#include "Tefnout/Core/Core.hpp"
#include "Tefnout/Core/Logger.hpp"

// @TODO
// GetAllocationSize is currently not used because no bounds checker expect the dummy one is
// implemented.
// @TODO

namespace Tefnout
{
namespace Memory
{
/**
 * @brief      A simple linear allocator policy where allocation and de-allocation is
 *             O(1). Alignment is handled under the hood.
 *
 * @note       Allocations do not assume anything about requested size (size of type, size
 *             of bounds + type, ...).
 */
class TEFNOUT_API LinearAllocator
{
  public:
    using size_type = std::size_t;

    LinearAllocator() = delete;
    explicit LinearAllocator(char *start, char *end) : m_pStart(start), m_pEnd(end), m_cursor(start)
    {
    }

    ~LinearAllocator(){};

    // Disable copy
    LinearAllocator &operator=(LinearAllocator other) = delete;
    LinearAllocator(const LinearAllocator &other) = delete;

    /**
     * @brief      Get a free memory address with enough memory for the requested size.
     * @note       @p size should be the full size including bounds is bounds checking is
     *             required.
     *
     * @warning    Assert will be raised if - size is 0 - alignment is not a power of 2 -
     *             there is not enough remaining memory
     *
     * @param[in]  size            The allocation size requested
     * @param[in]  alignment       The alignment requested for the type
     * @param[in]  frontBoundSize  The front bound size is any
     *
     * @return     Address with enough free space for requested size (with proper
     *             alignment).
     */
    inline void *Allocate(size_type size, size_type alignment, size_type frontBoundSize)
    {
        TEFNOUT_ASSERT(size != 0, "Assertion fails :: Size ({0}) is invalid.", size);

        const std::size_t mask = alignment - 1;
        TEFNOUT_ASSERT((alignment & mask) == 0,
                       "Assertion fails :: Alignment ({0}) is not a power of 2.", alignment);

        // Find the number of needed additionnal bytes
        // Force alignment of data (frontBound + data) because bound are debug only
        const std::size_t shift =
            (alignment - (reinterpret_cast<uintptr_t>(m_cursor) + frontBoundSize) % alignment) %
            alignment;

            // TEFNOUT_ERROR("cursor={0} - shift={1} - alignment={2} - frontBoundSize={3}",
            //               reinterpret_cast<uintptr_t>(m_cursor), shift, alignment,
            //               frontBoundSize);

            TEFNOUT_ASSERT(m_cursor + size + shift <= m_pEnd, "Assertion fails :: {0}.",
                           "Linear allocator reserved memory is exhausted");

        char *alignedAddress = m_cursor + shift;
        m_cursor = alignedAddress + size;

        return alignedAddress;
    }

    /**
     * @brief      Does nothing at all.
     *
     * @param      objPtr  The object pointer
     */
    inline void Free(void *objPtr){};

    /**
     * @brief      Gets the allocated size for a specific object pointer
     *
     * @param      objPtr  The object pointer
     *
     * @return     The allocated size.
     */
    inline size_type GetAllocationSize(void *objPtr) const
    {
        return sizeof(objPtr);
    }

    /**
     * @brief      Move cursor back start of reserved memory. All new call to Allocate
     *             will be done on top of existing.
     */
    void Clear()
    {
        m_cursor = m_pStart;
    }

  private:
    char *m_pStart;
    char *m_pEnd;
    char *m_cursor;
};

} // namespace Memory
} // namespace Tefnout

#endif
