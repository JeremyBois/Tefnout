#include "WindowFactory.hpp"

#ifdef __GNUC__
#include "Tefnout/Window/GLFWBackend.hpp"
#else
#include "Tefnout/Window/GLFWBackend.hpp"
#endif

namespace Tefnout
{
namespace Window
{

std::unique_ptr<IWindow> Create(const GenericProperties &properties)
{
#ifdef __GNUC__
    return std::make_unique<GLFWBackend>(properties);
#else
#endif
}
} // namespace Window
} // namespace Tefnout
