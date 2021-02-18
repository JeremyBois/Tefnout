#ifndef __LOGGER__HPP
#define __LOGGER__HPP

#include "Tefnout/Core/Platform.hpp"

// @TODO
// Allow TEFNOUT_LOG_LEVEL to have a default value as below
// without losing cmake override capacities ...

// #ifndef TEFNOUT_LOG_LEVEL
// // Define to enable / disable log function at compile time based on build type
// // See CmakeLists.txt for more information
// #define TEFNOUT_LOG_LEVEL TEFNOUT_LEVEL_INFO
// #endif
// // Force same state in both log lib and engine
// #define SPDLOG_ACTIVE_LEVEL TEFNOUT_LOG_LEVEL

#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
// user defined types logging by implementing operator<<
#include "spdlog/fmt/ostr.h" // must be included


namespace Tefnout
{

class TEFNOUT_API Logger
{
  public:
    /**
     * @brief      Initializes internal loggers used in macro.
     */
    static void Init();

    inline static std::shared_ptr<spdlog::logger> &GetEngineLogger()
    {
        return s_EngineLogger;
    }
    inline static std::shared_ptr<spdlog::logger> &GetUserLogger()
    {
        return s_UserLogger;
    }

  private:
    Logger() = delete;
    ~Logger() = delete;

    static std::shared_ptr<spdlog::logger> s_EngineLogger;
    static std::shared_ptr<spdlog::logger> s_UserLogger;
};

} // namespace Tefnout

// General macro
#define TEFNOUT_LOGGER_CALL(logger, level, ...)                                                    \
    (logger)->log(spdlog::source_loc{__FILE__, __LINE__, static_cast<const char *>(__FUNCTION__)}, level, __VA_ARGS__)

// Log levels
#define TEFNOUT_LEVEL_TRACE 0
#define TEFNOUT_LEVEL_DEBUG 1
#define TEFNOUT_LEVEL_INFO 2
#define TEFNOUT_LEVEL_WARN 3
#define TEFNOUT_LEVEL_ERROR 4
#define TEFNOUT_LEVEL_CRITICAL 5
#define TEFNOUT_LEVEL_OFF 6

// ENGINE LOGS
#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_TRACE
#define TEFNOUT_TRACE(...)                                                                         \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#define TEFNOUT_TRACE(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_DEBUG
#define TEFNOUT_DEBUG(...)                                                                         \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define TEFNOUT_DEBUG(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_INFO
#define TEFNOUT_INFO(...)                                                                          \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::info, __VA_ARGS__)
#else
#define TEFNOUT_INFO(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_WARN
#define TEFNOUT_WARN(...)                                                                          \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#define TEFNOUT_WARN(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_ERROR
#define TEFNOUT_ERROR(...)                                                                         \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::err, __VA_ARGS__)
#else
#define TEFNOUT_ERROR(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_CRITICAL
#define TEFNOUT_CRITICAL(...)                                                                      \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#define TEFNOUT_CRITICAL(...) (void)0
#endif

// USER LOGS
#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_TRACE
#define USER_TRACE(...)                                                                            \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#define USER_TRACE(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_DEBUG
#define USER_DEBUG(...)                                                                            \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define USER_DEBUG(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_INFO
#define USER_INFO(...)                                                                             \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::info, __VA_ARGS__)
#else
#define USER_INFO(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_WARN
#define USER_WARN(...)                                                                             \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#define USER_WARN(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_ERROR
#define USER_ERROR(...)                                                                            \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::err, __VA_ARGS__)
#else
#define USER_ERROR(...) (void)0
#endif

#if TEFNOUT_LOG_LEVEL <= TEFNOUT_LEVEL_CRITICAL
#define USER_CRITICAL(...)                                                                         \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#define USER_CRITICAL(...) (void)0
#endif

#endif
