#include "catch2/catch.hpp"

#include <chrono>

#include "Tefnout.hpp"


/*
 Unit-Tests for Timer implementation.

 @NOTE
 Using them with different internal type force compile time checks of template code.
 @NOTE
*/


TEST_CASE("Timer can be created with different tick type and different callback", "[Timer]")
{
    SECTION("Timer using double for internal time representation")
    {
        using timeType = std::chrono::duration<double, std::milli>;

        Tefnout::Utility::Timer<timeType> timer("Couscous");
    }

    SECTION("Timer with user defined callback")
    {
        using timeType = std::chrono::hours;

        auto test_callback = [](const Tefnout::Utility::TimerReport<timeType> report) -> void {
            std::cout << report.Start << "--->" << report.End << "(ID=" << report.Id << ")"
                      << std::endl;
        };

        Tefnout::Utility::Timer<timeType> timer("Couscous");
    }
}
