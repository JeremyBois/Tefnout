#ifndef __MEMORYHANDLER__HPP
#define __MEMORYHANDLER__HPP

#include "Tefnout/Core/Core.hpp"
#include <bits/c++config.h>
#include <cassert>

/* Resources
 * https://nfrechette.github.io/2015/05/21/linear_allocator/
 * https://blog.molecular-matters.com
 * https://bitsquid.blogspot.com/2010/09/custom-memory-allocation-in-c.html
 * https://accu.org/conf-docs/PDFs_2008/Alexandrescu-memory-allocation.screen.pdf
 */

// @TODO Add bound checking requirements and implement one.

namespace Tefnout
{
namespace Memory
{

// Forward declaration of policy constraints
// Both defined using sfinae or type constraints
template <typename T> struct IsAllocator;
template <typename T> struct IsMemoryTracker;
template <typename T> struct IsBoundsChecker;
template <typename T> struct IsThreadHandler;
template <typename T> struct IsMemoryBlock;

/**
 * @brief      This is the host class that can be used to implement different kind of
 *             memory handler using different combinaisons of Policy. Memory can be
 *             reserved on the stack or on the heap depending on constructor memory
 *             location class used.
 * @note       Required interface for each policy is inforced at compilation time.
 *
 * @tparam     AllocatorPolicy       Implement the memory allocation.
 * @tparam     ThreadHandlingPolicy  Implement the multi-threading access protections.
 * @tparam     BoundsCheckingPolicy  Implement checking capabilities of the handler.
 * @tparam     MemoryTrackingPolicy  Implement tracking capabilities of the handler.
 */
template <typename AllocatorPolicy, typename ThreadHandlingPolicy, typename BoundsCheckingPolicy,
          typename MemoryTrackingPolicy>
class TEFNOUT_API Handler
{
  public:
    // Make refactoring easier
    using size_type = std::size_t;
    using const_size_type = const std::size_t;

    template <typename MemoryBlockPolicy>
    explicit Handler(const MemoryBlockPolicy& reservedMemory)
        : m_allocator(reservedMemory.GetStart(), reservedMemory.GetEnd())
    {
        // Checks policies at compile time
        IsAllocator<AllocatorPolicy>{};
        IsMemoryTracker<MemoryTrackingPolicy>{};
        IsBoundsChecker<BoundsCheckingPolicy>{};
        IsThreadHandler<ThreadHandlingPolicy>{};
        IsMemoryBlock<MemoryBlockPolicy>{};
    }

    ~Handler() = default;

    // Interface
    void* Allocate(size_type size, size_type alignment, const char* description)
    {
        m_threadHandler.Lock();

        // Calculate the needed memory to be allocated
        const_size_type originalSize = size;

        // Front and Back of checker can be used to check if memory still valid
        const_size_type sizeWithBounds =
            size + BoundsCheckingPolicy::s_sizeFront + BoundsCheckingPolicy::s_sizeBack;

        char* allocatedMemory = static_cast<char*>(
            m_allocator.Allocate(sizeWithBounds, alignment, BoundsCheckingPolicy::s_sizeFront));

        // @TODO add SetupFront and SetupBack in BoundsCheckerPolicy

        // Pass allocated memory with its size and requested size
        // sizeWithBounds depends on BoundsChecker implementation
        m_memoryTracker.OnAllocation(allocatedMemory, sizeWithBounds, originalSize, description);

        m_threadHandler.UnLock();

        // Return a pointer starting at requested memory excluding boundchecker if exists
        return (allocatedMemory + BoundsCheckingPolicy::s_sizeFront);
    }

    void Free(void* objPtr)
    {
        m_threadHandler.Lock();

        // Account for bounds do get CorrectType address
        char* originalMemory = static_cast<char*>(objPtr) - BoundsCheckingPolicy::s_sizeFront;

        // @TODO add CheckFront and CheckBack in BoundsCheckerPolicy

        // Allow tracker to do what it want with memory before freeing it
        m_memoryTracker.OnFree(originalMemory);
        m_allocator.Free(objPtr);

        m_threadHandler.UnLock();
    }

    const AllocatorPolicy& GetAllocator() const
    {
        return m_allocator;
    };

