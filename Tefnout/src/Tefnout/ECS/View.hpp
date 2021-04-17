#ifndef __VIEW__HPP
#define __VIEW__HPP

#include "Tefnout/Core/Core.hpp"

#include "Tefnout/ECS/Entity.hpp"
#include "Tefnout/ECS/SparseSet.hpp"
#include "Tefnout/ECS/Warehouse.hpp"

#include <iterator>
#include <tuple>

// @UNSURE
// Use unique tuple for the pool ??
// https://stackoverflow.com/questions/55941964/how-to-filter-duplicate-types-from-tuple-c
// @UNSURE

// @TODO
// Implement get based on type to quickly get a component from an entity id
// @TODO

namespace Tefnout
{
namespace ECS
{

/**
 * @brief      A view is a light wrapper arround multiple components containers allowing
 *             to loop over all entities shared among all containers in an efficient way.
 *
 * @tparam     ComponentTypes  Variadic type name for components to add to this collection.
 */
template <typename... ComponentTypes> class TEFNOUT_API View
{
  private:
    // Forward declaration needed
    template <typename ComponentIterator> class ViewEntitiesIterator;
    template <typename ComponentIterator> class ViewComponentsIterator;

  public:
    // Type of container used to store component collection without the current view (hence)
    // https://en.cppreference.com/w/cpp/language/sizeof...
    using others_type = std::array<const SparseSet*, (sizeof...(ComponentTypes) - 1)>;

    using size_type = std::size_t;
    using const_iterator = ViewEntitiesIterator<typename SparseSet::const_iterator>;
    using const_reverse_iterator = ViewEntitiesIterator<typename SparseSet::const_reverse_iterator>;

    const_iterator begin() const
    {
        return const_iterator{m_view->cbegin(), m_view->cend(), m_view->cbegin(),
                              SelectOthersFromPool(m_view)};
    }

    const_iterator end() const
    {
        return const_iterator{m_view->cbegin(), m_view->cend(), m_view->cend(),
                              SelectOthersFromPool(m_view)};
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator{m_view->crbegin(), m_view->crend(), m_view->crend(),
                                      SelectOthersFromPool(m_view)};
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator{m_view->crbegin(), m_view->crend(), m_view->crend(),
                                      SelectOthersFromPool(m_view)};
    }

    View() : m_pool{}, m_view{}
    {
    }

    View(Warehouse<ComponentTypes>&... components)
        : m_pool{std::make_tuple(&components...)}, m_view(SelectViewFromPool())
    {
    }
    ~View() = default;

    /**
     * @brief      Extract all or a specific selection of component from this view. Can be
     *             safely used to extract components while iterating over view entities.
     *
     * @note       Trying to get a component not owned by @p entity result in an assertion
     *             error or an empty tuple depending on build type.
     *
     * @param[in]  entity  The entity used to select components
     *
     * @tparam     Cs      Variadic types for components. Assumes all component is empty.
     *
     * @return     A tuple of required components extracted from the view
     */
    template <typename... Cs> decltype(auto) Components(const Entity entity) const
    {
        if constexpr (sizeof...(Cs) == 0)
        {
            // All components
            return std::tuple_cat(
                std::forward_as_tuple(std::get<Warehouse<ComponentTypes>*>(m_pool)->Get(entity))...);
        }
        else
        {
            // Only required components
            return std::tuple_cat(
                std::forward_as_tuple(std::get<Warehouse<Cs>*>(m_pool)->Get(entity))...);
        }
    }

  private:
    // Store each container inside a tuple to allow access by type
    // Types must be uniques but not currently checked
    const std::tuple<Warehouse<ComponentTypes>*...> m_pool;

    SparseSet* m_view;

    /**
     * @brief      Find the smaller component containers (based on its size) from the whole
     *             pool.
     *
     * @return     Smallest sparseSet from the pool.
     */
    SparseSet* SelectViewFromPool()
    {
        // https://stackoverflow.com/a/16387374
        return std::apply(
            [](auto const&... ps) -> SparseSet* {
                // Select the smallest sparseSet from the pool
                return (std::min)({static_cast<SparseSet*>(ps)...},
                                  [](const auto* pLeft, const auto* pRight) {
                                      return pLeft->Size() < pRight->Size();
                                  });
            },
            m_pool);
    }

    /**
     * @brief      Select all component containers from the pool except the one used as
     *             iterator (view)
     *
     * @param[in]  pView  The current view in used.
     *
     * @return     Pool component sparse set with @p pView removed.
     */
    others_type SelectOthersFromPool(const SparseSet* pView) const
    {
        others_type others{};

        // Using std::apply to loop over tuple elements without manually
        // writting template metaprogramming
        // https://stackoverflow.com/a/16387374
        std::apply(
            [&others, pView](auto const&... ps) {
                std::size_t pos = 0;

                // This is the actual function called for each element
                ((ps == pView ? void() : void(others[pos++] = ps)), ...);
            },
            m_pool);

        return others;
    }

    /**
     * @brief      View internal iterator used to loop over shared entities in all
     *             components containers.
     *
     * @tparam     ComponentIterator  Iterator type used for looping over.
     */
    template <typename ComponentIterator> class ViewEntitiesIterator
    {
      public:
        // Only interested in looping over a set of components so restrict category
        using iterator_category = std::bidirectional_iterator_tag;

        // Assign traits based on provided iterator
        using size_type = std::size_t;
        using difference_type = typename std::iterator_traits<ComponentIterator>::difference_type;
        using value_type = typename std::iterator_traits<ComponentIterator>::value_type;
        using pointer = typename std::iterator_traits<ComponentIterator>::pointer;
        using reference = typename std::iterator_traits<ComponentIterator>::reference;

        ViewEntitiesIterator(ComponentIterator start, ComponentIterator end,
                             ComponentIterator current, others_type other)
            : m_start(start), m_end(end), m_current(current), m_others(other)
        {
        }

        reference operator*()
        {
            // auto test = std::tuple_cat(std::make_tuple(*it), view->get(*it));
            return *m_current;
        }

        // @NOTE Const or non const based on template isConst
        pointer operator->()
        {
            return &(operator*());
        }

        // Prefix increment
        ViewEntitiesIterator& operator++()
        {

            while (++m_current != m_end && !HasAllComponents())
            {
                // Loop over entities until the end or if a valid one is found
            }
            return *this;
        }

        // Postfix increment
        ViewEntitiesIterator operator++(int)
        {
            ViewEntitiesIterator tmp = *this;
            ++(*this); // Delegate to prefix increment
            return tmp;
        }

        // Prefix decrement
        ViewEntitiesIterator& operator--()
        {
            while (--m_current != m_start && !HasAllComponents())
            {
                // Loop over entities until the end or if a valid one is found
            }
            return *this;
        }

        // Postfix decrement
        ViewEntitiesIterator operator--(int)
        {
            ViewEntitiesIterator tmp = *this;
            --(*this); // Delegate to prefix decrement
            return tmp;
        }

        // friend allows to declare this operator as non-member
        // but still getting access to private fields in implementation
        friend bool operator==(const ViewEntitiesIterator& a, const ViewEntitiesIterator& b)
        {
            return a.m_current == b.m_current;
        };
        friend bool operator!=(const ViewEntitiesIterator& a, const ViewEntitiesIterator& b)
        {
            return a.m_current != b.m_current;
        };

        friend bool operator<(const ViewEntitiesIterator& a, const ViewEntitiesIterator& b)
        {
            return a.m_current < b.m_current;
        }
        friend bool operator<=(const ViewEntitiesIterator& a, const ViewEntitiesIterator& b)
        {
            return a.m_current <= b.m_current;
        }
        friend bool operator>(const ViewEntitiesIterator& a, const ViewEntitiesIterator& b)
        {
            return !a->operator<=(b);
        }
        friend bool operator>=(const ViewEntitiesIterator& a, const ViewEntitiesIterator& b)
        {
            return !a->operator<(b);
        }

      private:
        ComponentIterator m_start;
        ComponentIterator m_end;
        ComponentIterator m_current;

        others_type m_others;

        /**
         * @brief      Check on all others component containers if current entity exists.
         *
         * @return     True if entity has a component in all containers else false.
         */
        bool HasAllComponents() const
        {
            const auto entity = *m_current;
            return std::all_of(
                m_others.cbegin(), m_others.cend(),
                [entity](const SparseSet* current) { return current->Contains(entity); });
        }
    };
};

} // namespace ECS
} // namespace Tefnout

#endif
