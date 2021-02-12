#ifndef __STREAMIO__HPP
#define __STREAMIO__HPP

#include "TefnoutPCH.hpp"

namespace Tefnout
{
namespace Utility
{
/**
 * @brief      Static class helper for IO operations.
 */
class TEFNOUT_API StreamIO
{
  private:
    StreamIO();

  public:
    StreamIO(StreamIO const &) = delete;
    void operator=(StreamIO const &) = delete;

    static std::string ReadFile(const std::string &filePath);
};
} // namespace Utility
} // namespace Tefnout

#endif
