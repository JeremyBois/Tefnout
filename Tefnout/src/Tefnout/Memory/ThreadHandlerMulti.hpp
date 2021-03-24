#ifndef __THREADHANDLERMULTI__HPP
#define __THREADHANDLERMULTI__HPP

#include "Tefnout/Core/Core.hpp"

#include <atomic>
#include <mutex>

namespace Tefnout
{
namespace Memory
{

/**
 * @brief      A wrapper class that can be used with any mutex.
 *
 * @note       Mutex is unlock in destructor if needed to avoid deadlocks.
 *
 * @tparam     MutexType  Mutex type
 */
template <typename MutexType> class TEFNOUT_API ThreadHandlerMutex
{
  public:
    ThreadHandlerMutex() : m_mutex{}, m_guard{m_mutex, std::defer_lock}
    {
    }

    ~ThreadHandlerMutex()
    {
        if (m_guard.owns_lock())
        {
            m_guard.unlock();
        }
    }

    inline void Lock()
    {
        m_guard.Lock();
    }

    inline void UnLock()
    {
        m_guard.Unlock();
    };

  private:
    MutexType m_mutex;
    std::unique_lock<MutexType> m_guard;
};

/**
 * @brief      A lock-free spinlock implemented using std::atomic_flag.
 */
class TEFNOUT_LOCAL ThreadHandlerSpinLock
{
  public:
    ThreadHandlerSpinLock() = default;
    ~ThreadHandlerSpinLock() = default;

    // Copy not allowed
    ThreadHandlerSpinLock(const ThreadHandlerSpinLock &that) = delete;
    ThreadHandlerSpinLock &operator=(ThreadHandlerSpinLock that) = delete;

    // Move not allowed
    ThreadHandlerSpinLock(const ThreadHandlerSpinLock &&that) = delete;

    inline void Lock()
    {
        // Lock free but not wait free
        // Avoid context switching by burning CPU cycles
        // Perform both read and write in one atomic operation
        while (m_flag.test_and_set(std::memory_order_release))
        {
        };
    };
    inline void UnLock()
    {
        m_flag.clear(std::memory_order_release);
    };

  private:
    // Lock free on any platform
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

} // namespace Memory
} // namespace Tefnout

#endif
