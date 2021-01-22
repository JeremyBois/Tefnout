#ifndef __ENTRYPOINT__HPP
#define __ENTRYPOINT__HPP

#include "Application.hpp"
#include "Logger.hpp"

// Will be defined in client code (See Samples)
extern Tefnout::Application *Tefnout::CreateApplication();

// Embed main to keep control of the engine during its whole lifetime
int main() // NOLINT (Remove clang-tidy warning)
{
    // Init logger
    Tefnout::Logger::Init();
    TEFNOUT_INFO("Welcome to the TEFNOUT engine !");

    // Init engine application
    auto *app = Tefnout::CreateApplication();
    app->Run();

    delete app;
}

#endif
