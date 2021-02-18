#ifndef __RINGBUFFER__HPP
#define __RINGBUFFER__HPP

#include "Tefnout/Core/Core.hpp"

#include <type_traits>

namespace Tefnout
{
namespace Buffer
{

// @REFERENCE
//      - https://gameprogrammingpatterns.com/event-queue.html#a-ring-buffer
//
//      https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators
//      - https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
//      - https://stackoverflow.com/questions/3899223/what-is-a-non-trivial-constructor-in-c
// @REFERENCE

enum class TEFNOUT_API Response : uint32_t
{
    Unknown = 0,
    Ok = 1,
    Overflow = 2,
    Abort = 3
};

template <typename T, std::size_t TCapacity, bool TFlagConst> struct TEFNOUT_API RingIterator;

/**
 * @brief      A fixed size container keeping avantages of an array but also allowing to
 *             queue element without any shifting (as with a linked list) using a cyclic
 *             array as described in "Game programming patterns"
 *             (https://gameprogrammingpatterns.com/event-queue.html#a-ring-buffer).
 *
 *             Items are added to the head (Front) until the tail (Back). Item are removed
 *             in a FIFO manner.
 *
 *             Looping over this structure will move from head to tail (forward) and tail
 *             to head (backward)
 *
 * @note       Items can also be randomly accessed but without any guarantee that asked
 *             index points to a valid memory address.
 *
 * @tparam     T          Type of stored elements
 * @tparam     TCapacity  Maximal container capacity
 */
template <typename T, std::size_t TCapacity> class TEFNOUT_API Ring
{
  public:
    // Required alias for STL trait definition
    using size_type = std::size_t;
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;

    // Alias for all available iterators
    using iterator = RingIterator<T, TCapacity, false>;
    using const_iterator = RingIterator<T, TCapacity, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    explicit Ring() : m_container({}), m_head(s_One), m_tail(s_Zero), m_pendingSize(s_Zero)
    {
    }

    /**
     * @brief      Get reference of the head item.
     *
     * @return     Const reference of the first item (head)
     */
    const_reference Front() const
    {
        return m_container[m_head];
    }

    /**
     * @brief      Get reference of the tail item
     *
     * @return     Const reference of the last item (tail)
     */
    const_reference Back() const
    {
        return m_container[m_tail];
    }

    /**
     * @brief      Get reference of the head item.
     *
     * @return     Mutable reference of the first item (head)
     */
    reference Front()
    {
        return m_container[m_head];
    }

    /**
     * @brief      Get reference of the tail item
     *
     * @return     Mutable reference of the last item (tail)
     */
    reference Back()
    {
        return m_container[m_tail];
    }

    /**
     * @brief      Pops oldest item from the queue.
     *
     * @return     The oldest item
     */
    value_type Pop()
    {
        auto old_head = m_head;
        UpdateHeadAndSize();
        return m_container[old_head];
    }

    /**
     * @brief      Safe way to add a new item to the queue. Only added if not already
     *             full.
     *
     * @param[in]  item  The item to enqueue
     *
     * @return     Buffer response indicating the operation result (@ref Response)
     */
    Response TryPush(T item)
    {
        auto response = Response::Unknown;

        if (!IsFull())
        {
            UpdateTailAndSize();
            response = Response::Ok;
            m_container[m_tail] = item;
            // TEFNOUT_DEBUG("tail={1} head={2} (pending = {0})", m_pendingSize, m_tail, m_head);
        }
        else
        {
            // Do nothing
            TEFNOUT_ERROR("Nothing pushed to avoid an overflow (TCapacity = {0})", TCapacity);
            response = Response::Abort;
        }

        return response;
    }

    /**
     * @brief      Add a new item to the queue. In case of overflow, the oldest item is
     *             discarded.
     * @see        @ref TryPush for a safer version.
     *
     * @param[in]  item  The item to enqueue
     *
     * @return     Buffer response indicating the operation result (@ref Response)
     */
    Response Push(T item)
    {
        auto response = Response::Unknown;

        if (!IsFull())
        {
            UpdateTailAndSize();
            response = Response::Ok;
        }
        else
        {
            // Advance both due to overflow
            // Oldest item is lost
            UpdateHeadAndSize();
            UpdateTailAndSize();
            TEFNOUT_ERROR("Overflow of Ring occurs (TCapacity = {0})", TCapacity);
            response = Response::Overflow;
        }

        m_container[m_tail] = item;
        // TEFNOUT_DEBUG("tail={1} head={2} (pending = {0})", m_pendingSize, m_tail, m_head);

        return response;
    }

    /**
     * @brief      Determines the number of queued items.
     *
     * @return     The number of item inside the queue.
     */
    size_type Size()
    {
        return m_pendingSize;
    }

    /**
     * @brief      Determines if full.
     *
     * @return     True if full, False otherwise.
     */
    bool IsFull()
    {
        return m_pendingSize == TCapacity;
    }

    /**
     * @brief      Determines if empty.
     *
     * @return     True if empty, False otherwise.
     */
    bool IsEmpty()
    {
        return m_pendingSize == s_Zero;
    }

    /**
     * @brief      Determines if there is some pending item in the queue.
     *
     * @return     True if pending, False otherwise.
     */
    bool IsPending()
    {
        return !IsEmpty();
    }

    /**
     * @brief      Reset the whole queue to initial state.
     */
    void Clear()
    {
        m_container.clear();
        m_head = s_One;
        m_tail = s_Zero;
        m_pendingSize = s_Zero;
    }

    // Be able to create iterator for our type
    iterator begin()
    {
        return iterator(this, m_head, s_Zero);
    }
    iterator end()
    {
        return iterator(this, m_head, m_pendingSize);
    }

    const_iterator cbegin() const
    {
        return const_iterator(this, m_head, s_Zero);
    }
    const_iterator cend() const
    {
        return const_iterator(this, m_head, m_pendingSize);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }
    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(cend());
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(cbegin());
    }

