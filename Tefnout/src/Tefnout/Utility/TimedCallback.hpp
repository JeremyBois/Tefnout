#ifndef __TIMEDCALLBACK__HPP
#define __TIMEDCALLBACK__HPP

#include "Tefnout/Core/Platform.hpp"
#include <atomic>
#include <functional>
#include <thread>

// @NOTE https://bartoszmilewski.com/2008/12/01/c-atomics-and-memory-ordering/
// @NOTE https://www.cprogramming.com/c++11/c++11-lambda-closures.html

namespace Tefnout
{
namespace Utility
{
/**
 * @brief      Manage a callback that should be called every x times (cyclically). Atomic is managed
 *             using lower level of memory fence that can work on any major processor but
 *             will be optimized (no fence will be used) on X86.
 */
class TEFNOUT_LOCAL TimedCallback
{
  private:
    std::atomic<bool> isActive;
    std::thread internalThread;

  public:
    TimedCallback();
    ~TimedCallback();

    /**
     * @brief      Determines if a callback is currently running.
     *
     * @return     True if running, False otherwise.
     */
    bool IsRunning() const;

    /**
     * @brief      Start a cyclic call to @p func every @p cyclicStep milliseconds (ms).
     *             If there is already a function running it will be stopped before
     *             running the new one.
     *
     * @param[in]  func        Function cyclically called
     * @param[in]  cyclicStep  The cyclic step in milliseconds (ms)
     */
    void Start(std::function<void()> func, int cyclicStep);

    /**
     * @brief      Force associated callback to stop
     */
    void Stop();
};
} // namespace Utility
} // namespace Tefnout

#endif
