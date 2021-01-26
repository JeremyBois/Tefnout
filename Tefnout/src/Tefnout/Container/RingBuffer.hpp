#ifndef __RINGBUFFER__HPP
#define __RINGBUFFER__HPP

#include "TefnoutPCH.hpp" // Fetch <iterator> and <array>

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
    // Required alias for STL trait definition
    using size_type = std::size_t;
    using value_type = T;
    using reference = value_type &;
    using const_reference = const value_type &;

    typedef std::random_access_iterator_tag iterator_category;

private:
    // Alias for all available iterators
    template <typename TBufferPointerType> struct RingIterator;

    using iterator_type = RingIterator<Ring<T, TCapacity>>;
    using const_iterator_type = RingIterator<const Ring<T, TCapacity>>;
    using reverse_iterator_type = std::reverse_iterator<iterator_type>;
    using const_reverse_iterator_type = std::reverse_iterator<const_iterator_type>;

public:
    // Using 1 for initial value of head removes the need to
    // test at each insertion if buffer is empty
    Ring() : m_head(0), m_tail(0), m_pendingSize(0)
    {
    }

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

    Response TryPush(T obj)
    {
        auto response = Response::Unknown;

        if (m_pendingSize != TCapacity)
        {
            m_container[m_tail] = obj;
            UpdateTailAndSize();
            response = Response::Ok;
        }
        else
        {
            // Do nothing
            TEFNOUT_ERROR("Overflow of Ring occurs (TCapacity = {0})", TCapacity);
            response = Response::Overflow;
        }

        return response;
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

    bool IsFull()
    {
        return m_pendingSize == TCapacity;
    }

    bool IsEmpty()
    {
        return m_pendingSize == 0;
    }

    bool IsPending()
    {
        return !IsEmpty();
    }

    void Clear()
    {
        m_container.clear();
        m_head = 0;
        m_tail = 0;
        m_pendingSize = 0;
    }

    // Be able to create iterator for our type
    iterator_type begin()
    {
        return RingIterator(this, m_head, 0);
    }
    iterator_type end()
    {
        return RingIterator(this, m_head, m_pendingSize);
    }

    const_iterator_type cbegin() const
    {
        return RingIterator(this, m_head, 0);
    }
    const_iterator_type cend() const
    {
        return RingIterator(this, m_head, m_pendingSize);
    }

    reverse_iterator_type rbegin()
    {
        return RingIterator(this, m_tail, 0);
    }
    reverse_iterator_type rend()
    {
        return RingIterator(this, m_tail, m_pendingSize);
    }

    const_reverse_iterator_type crbegin() const
    {
        return RingIterator(this, m_tail, 0);
    }
    const_reverse_iterator_type crend() const
    {
        return RingIterator(this, m_tail, m_pendingSize);
    }

  private:
    // Thin wrapper around a raw array without overhead
    std::array<T, TCapacity> m_container;
    size_type m_head;
    size_type m_tail;
    size_type m_pendingSize;

    reference operator[](size_type index)
    {
        auto indexInBounds = index % TCapacity;
        return m_container[indexInBounds];
    }

    const_reference &operator[](size_type index) const
    {
        const Ring<T, TCapacity>& itCons = *this;
        auto indexInBounds = index % TCapacity;
        return const_cast<reference>(itCons[indexInBounds]);
        // return const_cast<reference>(m_container[indexInBounds]);
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

    /**
     * @brief      Iterator implementation for Ring as a bidirectionnal iterator.
     */
    template <typename TBufferPointerType = Ring<T, TCapacity>>
    struct RingIterator
    {
        // STL friendly for optimization and algorithm selection using Traits
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using const_pointer = const value_type *;

        RingIterator(TBufferPointerType *ptr, size_type head, size_type delta)
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
        friend bool operator >(const RingIterator &a, const RingIterator &b)
        {
            return !a->operator<=(b);
        }
        friend bool operator>=(const RingIterator &a, const RingIterator &b)
        {
            return !a->operator<(b);
        }

      private:
        TBufferPointerType *buffer_ptr;
        size_type m_head;
        size_type m_delta;
    };
};
} // namespace Buffer
} // namespace Tefnout
// #include "Ring.hxx"

#endif
