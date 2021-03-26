#include "TefnoutPCH.hpp"

#include "LinearAllocator.hpp"

namespace Tefnout
{
namespace Memory
{

void* LinearAllocator::AllocateImpl(size_type size, size_type alignment,
                                       size_type frontBoundSize)
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

    char* alignedAddress = m_cursor + shift;
    m_cursor = alignedAddress + size;

    return alignedAddress;
}
} // namespace Memory
} // namespace Tefnout
