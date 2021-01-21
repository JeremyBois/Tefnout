#ifndef __LOGGER__HPP
#define __LOGGER__HPP

// DISABLE / ENABLE macros based on TEFNOUT_ACTIVE_LEVEL
// Define to enable / disable log function at compile time
// #define TEFNOUT_ACTIVE_LEVEL SPDLOG_LEVEL_ERROR  // Release
#define TEFNOUT_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // Debug

#include "Tefnout_macro.hpp" // Needed for macro definition
#include <memory>

#include "spdlog/logger.h"
#include "spdlog/spdlog.h"

namespace Tefnout
{

class TEFNOUT_API Logger
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

} // namespace Tefnout

// General macro
#define TEFNOUT_LOGGER_CALL(logger, level, ...)                                                                      \
    (logger)->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

// ENGINE LOGS
#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define TEFNOUT_TRACE(...)                                                                                           \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#define TEFNOUT_TRACE(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define TEFNOUT_DEBUG(...)                                                                                           \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define TEFNOUT_DEBUG(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define TEFNOUT_INFO(...)                                                                                            \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::info, __VA_ARGS__)
#else
#define TEFNOUT_INFO(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define TEFNOUT_WARN(...)                                                                                            \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#define TEFNOUT_WARN(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define TEFNOUT_ERROR(...)                                                                                           \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::err, __VA_ARGS__)
#else
#define TEFNOUT_ERROR(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define TEFNOUT_CRITICAL(...)                                                                                        \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetEngineLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#define TEFNOUT_CRITICAL(...) (void)0
#endif

// USER LOGS
#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_TRACE
#define USER_TRACE(...) TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::trace, __VA_ARGS__)
#else
#define USER_TRACE(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define USER_DEBUG(...) TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::debug, __VA_ARGS__)
#else
#define USER_DEBUG(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define USER_INFO(...) TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::info, __VA_ARGS__)
#else
#define USER_INFO(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_WARN
#define USER_WARN(...) TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::warn, __VA_ARGS__)
#else
#define USER_WARN(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_ERROR
#define USER_ERROR(...) TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::err, __VA_ARGS__)
#else
#define USER_ERROR(...) (void)0
#endif

#if TEFNOUT_ACTIVE_LEVEL <= SPDLOG_LEVEL_CRITICAL
#define USER_CRITICAL(...)                                                                                             \
    TEFNOUT_LOGGER_CALL(::Tefnout::Logger::GetUserLogger(), spdlog::level::critical, __VA_ARGS__)
#else
#define USER_CRITICAL(...) (void)0
#endif

#endif
