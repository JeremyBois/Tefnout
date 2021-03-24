#include "TimedCallback.hpp"

#include <chrono>


namespace Tefnout
{
namespace Utility
{
TimedCallback::TimedCallback() : isActive(false), internalThread{}
{
}

TimedCallback::~TimedCallback()
{
    if (isActive.load(std::memory_order_acquire))
    {
        Stop();
    };
}

void TimedCallback::Stop()
{
    isActive.store(false, std::memory_order_release);
    if (internalThread.joinable())
        internalThread.join();
}

void TimedCallback::Start(std::function<void()> func, int cyclicStep)
{
    if (isActive.load(std::memory_order_acquire))
    {
        Stop();
    };
    isActive.store(true, std::memory_order_release);

    internalThread = std::thread([this, cyclicStep, func]() {
        while (isActive.load(std::memory_order_acquire))
        {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(cyclicStep));
        }
    });
}

bool TimedCallback::IsRunning() const
{
    return (isActive.load(std::memory_order_acquire) && internalThread.joinable());
}
} // namespace Utility
} // namespace Tefnout
