#ifndef __STREAMIO__HPP
#define __STREAMIO__HPP

#include "Tefnout/Core/CorePCH.hpp"
#include <optional>

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

    /**
     * @brief      Reads a file from the operating system.
     *
     * @param[in]  filePath  The file path
     *
     * @return     (Optional) Content of the file at provided file path.
     */
    static std::optional<std::string> ReadFile(const std::string &filePath);

    /**
     * @brief      Gets the filename from a file path,
     *
     * @param[in]  filePath       The file path
     * @param[in]  keepExtension  True if extension should be kept
     *
     * @return     (Optional) The filename extracted from the file path.
     */
    static std::optional<std::string> GetFilenameFrom(std::string filePath,
                                                      bool keepExtension = false);

    /**
     * @brief      Gets the extension from a file path.
     *
     * @param[in]  filePath  The file path
     *
     * @return     (Optional) The extension extracted from the file path.
     */
    static std::optional<std::string> GetExtensionFrom(std::string filePath);
};
} // namespace Utility
} // namespace Tefnout

#endif
