#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

#include "Tefnout/Core/Logger.hpp"


int main(int argc, char *argv[])
{
    // https://github.com/catchorg/Catch2/blob/devel/docs/own-main.md#top
    // Needed to avoid core dump when logging
    Tefnout::Logger::Init();

    int result = Catch::Session().run(argc, argv);

    return result;
}