  private:
    AllocatorPolicy m_allocator;
    BoundsCheckingPolicy m_boundsChecker;
    MemoryTrackingPolicy m_memoryTracker;
    ThreadHandlingPolicy m_threadHandler;
};

template <typename T, typename TMemoryHandler, typename... Args>
T* MakeNew(TMemoryHandler& handler, const char* description, Args&&... args)
{
    // Memory is allocated by the handler NOT the new operator
    // Using new placement operator not the operator new
    return new (handler.Allocate(sizeof(T), std::alignment_of<T>::value, description)) T(args...);
}

template <typename T, class TMemoryHandler> void MakeDelete(T* objectPtr, TMemoryHandler& handler)
{
    if (nullptr != objectPtr)
    {
        // Because delete operator will never be called
        // then destructor must be called manually
        objectPtr->~T();

        handler.Free(objectPtr);
    }
}

// @TEST Template type constraints
template <typename T> class AllocatorPolicyTester
{
  private:
    // Matching type for sizeof
    typedef char CorrectType;
    // Following must hold
    // sizeof(IncorrectType) != sizeof(CorrectType)
    struct IncorrectType
    {
        char dummy[2];
    };

    template <typename C> static CorrectType Allocate(decltype(&C::Allocate));
    template <typename C> static IncorrectType Allocate(...);

    template <typename C> static CorrectType Free(decltype(&C::Free));
    template <typename C> static IncorrectType Free(...);

    template <typename C> static CorrectType GetAllocationSize(decltype(&C::GetAllocationSize));
    template <typename C> static IncorrectType GetAllocationSize(...);

  public:
    enum
    {
        HasAllocate = sizeof(Allocate<T>(0)) == sizeof(char),
        HasFree = sizeof(Free<T>(0)) == sizeof(char),
        HasGetAllocationSize = sizeof(GetAllocationSize<T>(0)) == sizeof(char)
    };
};

/**
 * @brief      Compile time policy check for Handler::AllocatorPolicy
 *
 * @tparam     T     Type to be tested at compile time.
 */
template <typename T> struct IsAllocator
{
    static void constraints(T a)
    {
        // Only check is method exists not if signature mathches
        static_assert(AllocatorPolicyTester<T>::HasAllocate,
                      "AllocatorPolicy failure - Missing Allocate method");
        static_assert(AllocatorPolicyTester<T>::HasFree,
                      "AllocatorPolicy failure - Missing Free method");
        static_assert(AllocatorPolicyTester<T>::HasGetAllocationSize,
                      "AllocatorPolicy failure - Missing GetAllocationSize method");
    }
    IsAllocator()
    {
        void (*p)(T) = constraints;
    } // NOLINT
};

template <typename T> class TrackerPolicyTester
{
  private:
    // Matching type for sizeof
    typedef char CorrectType;
    // Following must hold
    // sizeof(IncorrectType) != sizeof(CorrectType)
    struct IncorrectType
    {
        char dummy[2];
    };

    template <typename C> static CorrectType OnAllocation(decltype(&C::OnAllocation));
    template <typename C> static IncorrectType OnAllocation(...);

    template <typename C> static CorrectType OnFree(decltype(&C::OnFree));
    template <typename C> static IncorrectType OnFree(...);

  public:
    enum
    {
        HasOnAllocation = sizeof(OnAllocation<T>(0)) == sizeof(char),
        HasOnFree = sizeof(OnFree<T>(0)) == sizeof(char),
    };
};

/**
 * @brief      Compile time policy check for Handler::MemoryTrackingPolicy
 *
 * @tparam     T     Type to be tested at compile time.
 */
template <typename T> struct IsMemoryTracker
{
    static void constraints(T a)
    {
        static_assert(TrackerPolicyTester<T>::HasOnAllocation,
                      "MemoryTracking Policy failure - Missing OnAllocation method");
        static_assert(TrackerPolicyTester<T>::HasOnFree,
                      "MemoryTracking Policy failure - Missing OnFree method");
    }
    IsMemoryTracker()
    {
        void (*p)(T) = constraints;
    } // NOLINT
};

/**
 * @brief      Compile time policy check for Handler::BoundsCheckingPolicy
 *
 * @tparam     T     Type to be tested at compile time.
 */
template <typename T> struct IsBoundsChecker
{
    static void constraints(T)
    {
        static_assert(std::is_same<decltype(T::s_sizeFront), decltype(T::s_sizeBack)>::value,
                      "BoundsChecking Policy failure");
    }
    IsBoundsChecker()
    {
        void (*p)(T) = constraints;
    } // NOLINT
};

template <typename T> class ThreadPolicyTester
{
  private:
    // Matching type for sizeof
    typedef char CorrectType;
    // Following must hold
    // sizeof(IncorrectType) != sizeof(CorrectType)
    struct IncorrectType
    {
        char a[2];
    };

    template <typename C> static CorrectType Lock(decltype(&C::Lock));
    template <typename C> static IncorrectType Lock(...);

    template <typename C> static CorrectType UnLock(decltype(&C::UnLock));
    template <typename C> static IncorrectType UnLock(...);

  public:
    enum
    {
        HasLock = sizeof(Lock<T>(0)) == sizeof(char),
        HasUnLock = sizeof(UnLock<T>(0)) == sizeof(char)
    };
};

/**
 * @brief      Compile time policy check for Handler::BoundsCheckingPolicy
 *
 * @tparam     T     Type to be tested at compile time.
 */
template <typename T> struct IsThreadHandler
{
    static void constraints(T a)
    {
        static_assert(ThreadPolicyTester<T>::HasUnLock,
                      "ThreadHandling Policy failure - Missing UnLock method");
        static_assert(ThreadPolicyTester<T>::HasLock,
                      "ThreadHandling Policy failure - Missing Lock method");
    }
    IsThreadHandler()
    {
        void (*p)(T) = constraints;
    } // NOLINT
};

template <typename T> class BlockPolicyTester
{
  private:
    // Matching type for sizeof
    typedef char CorrectType;
    // Following must hold
    // sizeof(IncorrectType) != sizeof(CorrectType)
    struct IncorrectType
    {
        char dummy[2];
    };

    template <typename C> static CorrectType GetStart(decltype(&C::GetStart));
    template <typename C> static IncorrectType GetStart(...);

    template <typename C> static CorrectType GetEnd(decltype(&C::GetEnd));
    template <typename C> static IncorrectType GetEnd(...);

  public:
    enum
    {
        HasGetStart = sizeof(GetStart<T>(0)) == sizeof(char),
        HasGetEnd = sizeof(GetEnd<T>(0)) == sizeof(char)
    };
};

template <typename T> struct IsMemoryBlock
{
    static void constraints(T a)
    {
        static_assert(BlockPolicyTester<T>::HasGetStart,
                      "MemoryBlock Policy failure - Missing GetStart method");
        static_assert(BlockPolicyTester<T>::HasGetEnd,
                      "MemoryBlock Policy failure - Missing GetEnd method");
    }
    IsMemoryBlock()
    {
        void (*p)(T) = constraints;
    } // NOLINT
};
} // namespace Memory
} // namespace Tefnout

#endif
