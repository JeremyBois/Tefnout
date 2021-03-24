#ifndef __MEMORYBLOCKHEAP__HPP
#define __MEMORYBLOCKHEAP__HPP

#include "Tefnout/Core/Core.hpp"

namespace Tefnout
{
namespace Memory
{
class TEFNOUT_API MemoryBlockHeap
{
  public:
    // MemoryBlockHeap() = delete;
    MemoryBlockHeap &operator=(const MemoryBlockHeap &) = delete;

    explicit MemoryBlockHeap(const std::size_t size) : m_size(size)
    {
        // https://en.cppreference.com/w/cpp/memory/c/aligned_alloc
        // Regular std::malloc aligns memory suitable for any object type
        // using alignof(std::max_align_t))
        m_start = static_cast<char*>(std::malloc(m_size));
        m_end = m_start + size;
    }

    ~MemoryBlockHeap()
    {
        std::free(m_start);
    }

    inline char *GetStart() const
    {
        return m_start;
    };

    inline char *GetEnd() const
    {
        return m_end;
    };

    inline std::size_t GetSize() const
    {
        return m_size;
    };

  private:
    const std::size_t m_size;
    char *m_start;
    char *m_end;
};
} // namespace Memory
} // namespace Tefnout

#endif
