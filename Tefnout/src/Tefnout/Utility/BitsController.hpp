#ifndef __BITSCONTROLLER__HPP
#define __BITSCONTROLLER__HPP

#include "Tefnout/Core/Core.hpp"

#include <array>


namespace Tefnout
{
namespace Utility
{
namespace Bits
{

/**
 * @brief      Container to store a value and offset pair.
 */
struct TEFNOUT_LOCAL ValueOffsetPair
{
  public:
    int Value{};
    unsigned int Offset = 0;
};

/**
 * @brief      Extract a bit range from a 64bits integer.
 *
 * @param[in]  source  The source used to extract bits
 * @param[in]  offset  The offset from least significant bit used to defined RIGHT bound
 * @param[in]  size    The size of the range used to defined the LEFT bound
 *
 * @return     Extracted bit range as a 64bits integer
 */
inline std::uint64_t TEFNOUT_LOCAL Extract(std::uint64_t source, const unsigned int offset,
                                          const unsigned int size)
{
    TEFNOUT_ASSERT(size + offset < 64, "Out of bounds (offset={0}, size={1})", offset, size);

    // ~0          --> -0b001
    // (~0  << 2)  -->  0b100  (Only 1 at pos 2)
    // ~(~0 << 2)  -->  0b011  (1s become 0s and 0s become 1s)
    const std::uint64_t mask = ~(~0 << size);

    // Right shift to discard least significant digits
    // 0b1011 >> 2 --> 0b10
    return (source >> offset) & mask;
}

inline std::uint64_t TEFNOUT_LOCAL Clear(std::uint64_t source, const unsigned int offset,
                                        const unsigned int size)
{
    TEFNOUT_ASSERT(size + offset < 64, "Out of bounds (offset={0}, size={1})", offset, size);

    // ~0          --> -0b001
    // (~0  << 2)  -->  0b100  (Only 1 at pos 2)
    // ~(~0 << 2)  -->  0b011  (1s become 0s and 0s become 1s)
    const std::uint64_t mask = ~(~0 << size);

    // Left shift our mask and revert it
    // 0b011 << 2 --> 0b1100
    // ~0b1100    --> 0b0011
    return source & ~(mask << offset);
}

/**
 * @brief      Set a value at a specific offset inside the source provided.
 *
 * @note       Bytes ARE NOT cleared first.
 *
 * @param[in]  source  The source used as container for @p value
 * @param[in]  value   The value to set just after @p offset
 * @param[in]  offset  The offset from the least significant bit used to place @p value
 *
 * @return     Updated source with @p value at position defined by @p offset
 */
inline std::uint64_t TEFNOUT_LOCAL Set(std::uint64_t source, const std::uint64_t value,
                                      const unsigned int offset)
{
    TEFNOUT_ASSERT(offset < 64, "Out of bounds (offset={0})", offset);

    // 0b0011 | (0b11 << 3)  --> 0b11011
    return source | (value << offset);
}

/**
 * @brief      Built a in64 container from a collection of {value, offset} pairs.
 *
 * @note       Correct offseting to avoid overflow is user responsability hence not
 *             checked.
 *
 * @param[in]  values  The values as a collection of pairs {value, offset}
 *
 * @tparam     Size    Number of values inside the collection.
 *
 * @return     Newly created container value based on provided @p values
 */
template <std::size_t Size>
inline std::uint64_t TEFNOUT_LOCAL Set(const std::array<ValueOffsetPair, Size>& values)
{
    std::uint64_t source{};

    for (const auto& [value, offset] : values)
    {
        TEFNOUT_ASSERT(offset < 64, "Out of bounds (offset={0})", offset);

        // @UNSURE Reuse Set because inline hint the compiler ??
        source |= (value << offset);
    }

    return source;
}

/**
 * @brief      Set a value at a specific offset inside the source provided.
 *
 * @note       Bytes from @p offset until @p offset + @p rangeSize ARE first cleared then value is set.
 *
 * @param[in]  source     The source used as container for @p value
 * @param[in]  value      The value to set just after @p offset
 * @param[in]  offset     The offset from the least significant bit used to place @p value
 * @param[in]  rangeSize  The number of bits to clear (usually the size reserved to store
 *                        values at a specific offset)
 *
 * @return     Updated source with @p value at position defined by @p offset
 */
inline std::uint64_t TEFNOUT_LOCAL ClearAndSet(std::uint64_t source, const std::uint64_t value,
                                              const unsigned int offset,
                                              const unsigned int rangeSize)
{
    TEFNOUT_ASSERT(rangeSize + offset < 64, "Out of bounds (offset={0}, rangeSize={1})", offset,
                   rangeSize);

    // Clear the whole reserved range first even if value does not need everything
    source = Clear(source, offset, rangeSize);

    // 0b0011 | (0b11 << 3)  --> 0b11011
    return source | (value << offset);
}

} // namespace Bits
} // namespace Utility
} // namespace Tefnout

#endif
