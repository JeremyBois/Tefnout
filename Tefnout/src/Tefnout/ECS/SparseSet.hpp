#ifndef __SPARSESET__HPP
#define __SPARSESET__HPP

#include "Entity.hpp"
#include "SparSetIterator.hpp"

#include "Tefnout/Core/Core.hpp"

#include <array>
#include <bits/c++config.h>
#include <sstream>
#include <utility>

// @TODO
//  - use a callback to let user do some stuff before a component is destructed
//      using OnRemoveCallback = std::function<void(const Entity entity, TComponent destroyedData)>;
//  - use a callback to let user do some stuff after a component is added
//      using OnAddCallback = std::function<void(const Entity entity, TComponent addedData)>;
//  - use a callback to let user do some stuff after a component is updated
//      using OnUpdateCallback = std::function<void(const Entity entity, TComponent updatedData)>;
// @TODO

namespace Tefnout
{
namespace ECS
{

template <typename TComponent, std::size_t capacity> class SparseSet
{
    static_assert(std::is_move_constructible<TComponent>::value &&
                      std::is_move_assignable<TComponent>::value,
                  "The managed type must be at least move constructible and assignable");
    static_assert(std::is_copy_constructible<TComponent>::value &&
                      std::is_copy_assignable<TComponent>::value,
                  "The managed type must be at least move constructible and assignable");

  public:
    // Internal types
    using size_type = decltype(capacity);
    using components_type = std::array<TComponent, capacity>;
    using dense_array_type = std::array<Entity, capacity>;
    using sparse_array_type = std::vector<Entity>;

    // Iterators for component
    using reverse_iterator = SparseSetIterator<TComponent, false>;
    using const_reverse_iterator = SparseSetIterator<TComponent, true>;
    using iterator = std::reverse_iterator<reverse_iterator>;
    using const_iterator = std::reverse_iterator<const_reverse_iterator>;

    // Iterators for entities
    using const_reverse_iterator_entities = SparseSetIterator<Entity, true>;
    using const_iterator_entities = std::reverse_iterator<const_reverse_iterator_entities>;

    SparseSet() : m_size(0)
    {
        // Fill is done at compile time in C++20
        m_dense.fill(nullEntity);

        // Init elements with placeholder
        TEFNOUT_ASSERT(m_sparse.size() == 0, "Initial size of sparse array should be 0 not {0}",
                       m_sparse.size());
    }

    // Default destructor
    ~SparseSet() = default;

    // Default move/copy operators
    SparseSet(const SparseSet& other) = default;
    SparseSet(SparseSet&& other) = default;
    SparseSet& operator=(const SparseSet& other) = default;

    // Iterators for entities
    const_iterator_entities beginEntities()
    {
        return const_iterator_entities{rendEntities()};
    }

    const_iterator_entities endEntities()
    {
        return const_iterator_entities{rbeginEntities()};
    }

    const_reverse_iterator_entities rbeginEntities()
    {
        return const_reverse_iterator_entities{m_dense.data(), 0};
    }

    const_reverse_iterator_entities rendEntities()
    {
        return const_reverse_iterator_entities{m_dense.data(), m_size};
    }

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
        return reverse_iterator{m_components.data(), m_size};
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
        return const_reverse_iterator{m_components.data(), m_size};
    }

    /**
     * @brief      Getter for compile time container capacity, the maximum number of
     *             element that can be stored in this container.
     *
     * @return     The maximum number of Entity that can be stored inside this container.
     */
    inline constexpr size_type Capacity() const
    {
        return capacity;
    }

    /**
     * @brief      Determines if empty.
     *
     * @return     True if empty, False otherwise.
     */
    inline bool IsEmpty() const
    {
        // Cannot use m_dense.size() because it return the capacity
        // not the currently used range
        return m_size == 0;
    }

    /**
     * @brief      Size is the number of slot currently in used in the container.
     *
     * @return     Number of entity with component in this container.
     */
    inline size_type Size() const
    {
        return m_size;
    }

