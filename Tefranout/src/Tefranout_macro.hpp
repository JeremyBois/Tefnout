#ifndef __TEFRANOUTMACRO__HPP
#define __TEFRANOUTMACRO__HPP


// More at
// https://cmake.org/Wiki/BuildingWinDLL
// https://gcc.gnu.org/wiki/Visibility


// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
    #if defined __MINGW32__
        // GCC with MINGW
        #define TEFRANOUT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define TEFRANOUT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define TEFRANOUT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        // Windows
        #define TEFRANOUT_HELPER_DLL_IMPORT __declspec(dllimport)
        #define TEFRANOUT_HELPER_DLL_EXPORT __declspec(dllexport)
        #define TEFRANOUT_HELPER_DLL_LOCAL
#endif
#else
    #if __GNUC__ >= 4
        // GCC
        #define TEFRANOUT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define TEFRANOUT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define TEFRANOUT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        // GCC does not support __attribute__ before version 4.
        #define TEFRANOUT_HELPER_DLL_IMPORT
        #define TEFRANOUT_HELPER_DLL_EXPORT
        #define TEFRANOUT_HELPER_DLL_LOCAL
    #endif
#endif

// Now we use the generic helper definitions above to define TEFRANOUT_API and TEFRANOUT_LOCAL.
// TEFRANOUT_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// TEFRANOUT_LOCAL is used for non-api symbols.
#ifdef TEFRANOUT_SHARED              // Defined if TEFRANOUT is compiled as a DLL
    #ifdef TEFRANOUT_SHARED_EXPORTS  // Defined if we are building the TEFRANOUT DLL (instead of using it)
        #define TEFRANOUT_API TEFRANOUT_HELPER_DLL_EXPORT
    #else
        #define TEFRANOUT_API TEFRANOUT_HELPER_DLL_IMPORT
    #endif // TEFRANOUT_SHARED_EXPORTS
    #define TEFRANOUT_LOCAL TEFRANOUT_HELPER_DLL_LOCAL
#else  // TEFRANOUT_SHARED is not defined: this means TEFRANOUT is a STATIC lib.
    #define TEFRANOUT_API
    #define TEFRANOUT_LOCAL
#endif // TEFRANOUT_SHARED

// STL
// #include <string>
#include <iostream>
// #include <map>

using std::cout;
using std::endl;


#endif
