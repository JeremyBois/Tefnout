#ifndef __TIMER__HPP
#define __TIMER__HPP

#include "Tefnout/Core/Core.hpp"
#include "Tefnout/Core/Platform.hpp"

#include <chrono>
#include <functional>
#include <iostream>

namespace Tefnout
{
namespace Utility
{

/**
 * @brief      A container to store Timer data when stopped
 *
 * @tparam     TDuration  Type info about the unit used for Start and End
 */
template <typename TDuration = std::chrono::milliseconds> struct TEFNOUT_API TimerReport
{
    const std::string Id;
    const long long Start;
    const long long End;

    TimerReport(const long long timerStart, const long long timerEnd, const std::string timerId)
        : Id(timerId), Start(timerStart), End(timerEnd)
    {
    }
};

/**
 * @brief      A RAII timer implementation where user can choose how to handle result
 *             by providing a specific callback.
 *
 * @tparam     TDuration  Duration time cast used to compute delta time between end and start point.
 */
template <typename TDuration = std::chrono::nanoseconds> class TEFNOUT_API Timer
{
  public:
    /// Callback type used when timer is stopped to report data
    /// https://stackoverflow.com/a/25848890
    using OnStopCallback = std::function<void(const TimerReport<TDuration> timerReport)>;

    explicit Timer(std::string timerId, OnStopCallback callback)
        : m_id{timerId}, m_startTime{}, m_callback(callback)
    {
        Start();
    }
    explicit Timer(std::string timerId) : m_id{timerId}, m_startTime{}, m_callback{}
    {
        // Default debug callback
        m_callback = [](const TimerReport<TDuration> report) -> void {
            std::cout << report.Start << "--->" << report.End << "(ID=" << report.Id
                      << ")" << std::endl;
        };
        Start();
    }

    ~Timer()
    {
        Stop();
    }

  private:
    std::string m_id;

    // Highest accuracy clock
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;

    // Callback
    OnStopCallback m_callback;

    void Start()
    {
        m_startTime = std::chrono::high_resolution_clock::now();
    }

    void Stop()
    {
        const std::chrono::time_point<std::chrono::high_resolution_clock> endTime =
            std::chrono::high_resolution_clock::now();

        long long startCount =
            std::chrono::duration_cast<TDuration>(m_startTime.time_since_epoch()).count();
        long long endCount =
            std::chrono::duration_cast<TDuration>(endTime.time_since_epoch()).count();

        // Delegate action to callback
        m_callback({startCount, endCount, m_id});
    }
};
} // namespace Utility
} // namespace Tefnout

#endif