    reference operator[](size_type index)
    {
        auto indexInBounds = index % TCapacity;
        return m_container[indexInBounds];
    }

    const_reference &operator[](size_type index) const
    {
        auto indexInBounds = index % TCapacity;
        return m_container[indexInBounds];
    }

  private:
    // Thin wrapper around a raw array without overhead
    std::array<T, TCapacity> m_container;
    size_type m_head;
    size_type m_tail;
    size_type m_pendingSize;

    // Avoid casting from int to size_type each time
    static const size_type s_One = size_type(1);
    static const size_type s_Zero = size_type(0);

    inline void UpdateHeadAndSize()
    {
        m_head = (m_head + s_One) % TCapacity;
        --m_pendingSize;
    }

    inline void UpdateTailAndSize()
    {
        m_tail = (m_tail + s_One) % TCapacity;
        ++m_pendingSize;
    }
};

/**
 * @brief      Iterator implementation for Ring as a bidirectionnal (mutable or constant)
 *             iterator.
 *
 * @tparam     T           Type of items inside the container.
 * @tparam     TCapacity   Size of the container.
 * @tparam     TFlagConst  Version of the iterator to create : true for a constant, false
 *                         for a mutable (default).
 */
template <typename T, std::size_t TCapacity, bool TFlagConst = false> struct TEFNOUT_API RingIterator
{
  public:
    // Iterator traits
    // STL friendly for optimization and algorithm selection
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = T;

    // Const or non-const based on boolean TFlagConst
    // https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators
    // TFlagConst == true  --> const iterator
    // TFlagConst == false --> iterator
    using pointer = typename std::conditional_t<TFlagConst, value_type const *, value_type *>;
    using reference = typename std::conditional_t<TFlagConst, value_type const &, value_type &>;

    // Use flag to deduce correct type (const or non const buffer pointer version)
    // Allow to remove explicit type in template argument
    using buffer_pointer_type =
        typename std::conditional_t<TFlagConst, const Ring<T, TCapacity> *, Ring<T, TCapacity> *>;

    explicit RingIterator(buffer_pointer_type ptr, size_type head, size_type delta)
        : buffer_ptr(ptr), m_head(head), m_delta(delta)
    {
    }

    ~RingIterator() = default;

    // Allow conversion from non-const to const but not the other way arround
    // preserving trivial construction
    // More at https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
    template <bool WasConst, class = std::enable_if_t<TFlagConst || !WasConst>>
    RingIterator(const RingIterator<T, TCapacity, WasConst> &rhs)
        : buffer_ptr(rhs.buffer_ptr), m_head(m_head), m_delta(m_delta)
    {
    }

    // @NOTE Const or non const based on template TFlagConst
    reference operator*()
    {
        // Item pointer
        return (*buffer_ptr)[m_head + m_delta];
    }

    // @NOTE Const or non const based on template TFlagConst
    pointer operator->()
    {
        // Reference of item pointer
        return &(operator*());
    }

    // Prefix increment
    RingIterator &operator++()
    {
        ++m_delta;
        return *this;
    }

    // Postfix increment
    RingIterator operator++(int)
    {
        RingIterator tmp = *this;
        ++(*this); // Delegate to prefix increment
        return tmp;
    }

    // Prefix decrement
    RingIterator &operator--()
    {
        --m_delta;
        return *this;
    }

    // Postfix decrement
    RingIterator operator--(int)
    {
        RingIterator tmp = *this;
        --(*this); // Delegate to prefix decrement
        return tmp;
    }

    // friend allows to declare this operator as non-member
    // but still getting access to private fields in implementation
    friend bool operator==(const RingIterator &a, const RingIterator &b)
    {
        return (a.m_head + a.m_delta) == (b.m_head + b.m_delta);
    };
    friend bool operator!=(const RingIterator &a, const RingIterator &b)
    {
        return (a.m_head + a.m_delta) != (b.m_head + b.m_delta);
    };

    friend bool operator<(const RingIterator &a, const RingIterator &b)
    {
        return (a.m_head + a.m_pendingSize < b.m_head + b.m_pendingSize);
    }
    friend bool operator<=(const RingIterator &a, const RingIterator &b)
    {
        return (a.m_head + a.m_pendingSize <= b.m_head + b.m_pendingSize);
    }
    friend bool operator>(const RingIterator &a, const RingIterator &b)
    {
        return !a->operator<=(b);
    }
    friend bool operator>=(const RingIterator &a, const RingIterator &b)
    {
        return !a->operator<(b);
    }

  private:
    buffer_pointer_type buffer_ptr;
    size_type m_head;
    size_type m_delta;
};

// @TEST - AVOID REGRESSIONS
// Assert we CAN convert from const to const iterator
static_assert(std::is_convertible_v<Ring<int, 5>::const_iterator, Ring<int, 5>::const_iterator>);
// Assert we CAN convert from non-const to non-const iterator
static_assert(std::is_convertible_v<Ring<int, 5>::iterator, Ring<int, 5>::iterator>);

// Assert we CAN convert from non-const to const iterator
static_assert(std::is_convertible_v<Ring<int, 5>::iterator, Ring<int, 5>::const_iterator>);
// Assert we CANNOT convert from const to non-const iterator
static_assert(not std::is_convertible_v<Ring<int, 5>::const_iterator, Ring<int, 5>::iterator>);

// Both const and non-const construction are trivial
static_assert(std::is_trivially_copy_constructible_v<Ring<int, 5>::const_iterator>);
static_assert(std::is_trivially_copy_constructible_v<Ring<int, 5>::iterator>);
// @TEST - AVOID REGRESSIONS

} // namespace Buffer
} // namespace Tefnout

#endif