    /**
     * @brief      Read-only access to continuous array of component store inside the
     *             container.
     * @note       Forward iterators loop in the reverse order of internal raw component to
     *             avoid pitfall when entities are removed.
     *
     * @return     Read-only access to continuous array of component.
     */
    const TComponent* RawData() const
    {
        return m_components.data();
    }

    /**
     * @brief      Read-only access to continuous array of entities store inside the
     *             container.
     * @note       Forward iterators loop in the reverse order of internal raw component to
     *             avoid pitfall when entities are removed.
     *
     * @return     Read-only access to continuous array of entities.
     */
    const Entity* RawEntities() const
    {
        return m_dense.data();
    }

    /**
     * @brief      True if entity inside this container else false.
     *
     * @param[in]  entity  The entity
     *
     * @return     True if inside the container else false.
     */
    bool Contains(const Entity& entity) const
    {
        return (entity.GetId() < m_sparse.size()) ? m_sparse[entity.GetId()] != nullEntity : false;
    }

    /**
     * @brief      Get the entity at a specific index in the dense array.
     *
     * @param[in]  index  The index to use to find the entity
     *
     * @return     Found entity or nullEntity placeholder.
     */
    Entity At(const size_type index) const
    {
        return index < m_size ? m_dense[index] : nullEntity;
    }

    size_type EntityIndex(const Entity entity) const
    {
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        return m_sparse[entity.GetId()].GetId();
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
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        const Entity denseIndex = m_sparse[entity.GetId()];
        return m_components[denseIndex.GetId()];
    }

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
        TEFNOUT_ASSERT(!Contains(entity), "{0} - Entity already inside.", entity);

        // Must be called before moving entity
        const auto sparseIndex = entity.GetId();

        // Store Entity and copy of component
        m_dense[m_size] = std::move(entity);
        m_components[m_size] = component;

        // Setup fast lookup from Entity to component using sparse array
        InsureSparseSize(sparseIndex);
        m_sparse[sparseIndex] = Entity{m_size};
        m_size++;
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
        TEFNOUT_ASSERT(!Contains(entity), "{0} - Entity already inside.", entity);

        // Must be called before moving entity
        const auto sparseIndex = entity.GetId();

        // Store Entity and copy of component
        m_dense[m_size] = std::move(entity);
        m_components[m_size] = component;

        // Setup fast lookup from Entity to component using sparse array
        InsureSparseSize(sparseIndex);
        m_sparse[sparseIndex] = Entity{m_size};
        m_size++;
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
        TEFNOUT_ASSERT(!Contains(entity), "{0} - Entity already inside this container.", entity);

        // Construct first to avoid inconsistent state in case of failure/throw
        m_components[m_size] = TComponent(std::forward<Args>(args)...);

        // Setup fast lookup from Entity to component using sparse array
        const auto sparseIndex = entity.GetId();
        InsureSparseSize(sparseIndex);
        m_sparse[sparseIndex] = Entity{m_size};
        m_dense[m_size] = entity;
        m_size++;

        return this->Get(entity);
    }

    /**
     * @brief      Call a @p updater function on the found entity component to update it
     *             inplace returning the new value to caller.
     *
     * @see        https://stackoverflow.com/a/3582313
     *
     * @param[in]  entity    The entity used to find the corresponding component
     * @param      updater   The function used to update the component
     *
     * @tparam     Function  a Variadic function that takes found component as first argument.
     *
     * @return     Updated component corresponding to @p entity argument after calling @updater
     *             on it.
     */
    template <typename Function, typename... Args>
    TComponent& Update(const Entity entity, Function updater, Args... args)
    {
        // Get reference
        auto&& component = m_components[EntityIndex(entity)];

        // Call user defined function with any number of arguments
        updater(component, std::forward<Args>(args)...);

        return component;
    }

    /**
     * @brief      Removes the specified entity from the container and return it.
     *
     * @param[in]  entity  The entity
     *
     * @return     Removed component.
     */
    void Remove(const Entity entity)
    {
        auto component = SwapAndPop(entity);
        m_size--;
    }

