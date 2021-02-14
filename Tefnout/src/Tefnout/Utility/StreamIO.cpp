#include "StreamIO.hpp"

#include <fstream>
#include <sstream>
#include <string>

namespace Tefnout
{
namespace Utility
{
std::optional<std::string> StreamIO::ReadFile(const std::string &filePath)
{
    std::ifstream readStream(filePath, std::ios::in);
    if (!readStream.is_open())
    {
        TEFNOUT_ERROR("Could not read file <{0}> (Does not exist)", filePath);
        return std::nullopt;
    }
    else
    {
        TEFNOUT_TRACE("Extract whole content of file at <{0}>", filePath);
    }

    // Iterate over readStream.rdbuf() to build a string
    std::string content(std::istreambuf_iterator<char>(readStream), {});
    readStream.close();

    return content;
}

std::optional<std::string> StreamIO::GetFilenameFrom(std::string filePath, bool keepExtension)
{
    const std::size_t last_slash_idx = filePath.find_last_of("\\/");
    if (std::string::npos != last_slash_idx)
    {
        // If not end of string remove prefix
        filePath.erase(0, last_slash_idx + 1);

        if (!keepExtension)
        {
            const std::size_t period_idx = filePath.rfind('.');
            if (std::string::npos != period_idx)
            {
                // If not end of string remove suffix
                filePath.erase(period_idx);
            }
        }
        return filePath;
    }
    else
    {
        return std::nullopt;
    }
}

std::optional<std::string> StreamIO::GetExtensionFrom(std::string filePath)
{
    const std::size_t last_period_idx = filePath.find_last_of('.');
    if (std::string::npos != last_period_idx)
    {
        filePath.erase(0, last_period_idx + 1);
        return filePath;
    }
    else
    {
        return std::nullopt;
    }
}
} // namespace Utility
} // namespace Tefnout
