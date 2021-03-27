#ifndef __SPARSESET__HPP
#define __SPARSESET__HPP

#include "Entity.hpp"
#include "SparSetIterator.hpp"

#include "Tefnout/Core/Core.hpp"

#include <array>
#include <bits/c++config.h>
#include <utility>

namespace Tefnout
{
namespace ECS
{

template <typename TData, std::size_t capacity> class SparseSet
{
    static_assert(std::is_move_constructible<TData>::value && std::is_move_assignable<TData>::value,
                  "The managed type must be at least move constructible and assignable");
    static_assert(std::is_copy_constructible<TData>::value && std::is_copy_assignable<TData>::value,
                  "The managed type must be at least move constructible and assignable");

  public:
    // Iterators for data
    using reverse_iterator = SparseSetIterator<TData, false>;
    using const_reverse_iterator = SparseSetIterator<TData, true>;
    using iterator = std::reverse_iterator<reverse_iterator>;
    using const_iterator = std::reverse_iterator<const_reverse_iterator>;

    // Iterators for entities
    using reverse_iterator_entities = SparseSetIterator<Entity, false>;
    using const_reverse_iterator_entities = SparseSetIterator<Entity, true>;
    using iterator_entities = std::reverse_iterator<reverse_iterator_entities>;
    using const_iterator_entities = std::reverse_iterator<const_reverse_iterator_entities>;

    SparseSet() : m_size(0)
    {
        // fill is done at compile time in C++20
        m_dense.fill(nullEntity);

        // Init elements with placeholder
        TEFNOUT_ASSERT(m_sparse.size() == 0, "Initial size of sparse array should be 0 not {0}",
                       m_sparse.size());
    }

    ~SparseSet() = default;

    // Iterators for entities
    iterator_entities beginEntities()
    {
        return iterator_entities{rendEntities()};
    }

    iterator_entities endEntities()
    {
        return iterator_entities{rbeginEntities()};
    }

    reverse_iterator_entities rbeginEntities()
    {
        return reverse_iterator_entities{m_dense.data(), 0};
    }

    reverse_iterator_entities rendEntities()
    {
        return reverse_iterator_entities{m_dense.data(), m_size};
    }

    // Iterators for data
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
        return reverse_iterator{m_data.data(), 0};
    }

    reverse_iterator rend()
    {
        return reverse_iterator{m_data.data(), m_size};
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
        return const_reverse_iterator{m_data.data(), 0};
    }

    const_reverse_iterator crend() const
    {
        return const_reverse_iterator{m_data.data(), m_size};
    }

    /**
     * @brief      Getter for compile time container capacity, the maximum number of
     *             element that can be stored in this container.
     *
     * @return     The maximum number of Entity that can be stored inside this container.
     */
    inline constexpr std::size_t Capacity() const
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
     * @return     Number of entity with data in this container.
     */
    inline std::size_t Size() const
    {
        return m_size;
    }

    /**
     * @brief      Read-only access to continuous array of data store inside the
     *             container.
     *
     * @return     Read-only access to continuous array of data.
     */
    const TData* RawData() const
    {
        return m_data.data();
    }

    /**
     * @brief      Read-only access to continuous array of entities store inside the
     *             container.
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
    Entity At(const std::size_t index) const
    {
        return index < m_size ? m_dense[index] : nullEntity;
    }

    std::size_t EntityIndex(const Entity entity) const
    {
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        return m_sparse[entity.GetId()].GetId();
    }

    /**
     * @brief      Get data associated with @p entity.
     *
     * @param[in]  entity  The entity
     *
     * @return     { description_of_the_return_value }
     */
    TData& Get(const Entity entity)
    {
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        const Entity denseIndex = m_sparse[entity.GetId()];
        return m_data[denseIndex.GetId()];
    }

    /**
     * @brief      Add data (@p data) to @p entity using move operator.
     * @see        SparseSet::EmplaceBack Faster alternative because data directly built
     *             in the container memory area.
     *
     * @param[in]  entity  The entity
     * @param[in]  data    The data
     */
    void PushBack(const Entity entity, TData&& data)
    {
        TEFNOUT_ASSERT(!Contains(entity), "{0} - Entity already inside.", entity);

        // Must be called before moving entity
        const auto sparseIndex = entity.GetId();

        // Store Entity and copy of data
        m_dense[m_size] = std::move(entity);
        m_data[m_size] = data;

        // Setup fast lookup from Entity to data using sparse array
        InsureSparseSize(sparseIndex);
        m_sparse[sparseIndex] = Entity{m_size};
        m_size++;
    }