    /**
     * @brief      Remove a user defined range from the collection.
     *
     * @param[in]  itBegin  The iterator begin
     * @param[in]  itEnd    The iterator end
     */
    template <typename Iterator> void Remove(Iterator itBegin, Iterator itEnd)
    {
        // Loop over the whole collection
        for (; itBegin != itEnd; ++itBegin)
        {
            Remove(*itBegin);
        }
    }

    /**
     * @brief      Remove all element of the collection.
     */
    void Clear()
    {
        Remove(beginEntities(), endEntities());
    }

    std::string ToString()
    {
        std::stringstream ss;
        ss << "SparseSet{";
        for (size_type i = m_size; i > 0; --i)
        {
            ss << "(" << m_dense[i - 1] << ", " << m_components[i - 1] << ")";
        }
        ss << "}";
        return ss.str();
    }

  private:
    // Used to keep track of dense array size
    size_type m_size;

    // Max size equals max ID allowed
    sparse_array_type m_sparse;

    // Max size equals max number of entity/component
    dense_array_type m_dense;
    components_type m_components;

    void InsureSparseSize(const size_type index)
    {
        if (index >= m_sparse.size())
        {
            // Use resize to assign default value to new elements
            m_sparse.resize(index + 1u);
        }
    }

    void Swap(const Entity first, const Entity second)
    {
        TEFNOUT_ASSERT(Contains(first), "{0} - Entity does not exists.", first);
        TEFNOUT_ASSERT(Contains(second), "{0} - Entity does not exists.", second);

        // Entity from dense array to get sparse array indexes
        const auto from = first.GetId();
        const auto to = second.GetId();

        using std::swap;
        swap(m_sparse[from], m_sparse[to]);
        swap(m_dense[m_sparse[from].GetId()], m_dense[m_sparse[to].GetId()]);
        swap(m_components[m_sparse[from].GetId()], m_components[m_sparse[to].GetId()]);
    }

    TComponent SwapAndPop(const Entity entity)
    {
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        // Entity from dense array to get sparse array indexes
        const auto from = entity.GetId();
        const auto to = m_dense[m_size - 1].GetId();

        // Extract component
        auto component = std::move(m_components[m_sparse[from].GetId()]);

        // Clear and swap (leaving null placeholder on the last element)
        m_dense[m_sparse[from].GetId()] = nullEntity;
        {
            using std::swap;
            swap(m_dense[m_sparse[from].GetId()], m_dense[m_sparse[to].GetId()]);
            swap(m_components[m_sparse[from].GetId()], m_components[m_sparse[to].GetId()]);
            swap(m_sparse[from], m_sparse[to]);
        }
        m_sparse[from] = nullEntity;

        return std::move(component);
    }
};

// @TEST - AVOID REGRESSIONS
// Assert we CAN convert from const to const iterator
static_assert(
    std::is_convertible_v<SparseSet<int, 5>::const_iterator, SparseSet<int, 5>::const_iterator>);
// Assert we CAN convert from non-const to non-const iterator
static_assert(std::is_convertible_v<SparseSet<int, 5>::iterator, SparseSet<int, 5>::iterator>);

// Assert we CAN convert from non-const to const iterator
static_assert(
    std::is_convertible_v<SparseSet<int, 5>::iterator, SparseSet<int, 5>::const_iterator>);
// Assert we CANNOT convert from const to non-const iterator
static_assert(not std::is_convertible_v<SparseSet<int, 5>::const_reverse_iterator,
                                        SparseSet<int, 5>::reverse_iterator>);

// Both const and non-const construction are trivial
static_assert(std::is_trivially_copy_constructible_v<SparseSet<int, 5>::const_reverse_iterator>);
static_assert(std::is_trivially_copy_constructible_v<SparseSet<int, 5>::reverse_iterator>);
// @TEST - AVOID REGRESSIONS

} // namespace ECS
} // namespace Tefnout

#endif
