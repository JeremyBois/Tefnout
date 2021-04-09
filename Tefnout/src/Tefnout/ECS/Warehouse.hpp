#ifndef __WAREHOUSE__HPP
#define __WAREHOUSE__HPP

#include "SparseSet.hpp"
#include "Tefnout/Core/Core.hpp"

#include <bits/c++config.h>
#include <sstream>
#include <type_traits>
#include <utility>

namespace Tefnout
{
namespace ECS
{

template <typename TComponent> class TEFNOUT_API Warehouse final : public SparseSet
{
    static_assert(std::is_move_constructible<TComponent>::value &&
                      std::is_move_assignable<TComponent>::value,
                  "The managed type must be at least move constructible and assignable");
    static_assert(std::is_copy_constructible<TComponent>::value &&
                      std::is_copy_assignable<TComponent>::value,
                  "The managed type must be at least move constructible and assignable");

  public:
    // Internal types
    using size_type = SparseSet::size_type;
    using base_type = SparseSet;
    using components_type = std::vector<TComponent>;

    // Iterators for component
    using reverse_iterator = SparseSetIterator<TComponent, false>;
    using const_reverse_iterator = SparseSetIterator<TComponent, true>;
    using iterator = std::reverse_iterator<reverse_iterator>;
    using const_iterator = std::reverse_iterator<const_reverse_iterator>;

    // Iterators for component
    iterator begin()
    {
        return iterator{rend()};
    }

    iterator end()
    {
        return iterator{rbegin()};
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator{m_components.data(), 0};
    }

    reverse_iterator rend()
    {
        return reverse_iterator{m_components.data(), this->Size()};
    }

    const_iterator cbegin() const
    {
        return const_iterator{crend()};
    }

    const_iterator cend() const
    {
        return const_iterator{crbegin()};
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator{m_components.data(), 0};
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator{m_components.data(), this->Size()};
    }

    Warehouse() = default;
    ~Warehouse() = default;

    /**
     * @brief      Add component (@p component) to @p entity using move operator.
     * @see        SparseSet::EmplaceBack Faster alternative because component directly built
     *             in the container memory area.
     *
     * @param[in]  entity  The entity
     * @param[in]  component    The component
     */
    void PushBack(const Entity entity, TComponent&& component)
    {
        m_components.push_back(component);

        base_type::EmplaceBack(entity);
    }

    /**
     * @brief      Add a new copy of component (@p component) to @p entity.
     * @see        SparseSet::EmplaceBack Faster alternative because component directly built
     *             in the container memory area.
     *
     * @param[in]  entity  The entity
     * @param[in]  component    The component
     */
    void PushBack(const Entity entity, const TComponent& component)
    {
        m_components.push_back(component);

        base_type::EmplaceBack(entity);
    }

    /**
     * @brief      Construct and add new component to be associated with @p entity in a more
     *             optimal way than PushBack.
     * @see        SparseSet::PushBack requires move/copy operations whereas EmplaceBack
     *             builds component directly in the container memory area.
     *
     * @param[in]  entity  The entity
     * @param      args    The arguments needed to buid the component.
     *
     * @tparam     Args    Variadic template type
     *
     * @return     Newly added component
     */
    template <typename... Args> TComponent& EmplaceBack(const Entity entity, Args&&... args)
    {
        // Construct first to avoid inconsistent state in case of failure/throw
        m_components.emplace_back(std::forward<Args>(args)...);

        base_type::EmplaceBack(entity);

        return Get(entity);
    }

    /**
     * @brief      Get component associated with @p entity.
     *
     * @param[in]  entity  The entity
     *
     * @return     { description_of_the_return_value }
     */
    TComponent& Get(const Entity entity)
    {
        TEFNOUT_ASSERT(this->Contains(entity), "{0} - Entity does not exists.", entity);

        return m_components[this->IndexEntity(entity)];
    }

    /**
     * @brief      Call a @p updater function on the found entity component to update
     *             it inplace returning the new value to caller.
     *
     * @see        https://stackoverflow.com/a/3582313
     *
     * @param[in]  entity    The entity used to find the corresponding component
     * @param      updater   The function used to update the component
     * @param[in]  args      @p updater arguments
     *
     * @tparam     Function  Function that takes found component as first argument.
     * @tparam     Args      Variadic type for @p updater arguments
     *
     * @return     Updated component corresponding to @p entity argument after calling
     *             @updater on it.
     */
    template <typename Function, typename... Args>
    TComponent& Update(const Entity entity, Function updater, Args... args)
    {
        auto&& component = m_components[this->IndexEntity(entity)];
        updater(component, std::forward<Args>(args)...);
        return component;
    }

    std::string ToString()
    {
        std::stringstream ss;
        ss << "Warehouse{";
        for (size_type i = this->Size(); i > 0; --i)
        {
            ss << "(" << this->At(i - 1) << ", " << m_components[i - 1] << ")";
        }
        ss << "}";
        return ss.str();
    }

  private:
    components_type m_components;

    /**
     * @brief      Called BEFORE an entity is removed from a collection.
     *
     * @param[in]  entityIndex  Index of the future removed entity.
     */
    void OnSwapAndPop(const size_type entityIndex) override
    {
        // @TODO Add event on destruction for user to react
        auto component = std::move(m_components[entityIndex]);

        using std::swap;
        swap(m_components[entityIndex], m_components[this->Size() - 1]);
        m_components.pop_back();
    }

    /**
     * @brief      Called AFTER two entities have been swapped in parent collection.
     *
     * @param[in]  from  Index of the first entity.
     * @param[in]  to    Index used for the second entity
     */
    void OnSwap(const size_type from, const size_type to) override
    {
        using std::swap;

        swap(m_components[from], m_components[to]);
    }
};

// @TEST - AVOID REGRESSIONS
// Assert we CAN convert from const to const iterator
static_assert(
    std::is_convertible_v<Warehouse<int>::const_iterator, Warehouse<int>::const_iterator>);
// Assert we CAN convert from non-const to non-const iterator
static_assert(std::is_convertible_v<Warehouse<int>::iterator, Warehouse<int>::iterator>);

// Assert we CAN convert from non-const to const iterator
static_assert(std::is_convertible_v<Warehouse<int>::iterator, Warehouse<int>::const_iterator>);
// Assert we CANNOT convert from const to non-const iterator
static_assert(not std::is_convertible_v<Warehouse<int>::const_reverse_iterator,
                                        Warehouse<int>::reverse_iterator>);

// Both const and non-const construction are trivial
static_assert(std::is_trivially_copy_constructible_v<Warehouse<int>::const_reverse_iterator>);
static_assert(std::is_trivially_copy_constructible_v<Warehouse<int>::reverse_iterator>);
// @TEST - AVOID REGRESSIONS

} // namespace ECS
} // namespace Tefnout

#endif
