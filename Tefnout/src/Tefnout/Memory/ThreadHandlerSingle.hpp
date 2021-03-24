#ifndef __THREADHANDLERSINGLE__HPP
#define __THREADHANDLERSINGLE__HPP

namespace Tefnout
{
namespace Memory
{
/**
 * @brief      Dummy thread handler that only support sequential access (a single thread).
 */
class ThreadHandlerSingle
{
  public:
    ThreadHandlerSingle() = default;
    ~ThreadHandlerSingle() = default;

    inline void Lock() const {};
    inline void UnLock() const {};
};
} // namespace Memory
} // namespace Tefnout

#endif
