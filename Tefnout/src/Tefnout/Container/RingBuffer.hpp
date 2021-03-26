#ifndef __RINGBUFFER__HPP
#define __RINGBUFFER__HPP

#include "Tefnout/Core/Core.hpp"

#include <type_traits>
#include <utility>

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

template <typename T, std::size_t capacity, bool isConst> struct TEFNOUT_API RingIterator;

/**
 * @brief      A fixed capacity container keeping avantages of an array but also allowing to
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
 * @tparam     capacity  Maximal container capacity
 */
template <typename T, std::size_t capacity> class TEFNOUT_API Ring
{
    static_assert(std::is_move_constructible_v<T> && std::is_move_assignable_v<T>,
                  "The managed type must be at least move constructible and assignable");

  public:
    // Required alias for STL trait definition
    using size_type = std::size_t;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Alias for all available iterators
    using iterator = RingIterator<T, capacity, false>;
    using const_iterator = RingIterator<T, capacity, true>;
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
        return std::move(m_container[old_head]);
    }

    /**
     * @brief      Safe way to add a new item to the queue. Only added if not already
     *             full.
     *
     * @param[in]  item  The item to enqueue
     *
     * @return     Buffer response indicating the operation result (@ref Response)
     */
    Response TryPush(const T& item)
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
            TEFNOUT_ERROR("Nothing pushed to avoid an overflow (capacity = {0})", capacity);
            response = Response::Abort;
        }

        return response;
    }

    /**
     * @brief      Safe way to add a new item to the queue. Only added if not already
     *             full.
     *
     * @param[in]  item  The item to enqueue
     *
     * @return     Buffer response indicating the operation result (@ref Response)
     */
    Response TryPush(T&& item)
    {
        auto response = Response::Unknown;

        if (!IsFull())
        {
            UpdateTailAndSize();
            response = Response::Ok;
            m_container[m_tail] = std::move(item);
            // TEFNOUT_DEBUG("tail={1} head={2} (pending = {0})", m_pendingSize, m_tail, m_head);
        }
        else
        {
            // Do nothing
            TEFNOUT_ERROR("Nothing pushed to avoid an overflow (capacity = {0})", capacity);
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
    Response Push(const T& item)
    {
        auto response = PreparePush();

        m_container[m_tail] = item;
        // TEFNOUT_DEBUG("tail={1} head={2} (pending = {0})", m_pendingSize, m_tail, m_head);

        return response;
    }

    Response Push(T&& item)
    {
        auto response = PreparePush();

        m_container[m_tail] = std::move(item);
        // TEFNOUT_DEBUG("tail={1} head={2} (pending = {0})", m_pendingSize, m_tail, m_head);

        return response;
    }

    /**
     * @brief      Emplace a new item to the buffer. Avoid creating the object on the
     *             caller function stackframe by directly created it in buffer storage.
     *
     * @param      args  The arguments used to build the object
     *
     * @tparam     Args  Variadic arguments used to build a type instance
     *
     * @return     Newly created and added instance
     */
    template <typename... Args> T& Emplace(Args&&... args)
    {
        auto response = PreparePush();

        m_container[m_tail] = T(std::forward<Args>(args)...);

        return m_container[m_tail];
    }

    constexpr size_type Capacity() const
    {
        return capacity;
    }

    /**
     * @brief      Determines the number of queued items.
     *
     * @return     The number of item inside the queue.
     */
    size_type Size() const
    {
        return m_pendingSize;
    }

    /**
     * @brief      Determines if full.
     *
     * @return     True if full, False otherwise.
     */
    bool IsFull() const
    {
        return m_pendingSize == capacity;
    }

    /**
     * @brief      Determines if empty.
     *
     * @return     True if empty, False otherwise.
     */
    bool IsEmpty() const
    {
        return m_pendingSize == s_Zero;
    }

    /**
     * @brief      Determines if there is some pending item in the queue.
     *
     * @return     True if pending, False otherwise.
     */
    bool IsPending() const
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
        auto indexInBounds = index % capacity;
        return m_container[indexInBounds];
    }

    const_reference& operator[](size_type index) const
    {
        auto indexInBounds = index % capacity;
        return m_container[indexInBounds];
    }

  private:
    // Thin wrapper around a raw array without overhead
    std::array<T, capacity> m_container;
    size_type m_head;
    size_type m_tail;
    size_type m_pendingSize;

    // Avoid casting from int to size_type each time
    static const size_type s_One = size_type(1);
    static const size_type s_Zero = size_type(0);

    inline void UpdateHeadAndSize()
    {
        m_head = (m_head + s_One) % capacity;
        --m_pendingSize;
    }

    inline void UpdateTailAndSize()
    {
        m_tail = (m_tail + s_One) % capacity;
        ++m_pendingSize;
    }

    Response PreparePush()
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
            TEFNOUT_ERROR("Overflow of Ring occurs (capacity = {0})", capacity);
            response = Response::Overflow;
        }

        return response;
    }
};

