#ifndef __TEFRENOUTMACRO__HPP
#define __TEFRENOUTMACRO__HPP


// More at
// https://cmake.org/Wiki/BuildingWinDLL
// https://gcc.gnu.org/wiki/Visibility


// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
    #if defined __MINGW32__
        // GCC with MINGW
        #define TEFRENOUT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define TEFRENOUT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define TEFRENOUT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        // Windows
        #define TEFRENOUT_HELPER_DLL_IMPORT __declspec(dllimport)
        #define TEFRENOUT_HELPER_DLL_EXPORT __declspec(dllexport)
        #define TEFRENOUT_HELPER_DLL_LOCAL
#endif
#else
    #if __GNUC__ >= 4
        // GCC
        #define TEFRENOUT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define TEFRENOUT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define TEFRENOUT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        // GCC does not support __attribute__ before version 4.
        #define TEFRENOUT_HELPER_DLL_IMPORT
        #define TEFRENOUT_HELPER_DLL_EXPORT
        #define TEFRENOUT_HELPER_DLL_LOCAL
    #endif
#endif

// Now we use the generic helper definitions above to define TEFRENOUT_API and TEFRENOUT_LOCAL.
// TEFRENOUT_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// TEFRENOUT_LOCAL is used for non-api symbols.
#ifdef TEFRENOUT_SHARED              // Defined if TEFRENOUT is compiled as a DLL
    #ifdef TEFRENOUT_SHARED_EXPORTS  // Defined if we are building the TEFRENOUT DLL (instead of using it)
        #define TEFRENOUT_API TEFRENOUT_HELPER_DLL_EXPORT
    #else
        #define TEFRENOUT_API TEFRENOUT_HELPER_DLL_IMPORT
    #endif // TEFRENOUT_SHARED_EXPORTS
    #define TEFRENOUT_LOCAL TEFRENOUT_HELPER_DLL_LOCAL
#else  // TEFRENOUT_SHARED is not defined: this means TEFRENOUT is a STATIC lib.
    #define TEFRENOUT_API
    #define TEFRENOUT_LOCAL
#endif // TEFRENOUT_SHARED

// STL
// #include <string>
#include <iostream>
// #include <map>

using std::cout;
using std::endl;


#endif
