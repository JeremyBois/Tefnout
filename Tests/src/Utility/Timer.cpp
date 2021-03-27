#include "catch2/catch.hpp"

#include <chrono>
#include <thread>

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

        auto test_callback = [](const Tefnout::Utility::TimerReport<timeType> report) -> void {
            // std::cout << report.Start << "--->" << report.End << "(ID=" << report.Id << ")"
            //           << std::endl;
            REQUIRE(report.Start != report.End);
        };

        Tefnout::Utility::Timer<timeType> timer("Couscous", test_callback);

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    SECTION("Timer with user defined callback")
    {
        using timeType = std::chrono::hours;

        auto test_callback = [](const Tefnout::Utility::TimerReport<timeType> report) -> void {
            REQUIRE(report.Start == report.End);
        };

        Tefnout::Utility::Timer<timeType> timer("Couscous", test_callback);

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
