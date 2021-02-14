#ifndef __TEFNOUTPCH__HPP
#define __TEFNOUTPCH__HPP

/*
    Used as a precompiled header to reduce compilation time.
    Will be available in the whole project.
*/

// @WARNINGS
// Platform specific macros
// Should be manually included where macro is needed
// because pre-processing happens before compilation
#include "Tefnout/Core/Platform.hpp"
// @WARNINGS

// Debug tools should not be updated too often
#include "Tefnout/Core/Logger.hpp"

// STL
#include <iostream>
#include <stdint.h>
#include <string>
#include <string_view>

#include <functional>
#include <memory>

#include <map>
#include <vector>

#endif