    /**
     * @brief      Add a new copy of data (@p data) to @p entity.
     * @see        SparseSet::EmplaceBack Faster alternative because data directly built
     *             in the container memory area.
     *
     * @param[in]  entity  The entity
     * @param[in]  data    The data
     */
    void PushBack(const Entity entity, const TData& data)
    {
        TEFNOUT_ASSERT(!Contains(entity), "{0} - Entity already inside.", entity);

        // Must be called before moving entity
        const auto sparseIndex = entity.GetId();

        // Store Entity and copy of data
        m_dense[m_size] = std::move(entity);
        m_data[m_size] = data;

        // Setup fast lookup from Entity to data using sparse array
        InsureSparseSize(sparseIndex);
        m_sparse[sparseIndex] = Entity{m_size};
        m_size++;
    }

    /**
     * @brief      Construct and add new data to be associated with @p entity in a more
     *             optimal way than PushBack.
     * @see        SparseSet::PushBack requires move/copy operations whereas EmplaceBack
     *             builds data directly in the container memory area.
     *
     * @param[in]  entity  The entity
     * @param      args    The arguments needed to buid the data.
     *
     * @tparam     Args    Variadic template type
     *
     * @return     Newly added data
     */
    template <typename... Args> TData& EmplaceBack(const Entity entity, Args&&... args)
    {
        TEFNOUT_ASSERT(!Contains(entity), "{0} - Entity already inside this container.", entity);

        // Construct first to avoid inconsistent state in case of failure/throw
        m_data[m_size] = TData(std::forward<Args>(args)...);

        // Setup fast lookup from Entity to data using sparse array
        const auto sparseIndex = entity.GetId();
        InsureSparseSize(sparseIndex);
        m_sparse[sparseIndex] = Entity{m_size};
        m_dense[m_size] = entity;
        m_size++;

        return this->Get(entity);
    }

    /**
     * @brief      Call a @p updater function on the found entity data to update it
     *             inplace returning the new value to caller.
     *
     * @see        https://stackoverflow.com/a/3582313
     *
     * @param[in]  entity    The entity used to find the corresponding data
     * @param      updater   The function used to update the data
     *
     * @tparam     Function  a Variadic function that takes found data as first argument.
     *
     * @return     Updated data corresponding to @p entity argument after calling @updater
     *             on it.
     */
    template <typename Function, typename... Args>
    TData& Update(const Entity entity, Function updater, Args... args)
    {
        // Get reference
        auto&& data = m_data[EntityIndex(entity)];

        // Call user defined function with any number of arguments
        updater(data, std::forward<Args>(args)...);

        return data;
    }

    /**
     * @brief      Removes the specified entity from the container and return it.
     *
     * @param[in]  entity  The entity
     *
     * @return     Removed data.
     */
    void Remove(const Entity entity)
    {
        auto data = SwapAndPop(entity);
        m_size--;

        // @TODO use a callback to let user do some stuff before destruction
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

  private:
    // Used to keep track of dense array size
    std::size_t m_size;

    // Max size equals max ID allowed
    std::vector<Entity> m_sparse;

    // Max size equals max number of entity/component
    std::array<Entity, capacity> m_dense;
    std::array<TData, capacity> m_data;

    void InsureSparseSize(const std::size_t index)
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
        swap(m_data[m_sparse[from].GetId()], m_data[m_sparse[to].GetId()]);
    }

    TData SwapAndPop(const Entity entity)
    {
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        // Entity from dense array to get sparse array indexes
        const auto from = entity.GetId();
        const auto to = m_dense[m_size - 1].GetId();

        // Extract data
        auto data = std::move(m_data[m_sparse[from].GetId()]);

        // Clear and swap (leaving null placeholder on the last element)
        m_dense[m_sparse[from].GetId()] = nullEntity;
        {
            using std::swap;
            swap(m_dense[m_sparse[from].GetId()], m_dense[m_sparse[to].GetId()]);
            swap(m_data[m_sparse[from].GetId()], m_data[m_sparse[to].GetId()]);
            swap(m_sparse[from], m_sparse[to]);
        }
        m_sparse[from] = nullEntity;

        return std::move(data);
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
