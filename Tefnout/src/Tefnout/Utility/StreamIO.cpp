#include "StreamIO.hpp"
#include "Tefnout/Core/Logger.hpp"

#include <fstream>
#include <optional>
#include <sstream>

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
} // namespace Utility
} // namespace Tefnout
