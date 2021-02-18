#ifndef __DEBUG__HPP
#define __DEBUG__HPP

// Add a signal that can be intercepted by the debugger before closing the program
#if defined TEFNOUT_DEBUG
    #if defined __MINGW32__ || __GNUC__
        // Windows + MinGW || Linux
        #include <signal.h>
        #define TEFNOUT_DEBUGBREAK() raise(SIGTRAP)
    #elif defined _WIN32 || defined __CYGWIN__
        // Windows only
        #define TEFNOUT_DEBUGBREAK() __debugbreak()
    #else
        // Not yet supported
        #define TEFNOUT_DEBUGBREAK() (void)0
    #endif

    // Add assert helper that can log a message first then raise a debug break signal
    // To test macro extension      --> http://coliru.stacked-crooked.com/
    // With the following arguments --> g++ -std=c++17 -O2 -Wall -pedantic -pthread main.cpp -E
    #define TEFNOUT_ASSERT_CALL(type, check, msg, ...)                                                 \
        {                                                                                              \
            if (!(check))                                                                              \
            {                                                                                          \
                type##_ERROR(msg, __VA_ARGS__);                                                        \
                TEFNOUT_DEBUGBREAK();                                                                  \
            }                                                                                          \
        }
    #define TEFNOUT_ASSERT(...) TEFNOUT_ASSERT_CALL(TEFNOUT, __VA_ARGS__)
    #define USER_ASSERT(...) TEFNOUT_ASSERT_CALL(USER, __VA_ARGS__)
#else
    #define TEFNOUT_ASSERT(...) (void)0
    #define USER_ASSERT(...) (void)0
#endif
#endif