/**
 * @brief      Iterator implementation for Ring as a bidirectionnal (mutable or constant)
 *             iterator.
 *
 * @tparam     T         Type of items inside the container.
 * @tparam     capacity  Capacity of the container.
 * @tparam     isConst   Version of the iterator to create : true for a constant, false
 *                       for a mutable (default).
 */
template <typename T, std::size_t capacity, bool isConst = false> struct TEFNOUT_API RingIterator
{
  public:
    // Iterator traits
    // STL friendly for optimization and algorithm selection
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = T;

    // Const or non-const based on boolean isConst
    // https://stackoverflow.com/questions/2150192/how-to-avoid-code-duplication-implementing-const-and-non-const-iterators
    // isConst == true  --> const iterator
    // isConst == false --> iterator
    using pointer = typename std::conditional_t<isConst, value_type const*, value_type*>;
    using reference = typename std::conditional_t<isConst, value_type const&, value_type&>;

    // Use flag to deduce correct type (const or non const buffer pointer version)
    // Allow to remove explicit type in template argument
    using buffer_pointer_type =
        typename std::conditional_t<isConst, const Ring<T, capacity>*, Ring<T, capacity>*>;

    explicit RingIterator(buffer_pointer_type ptr, size_type head, size_type delta)
        : m_buffer_ptr(ptr), m_head(head), m_delta(delta)
    {
    }

    ~RingIterator() = default;

    // Allow conversion from non-const to const but not the other way arround
    // preserving trivial construction
    // More at https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
    template <bool WasConst, class = std::enable_if_t<isConst || !WasConst>>
    RingIterator(const RingIterator<T, capacity, WasConst>& rhs)
        : m_buffer_ptr(rhs.m_buffer_ptr), m_head(rhs.m_head), m_delta(rhs.m_delta)
    {
    }

    // @NOTE Const or non const based on template isConst
    reference operator*()
    {
        // Item pointer
        return (*m_buffer_ptr)[m_head + m_delta];
    }

    // @NOTE Const or non const based on template isConst
    pointer operator->()
    {
        // Reference of item pointer
        return &(operator*());
    }

    // Prefix increment
    RingIterator& operator++()
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
    RingIterator& operator--()
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
    friend bool operator==(const RingIterator& a, const RingIterator& b)
    {
        return (a.m_head + a.m_delta) == (b.m_head + b.m_delta);
    };
    friend bool operator!=(const RingIterator& a, const RingIterator& b)
    {
        return (a.m_head + a.m_delta) != (b.m_head + b.m_delta);
    };

    friend bool operator<(const RingIterator& a, const RingIterator& b)
    {
        return (a.m_head + a.m_pendingSize < b.m_head + b.m_pendingSize);
    }
    friend bool operator<=(const RingIterator& a, const RingIterator& b)
    {
        return (a.m_head + a.m_pendingSize <= b.m_head + b.m_pendingSize);
    }
    friend bool operator>(const RingIterator& a, const RingIterator& b)
    {
        return !a->operator<=(b);
    }
    friend bool operator>=(const RingIterator& a, const RingIterator& b)
    {
        return !a->operator<(b);
    }

  private:
    buffer_pointer_type m_buffer_ptr;
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
