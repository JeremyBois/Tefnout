#include "WindowFactory.hpp"

#if defined _WIN32 || defined __GNUC__
#include "Tefnout/Window/GLFW/GLFWBackend.hpp"
#else
#include "Tefnout/Core/Logger.hpp"
#include "Tefnout/Window/NullBackend.hpp"
#endif

namespace Tefnout
{
namespace Window
{

std::unique_ptr<IWindow> Create(const GenericProperties &properties)
{
#if defined _WIN32 || defined __GNUC__
    return std::make_unique<GLFWBackend>(properties);
#else
    TEFNOUT_ERROR("NullBackend service will be used. Rendering not supported on this platform.");
    return std::make_unique<NullBackend>();
#endif
}
} // namespace Window
} // namespace Tefnout
