#include "Logger.hpp"
#include "spdlog/common.h"
#include "spdlog/sinks/stdout_sinks.h"

#include <memory>

// This ignores all warnings raised inside External headers (for both clang and GCC)
// https://nelkinda.com/blog/suppress-warnings-in-gcc-and-clang/#d11e364
_Pragma("GCC diagnostic push")
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
    _Pragma("GCC diagnostic pop")

        namespace Tefrenout
{
    std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
    std::shared_ptr<spdlog::logger> Logger::s_UserLogger;

    void Logger::Init()
    {
        // Store all sinks
        std::vector<spdlog::sink_ptr> sinks;

        // Add a thread safe (_mt) console sink
        // Log in console starting from debug level
        sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks[0]->set_level(spdlog::level::debug);
        // Format as below with color based on level
        // [19:03:44] [TEFRENOUT] [-D-] [Logger.cpp:57] OulalaOulala
        sinks[0]->set_pattern("%^[%H:%M:%S] [%n] [%L] [%s:%#] %v %$");

        // Add a thread safe (_mt) file sink (false means keeping previous logs)
        // Log in file starting from trace level
        // Default format for file logging
        sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Tefrenout.log", false));
        sinks[1]->set_level(spdlog::level::trace);

        // Setup Engine logger manually as a combination of console and file
        s_EngineLogger = std::make_shared<spdlog::logger>("TEFRENOUT", std::begin(sinks), std::end(sinks));
        spdlog::register_logger(s_EngineLogger); // Register for global access
        s_EngineLogger->set_level(spdlog::level::trace);
        s_EngineLogger->flush_on(spdlog::level::debug); // Update log file at each trace

        // Setup User logger manually as a combination of console and file
        s_UserLogger = std::make_shared<spdlog::logger>("USER", std::begin(sinks), std::end(sinks));
        spdlog::register_logger(s_UserLogger); // Register for global access
        s_UserLogger->set_level(spdlog::level::trace);
        s_UserLogger->flush_on(spdlog::level::debug); // Update log file at each trace

        // How to use
        // https://github.com/gabime/spdlog

        // Engine
        // TEFRENOUT_TRACE("TRACE");
        // TEFRENOUT_DEBUG("DEBUG");
        // TEFRENOUT_INFO("INFO");
        // TEFRENOUT_WARN("WARN");
        // TEFRENOUT_ERROR("ERROR");
        // TEFRENOUT_CRITICAL("CRITICAL");

        // User
        // USER_TRACE("TRACE");
        // USER_DEBUG("DEBUG");
        // USER_INFO("INFO");
        // USER_WARN("WARN");
        // USER_ERROR("ERROR");
        // USER_CRITICAL("CRITICAL");

        // With argument and specific formating
        // USER_INFO("CRITICAL {0:d} {1:x}", 44, 666);
    }
}
