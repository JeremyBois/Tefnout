#ifndef __ENTRYPOINT__HPP
#define __ENTRYPOINT__HPP

#include "Tefrenout_macro.hpp" // Needed for macro definition

#include "Application.hpp"
#include "Logger.hpp"

// Will be defined in client code (See Samples)
extern Tefrenout::Application *Tefrenout::CreateApplication();

// Embed main to keep control of the engine during its whole lifetime
int main() // NOLINT (Remove clang-tidy warning)
{
    // Init logger
    Tefrenout::Logger::Init();
    TEFRENOUT_INFO("Welcome to the TEFRENOUT engine !");

    // Init engine application
    auto *app = Tefrenout::CreateApplication();
    app->Run();

    delete app;
}

#endif
