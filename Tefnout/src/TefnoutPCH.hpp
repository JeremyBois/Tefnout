#ifndef __TEFNOUTPCH__HPP
#define __TEFNOUTPCH__HPP

/*
    Used as a precompiled header to reduce compilation time.
    Will be available in the whole project.
*/

// Platform specific macros
// WARNINGS
// Should be manually included where macro is needed
// because pre-processing happens before compilation
#include "Tefnout/Core/Platform.hpp"

// STL
#include <stdint.h>
#include <iostream>
#include <string>

#include <memory>
#include <functional>

#include <map>
#include <vector>

// Thanks !
using std::cout;
using std::endl;

#endif
