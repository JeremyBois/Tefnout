#ifndef __STACK__HPP
#define __STACK__HPP

#include "Tefnout/Core/ApplicationLayer.hpp"
#include "Tefnout/Core/Logger.hpp"
#include "Tefnout/Core/Platform.hpp"

#include <iterator>
#include <memory>
#include <vector>

namespace Tefnout
{

/**
 * @brief      Constraint checking for class members methods OnPop and OnPush.
 *
 * @tparam     T     Type to be tested at compile time.
 */
template<class T> struct IsStackable
{
    static void constraints(T a) { a.OnPop(); a.OnPush();}
    IsStackable() { void(*p)(T) = constraints; }
};


/**
 * @brief      General implementation of a Stack that take ownership of pushed data.
 *
 * @tparam     T     Data type stored inside the Stack that should implement at least
 *                   OnPop and OnPush methods.
 */
template <typename T> class TEFNOUT_API Stack
{
  public:
    using ContainerType = std::vector<std::unique_ptr<T>>;

    // Alias for all available iterators
    using iterator = typename ContainerType::iterator;
    using const_iterator = typename ContainerType::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Force compile time check of constraints
    explicit Stack() : m_container{}, m_nextIndex{0} { IsStackable<T>{}; }
    ~Stack();

    // Operator
    void Push(T *element);
    void PushLast(T *element);
    T* Pop();

    // Iterators
    iterator begin()
    {
        return m_container.begin();
    }
    iterator end()
    {
        return m_container.end();
    }

    reverse_iterator rbegin()
    {
        return m_container.rbegin();
    }
    reverse_iterator rend()
    {
        return m_container.rend();
    }

    const_iterator cbegin() const
    {
        return m_container.cbegin();
    }
    const_iterator cend() const
    {
        return m_container.cend();
    }

    const_reverse_iterator crbegin() const
    {
        return m_container.crbegin();
    }
    const_reverse_iterator crend() const
    {
        return m_container.crend();
    }

  private:
    ContainerType m_container;
    uint32_t m_nextIndex;
};

// Implementation
template <typename T> Stack<T>::~Stack()
{
    for (const auto &layer : m_container)
    {
        layer->OnPop();
    }
}

template <typename T> void Stack<T>::Push(T *element)
{
    m_container.emplace(m_container.begin() + m_nextIndex, std::unique_ptr<T>(element));
    m_container[m_nextIndex]->OnPush();
    ++m_nextIndex;
}

template <typename T> void Stack<T>::PushLast(T *element)
{
    m_container.emplace_back(std::unique_ptr<T>(element));
    m_container[m_container.size()]->OnPush();
}


// // Using specialization (not needed with type constraint)
// template <> inline Stack<ApplicationLayer>::~Stack()
// {
//     for (const auto &layer : m_container)
//     {
//         layer->OnPop();
//     }
// }

// template <> inline void Stack<ApplicationLayer>::Push(ApplicationLayer *element)
// {
//     m_container.emplace(m_container.begin() + m_nextIndex, std::unique_ptr<ApplicationLayer>(element));
//     m_container[m_nextIndex]->OnPush();
//     ++m_nextIndex;
// }

// template <> inline void Stack<ApplicationLayer>::PushLast(ApplicationLayer *element)
// {
//     m_container.emplace_back(std::unique_ptr<ApplicationLayer>(element));
//     m_container[m_container.size()]->OnPush();
// }


// @TEST - AVOID REGRESSIONS
// Assert we CAN convert from const to const iterator
static_assert(std::is_convertible_v<Stack<ApplicationLayer>::const_iterator, Stack<ApplicationLayer>::const_iterator>);
// Assert we CAN convert from non-const to non-const iterator
static_assert(std::is_convertible_v<Stack<ApplicationLayer>::iterator, Stack<ApplicationLayer>::iterator>);

// Assert we CAN convert from non-const to const iterator
static_assert(std::is_convertible_v<Stack<ApplicationLayer>::const_iterator, Stack<ApplicationLayer>::const_iterator>);
// Assert we CANNOT convert from const to non-const iterator
static_assert(not std::is_convertible_v<Stack<ApplicationLayer>::const_iterator, Stack<ApplicationLayer>::iterator>);

// Both const and non-const construction are trivial
static_assert(std::is_trivially_copy_constructible_v<Stack<ApplicationLayer>::const_iterator>);
static_assert(std::is_trivially_copy_constructible_v<Stack<ApplicationLayer>::iterator>);
// @TEST - AVOID REGRESSIONS

} // namespace Tefnout



#endif
