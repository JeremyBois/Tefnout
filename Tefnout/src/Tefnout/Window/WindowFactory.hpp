#ifndef __WINDOWFACTORY__HPP
#define __WINDOWFACTORY__HPP

#include "Tefnout/Core/Platform.hpp"
#include "TefnoutPCH.hpp"

#include "Tefnout/Window/IWindow.hpp"


namespace Tefnout
{
namespace Window
{
    std::unique_ptr<IWindow> Create(const GenericProperties &properties);
}

} // namespace Tefnout

#endif
