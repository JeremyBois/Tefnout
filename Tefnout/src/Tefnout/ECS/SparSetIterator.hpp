#ifndef __SPARSETITERATOR__HPP
#define __SPARSETITERATOR__HPP

#include "Tefnout/Core/Platform.hpp"
#include <bits/c++config.h>
#include <iterator>
#include <type_traits>

namespace Tefnout
{
namespace ECS
{
/**
 * @brief      Iterator implementation for SparseSet as a random access (mutable or
 *             constant) iterator.
 *
 * @tparam     T        Type of items inside the container.
 * @tparam     isConst  Version of the iterator to create : true for a constant, false for
 *                      a mutable (default).
 */
template <typename T, bool isConst = false> struct TEFNOUT_API SparseSetIterator
{
  public:
    // Iterator traits
    // STL friendly for optimization and algorithm selection
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = T;

    // Const or non-const based on boolean isConst
    // isConst == true  --> const iterator
    // isConst == false --> iterator
    using pointer = typename std::conditional_t<isConst, const value_type*, value_type*>;
    using reference = typename std::conditional_t<isConst, const value_type&, value_type&>;

    explicit SparseSetIterator(pointer ptr, size_type index) : m_ptr(ptr), m_index(index)
    {
    }

    ~SparseSetIterator() = default;

    // Allow conversion from non-const to const but not the other way arround
    // preserving trivial construction
    // More at https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
    template <bool WasConst, class = std::enable_if_t<isConst || !WasConst>>
    SparseSetIterator(const SparseSetIterator<T, WasConst>& rhs)
        : m_ptr(rhs.m_ptr), m_index(rhs.m_index)
    {
    }

    // @NOTE Const or non const based on template isConst
    reference operator*()
    {
        // Item pointer
        return m_ptr[m_index];
    }

    // @NOTE Const or non const based on template isConst
    pointer operator->()
    {
        // Reference of item pointer
        return &(operator*());
    }

    // Prefix increment
    SparseSetIterator& operator++()
    {
        ++m_index;
        return *this;
    }

    // Postfix increment
    SparseSetIterator operator++(int)
    {
        SparseSetIterator tmp = *this;
        ++(*this); // Delegate to prefix increment
        return tmp;
    }

    // Prefix decrement
    SparseSetIterator& operator--()
    {
        --m_index;
        return *this;
    }

    // Postfix decrement
    SparseSetIterator operator--(int)
    {
        SparseSetIterator tmp = *this;
        --(*this); // Delegate to prefix decrement
        return tmp;
    }

    SparseSetIterator operator+(const difference_type value)
    {
        SparseSetIterator tmp = *this;
        return (tmp += value);
    }

    SparseSetIterator& operator+=(const difference_type value)
    {
        m_index += value;
        return *this;
    }

    SparseSetIterator operator-(const difference_type value)
    {
        SparseSetIterator tmp = *this;
        return (tmp -= value);
    }

    SparseSetIterator& operator-=(const difference_type value)
    {
        m_index -= value;
        return *this;
    }

    reference operator[](const difference_type value) const
    {
        return &m_ptr[value];
    }

    // friend allows to declare this operator as non-member
    // but still getting access to private fields in implementation
    friend bool operator==(const SparseSetIterator& a, const SparseSetIterator& b)
    {
        return a.m_index == b.m_index;
    };
    friend bool operator!=(const SparseSetIterator& a, const SparseSetIterator& b)
    {
        return a.m_index != b.m_index;
    };

    friend bool operator<(const SparseSetIterator& a, const SparseSetIterator& b)
    {
        return a.m_index < b.m_index;
    }
    friend bool operator<=(const SparseSetIterator& a, const SparseSetIterator& b)
    {
        return a.m_index <= b.m_index;
    }
    friend bool operator>(const SparseSetIterator& a, const SparseSetIterator& b)
    {
        return !a->operator<=(b);
    }
    friend bool operator>=(const SparseSetIterator& a, const SparseSetIterator& b)
    {
        return !a->operator<(b);
    }

  private:
    pointer m_ptr;
    size_type m_index;
};

} // namespace ECS
} // namespace Tefnout

#endif
