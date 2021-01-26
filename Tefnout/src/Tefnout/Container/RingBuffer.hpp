#ifndef __RINGBUFFER__HPP
#define __RINGBUFFER__HPP

#include "TefnoutPCH.hpp"  // Fetch <iterator> and <array>

#include "Tefnout/Core/Logger.hpp"

namespace Tefnout
{
namespace Buffer
{

enum class Response : uint32_t
{
    Unknown = 0,
    Ok = 1,
    Overflow = 2
};

template <typename T, std::size_t TCapacity> class Ring
{
  public:
    // Make refactoring easier with STL friendly alias
    using value_type = T;
    using reference = value_type &;
    using pointer = value_type *;
    using const_reference = const value_type &;
    using const_pointer = const value_type *;

    using size_type = std::size_t;
    using container_type = std::array<T, TCapacity>;

    // Using 1 for initial value of head removes the need to
    // test at each insertion if buffer is empty
    Ring() : m_head(1), m_tail(0), m_pendingSize(0)
    {
    }

    // template <bool isconst = false> // For const or non const iterator

    /**
     * @brief      Iterator implementation for Ring as a bidirectionnal iterator.
     */
    struct RingIterator
    {
        // STL friendly for optimization and algorithm selection
        using iterator_category = std::forward_iterator_tag;
        // using difference_type = std::ptrdiff_t;

        RingIterator(Ring<T, TCapacity> *ptr, size_type head, size_type delta)
            : buffer_ptr(ptr), m_head(head), m_delta(delta)
        {
        }

        reference operator*()
        {
            // Item pointer
            return (*buffer_ptr)[m_head + m_delta];
        }
        pointer operator->()
        {
            // Reference of item pointer
            return &(operator*());
        }

        const_reference operator*() const
        {
            // Item pointer
            return (*buffer_ptr)[m_head + m_delta];
        }
        const_pointer operator->() const
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

        // @MAYBE random access
        // Needed for random access iterator
        // Currently not needed
        // RingIterator operator+(difference_type n)
        // {
        //     RingIterator tmp = *this;
        //     tmp.m_delta += n;
        //     return tmp;
        // }
        // RingIterator &operator+=(difference_type n)
        // {
        //     m_delta += n;
        //     return *this;
        // }
        // RingIterator operator-(difference_type n)
        // {
        //     RingIterator tmp = *this;
        //     tmp.m_delta -= n;
        //     return tmp;
        // }
        // RingIterator &operator-=(difference_type n)
        // {
        //     m_delta -= n;
        //     return *this;
        // }

        // friend allows to declare this operator as non-member
        // but still getting access to private fields in implémentation
        friend bool operator==(const RingIterator &a, const RingIterator &b)
        {
            return (a.m_head + a.m_delta) == (b.m_head + b.m_delta);
        };
        friend bool operator!=(const RingIterator &a, const RingIterator &b)
        {
            return (a.m_head + a.m_delta) != (b.m_head + b.m_delta);
        };

      private:
        Ring<T, TCapacity> *buffer_ptr;
        size_type m_head;
        size_type m_delta;
    };

    const_reference Front() const
    {
        return m_container[m_head];
    }

    const_reference Back() const
    {
        return m_container[m_tail];
    }

    reference Front()
    {
        return m_container[m_head];
    }

    reference Back()
    {
        return m_container[m_tail];
    }

    value_type Pop()
    {
        auto old_head = m_head;
        UpdateHeadAndSize();
        return m_container[old_head];
    }

    Response Push(T obj)
    {
        m_container[m_tail] = obj;
        auto response = Response::Unknown;

        if (m_pendingSize != TCapacity)
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

        TEFNOUT_DEBUG("tail={1} head={2} (pending = {0})", m_pendingSize, m_tail, m_head);

        return response;
    }

    bool IsItemPending()
    {
        return m_head != m_tail;
    }

    bool IsEmpty()
    {
        return m_head == m_tail == 0;
    }

    void Clear()
    {
        m_container.clear();
        m_head = 0;
        m_tail = 0;
    }

    // Be able to create iterator for our type
    RingIterator begin()
    {
        return RingIterator(this, m_head, 0);
    }
    RingIterator end()
    {
        return RingIterator(this, m_head, m_pendingSize);
    }

  private:
    // Thin wrapper around a raw array without overhead
    container_type m_container;
    size_type m_head;
    size_type m_tail;
    size_type m_pendingSize;

    T &operator[](size_type index)
    {
        auto indexInBounds = index % TCapacity;
        return m_container[indexInBounds];
    }

    const T &operator[](size_type index) const
    {
        auto indexInBounds = index % TCapacity;
        return m_container[indexInBounds];
    }

    inline void UpdateHeadAndSize()
    {
        m_head = (m_head + 1) % TCapacity;
        --m_pendingSize;
    }

    inline void UpdateTailAndSize()
    {
        m_tail = (m_tail + 1) % TCapacity;
        ++m_pendingSize;
    }
};
} // namespace Buffer
} // namespace Tefnout
// #include "Ring.hxx"

#endif
