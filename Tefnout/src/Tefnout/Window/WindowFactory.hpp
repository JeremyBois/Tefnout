#ifndef __WINDOWFACTORY__HPP
#define __WINDOWFACTORY__HPP

#include "Tefnout/Window/IWindow.hpp"

namespace Tefnout
{
namespace Window
{
TEFNOUT_LOCAL std::unique_ptr<IWindow> Create(const GenericProperties &properties);
}

} // namespace Tefnout

#endif
