#ifndef __PLATFORM__HPP
#define __PLATFORM__HPP

// More at
// https://cmake.org/Wiki/BuildingWinDLL
// https://gcc.gnu.org/wiki/Visibility


// Generic helper definitions for shared library support
#if defined _WIN32 || (defined __CYGWIN__)
    #if defined __MINGW32__ || (defined __MINGW64__)
        // GCC with MINGW
        #define TEFNOUT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define TEFNOUT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define TEFNOUT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #elif defined _MSC_VER
        // Microsoft compiler
        #define TEFNOUT_HELPER_DLL_IMPORT __declspec(dllimport)
        #define TEFNOUT_HELPER_DLL_EXPORT __declspec(dllexport)
        #define TEFNOUT_HELPER_DLL_LOCAL
    #else
        // Unknown
        #define TEFNOUT_HELPER_DLL_IMPORT
        #define TEFNOUT_HELPER_DLL_EXPORT
        #define TEFNOUT_HELPER_DLL_LOCAL
    #endif
#else
    #if __GNUC__ >= 4 || (defined __clang__)
        // GCC
        #define TEFNOUT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define TEFNOUT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define TEFNOUT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
        // GCC does not support __attribute__ before version 4.
        #define TEFNOUT_HELPER_DLL_IMPORT
        #define TEFNOUT_HELPER_DLL_EXPORT
        #define TEFNOUT_HELPER_DLL_LOCAL
    #endif
#endif

// Now we use the generic helper definitions above to define TEFNOUT_API and TEFNOUT_LOCAL.
// TEFNOUT_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// TEFNOUT_LOCAL is used for non-api symbols.
#ifdef TEFNOUT_SHARED              // Defined if TEFNOUT is compiled as a DLL
    #ifdef TEFNOUT_SHARED_EXPORTS  // Defined if we are building the TEFNOUT DLL (instead of using it)
        #define TEFNOUT_API TEFNOUT_HELPER_DLL_EXPORT
    #else
        #define TEFNOUT_API TEFNOUT_HELPER_DLL_IMPORT
    #endif // TEFNOUT_SHARED_EXPORTS
    #define TEFNOUT_LOCAL TEFNOUT_HELPER_DLL_LOCAL
#else  // TEFNOUT_SHARED is not defined: this means TEFNOUT is a STATIC lib.
    #define TEFNOUT_API
    #define TEFNOUT_LOCAL
#endif // TEFNOUT_SHARED


// Compile time hash function that works also for template function
// as opposed to __func__ that only extract function name
#if defined _MSC_VER
    #define FUNC_NAME __FUNCSIG__
#elif defined __clang__ || (defined __GNUC__)
    #define FUNC_NAME __PRETTY_FUNCTION__
#endif


// Allow to test current platform at compile time
// Needed for networking
#define TEFNOUT_PLATFORM_WINDOWS  1
#define TEFNOUT_PLATFORM_MAC      2
#define TEFNOUT_PLATFORM_UNIX     3

#if defined _WIN32 || (defined __CYGWIN__)
    #define TEFNOUT_PLATFORM TEFNOUT_PLATFORM_WINDOWS
#elif defined(__APPLE__)
    #define TEFNOUT_PLATFORM TEFNOUT_PLATFORM_MAC
#else
    #define TEFNOUT_PLATFORM TEFNOUT_PLATFORM_UNIX
#endif


#endif
