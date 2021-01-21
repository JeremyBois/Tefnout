#ifndef __LOGGER__HPP
#define __LOGGER__HPP

// DISABLE / ENABLE macros based on TEFRENOUT_ACTIVE_LEVEL
// Define to enable / disable log function at compile time
// #define TEFRENOUT_ACTIVE_LEVEL SPDLOG_LEVEL_ERROR  // Release
#define TEFRENOUT_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Debug

#include "Tefrenout_macro.hpp" // Needed for macro definition
#include <memory>

#include "spdlog/logger.h"
#include "spdlog/spdlog.h"

namespace Tefrenout
{

class TEFRENOUT_API Logger
{
  public:
    Logger() = delete;
    ~Logger() = delete;

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
    static std::shared_ptr<spdlog::logger> s_EngineLogger;
    static std::shared_ptr<spdlog::logger> s_UserLogger;
};

} // namespace Tefrenout

// General macro
#define TEFRENOUT_LOGGER_CALL(logger, level, ...)                                                                      \
    (logger)->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

// ENGINE LOGS
#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define TEFRENOUT_TRACE(...)                                                                                           \
    TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetEngineLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#define TEFRENOUT_TRACE(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define TEFRENOUT_DEBUG(...)                                                                                           \
    TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetEngineLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define TEFRENOUT_DEBUG(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define TEFRENOUT_INFO(...)                                                                                            \
    TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetEngineLogger(), spdlog::level::info, __VA_ARGS__)
#else
#define TEFRENOUT_INFO(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define TEFRENOUT_WARN(...)                                                                                            \
    TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetEngineLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#define TEFRENOUT_WARN(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define TEFRENOUT_ERROR(...)                                                                                           \
    TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetEngineLogger(), spdlog::level::err, __VA_ARGS__)
#else
#define TEFRENOUT_ERROR(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define TEFRENOUT_CRITICAL(...)                                                                                        \
    TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetEngineLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#define TEFRENOUT_CRITICAL(...) (void)0
#endif

// USER LOGS
#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define USER_TRACE(...) TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetUserLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#define USER_TRACE(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define USER_DEBUG(...) TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetUserLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define USER_DEBUG(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define USER_INFO(...) TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetUserLogger(), spdlog::level::info, __VA_ARGS__)
#else
#define USER_INFO(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define USER_WARN(...) TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetUserLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#define USER_WARN(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define USER_ERROR(...) TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetUserLogger(), spdlog::level::err, __VA_ARGS__)
#else
#define USER_ERROR(...) (void)0
#endif

#if TEFRENOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define USER_CRITICAL(...)                                                                                             \
    TEFRENOUT_LOGGER_CALL(::Tefrenout::Logger::GetUserLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#define USER_CRITICAL(...) (void)0
#endif

#endif
