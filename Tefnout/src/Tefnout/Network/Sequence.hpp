#ifndef __TEFNOUT_SEQUENCE__HPP
#define __TEFNOUT_SEQUENCE__HPP

#include "Tefnout/Core/Core.hpp"
#include <limits>

namespace Tefnout
{
namespace Network
{

/**
 * @brief      Determines if @p seq1 is greater than @p seq2 assuming both value comes
 *             from the same sequence generator.
 *
 * @param[in]  seq1  The first sequence element
 * @param[in]  seq2  The secdond sequence element
 *
 * @tparam     T     Sequence element type
 *
 * @return     True if @p seq1 is greater than @p seq2
 */
template <typename T> inline constexpr TEFNOUT_API bool IsGreater(T seq1, T seq2)
{
    constexpr T middleSizeLimit = std::numeric_limits<T>::max() / 2;
    return ((seq1 > seq2) && (seq1 - seq2 <= middleSizeLimit)) ||
           ((seq1 < seq2) && (seq2 - seq1 > middleSizeLimit));
}

} // namespace Network
} // namespace Tefnout

#endif
