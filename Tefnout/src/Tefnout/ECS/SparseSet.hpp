#ifndef __SPARSESET__HPP
#define __SPARSESET__HPP

#include "Tefnout/Core/Core.hpp"

#include "SparSetIterator.hpp"
#include "Tefnout/Core/Logger.hpp"
#include "Tefnout/ECS/Entity.hpp"

#include <bits/c++config.h>
#include <sstream>

namespace Tefnout
{
namespace ECS
{

template <std::size_t capacity> class TEFNOUT_API SparseSet
{
  public:
    // Internal types
    using size_type = decltype(capacity);
    using dense_array_type = std::array<Entity, capacity>;
    using sparse_array_type = std::vector<Entity>;

    // Iterators for entities
    using const_reverse_iterator = SparseSetIterator<Entity, true>;
    using const_iterator = std::reverse_iterator<const_reverse_iterator>;
    using reverse_iterator = SparseSetIterator<Entity, false>;
    using iterator = std::reverse_iterator<reverse_iterator>;

    SparseSet() : m_size(0)
    {
        m_dense.fill(nullEntity);

        TEFNOUT_ASSERT(m_sparse.size() == 0, "Initial size of sparse array should be 0 not {0}",
                       m_sparse.size());
    }

    // Default destructor
    virtual ~SparseSet() = default;

    // Default move/copy operators
    SparseSet(const SparseSet& other) = default;
    SparseSet(SparseSet&& other) = default;
    SparseSet& operator=(const SparseSet& other) = default;

    // Iterators for entities
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
        return reverse_iterator{m_dense.data(), 0};
    }

    reverse_iterator rend()
    {
        return reverse_iterator{m_dense.data(), m_size};
    }

    const_iterator cbegin()
    {
        return const_iterator{crend()};
    }

    const_iterator cend()
    {
        return const_iterator{crbegin()};
    }

    const_reverse_iterator crbegin()
    {
        return const_reverse_iterator{m_dense.data(), 0};
    }

    const_reverse_iterator crend()
    {
        return const_reverse_iterator{m_dense.data(), m_size};
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

    /**
     * @brief      Get index of @p entity in the dense array.
     *
     * @param[in]  entity  The entity
     *
     * @return     Index where @p entity is stored in the dense array.
     */
    size_type IndexEntity(const Entity entity) const
    {
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        return m_sparse[entity.GetId()].GetId();
    }

    /**
     * @brief      Construct and add new component to be associated with @p entity in a
     *             more optimal way than PushBack.
     *
     * @param[in]  entity  The entity
     *
     * @return     Newly added component
     */
    void EmplaceBack(const Entity entity)
    {
        TEFNOUT_ASSERT(!Contains(entity), "{0} - Entity already inside this container.", entity);

        const auto sparseIndex = entity.GetId();
        InsureSparseSize(sparseIndex);
        m_sparse[sparseIndex] = Entity{m_size};
        m_dense[m_size] = std::move(entity);
        m_size++;
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
        SwapAndPop(entity);
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
        Remove(begin(), end());
    }

    /**
     * @brief      Returns a string representation of the object.
     *
     * @return     String representation of the object.
     */
    std::string ToString()
    {
        std::stringstream ss;
        ss << "SparseSet{";
        for (size_type i = this->Size(); i > 0; --i)
        {
            ss << "(" << this->At(i - 1) << ")";
        }
        ss << "}";
        return ss.str();
    }

  protected:
    /**
     * @brief      Called BEFORE an entity is removed from a collection.
     *
     * @param[in]  <unnamed>  Index of the future removed entity.
     */
    virtual void OnSwapAndPop(const size_type){};

    /**
     * @brief      Called AFTER two entities have been swapped.
     *
     * @param[in]  <unnamed>  Index of the first entity.
     * @param[in]  <unnamed>  Index used for the second entity
     */
    virtual void OnSwap(const size_type, const size_type){};

  private:
    // Used to keep track of dense array size
    size_type m_size;
    sparse_array_type m_sparse;
    dense_array_type m_dense;

    void InsureSparseSize(const size_type index)
    {
        if (index >= m_sparse.size())
        {
            // Use resize to assign default value to new elements
            // Default value is by default (ahah) built using empty constructor
            m_sparse.resize(index + 1u);
        }
    }

    void Swap(const Entity first, const Entity second)
    {
        TEFNOUT_ASSERT(Contains(first), "{0} - Entity does not exists.", first);
        TEFNOUT_ASSERT(Contains(second), "{0} - Entity does not exists.", second);

        const auto from = IndexEntity(first);
        const auto to = IndexEntity((second));

        using std::swap;
        swap(m_sparse[first.GetId()], m_sparse[second.GetId()]);
        swap(m_dense[from], m_dense[to]);

        // Child can use it to make more actions
        OnSwap(from, to);
    }

    void SwapAndPop(const Entity entity)
    {
        TEFNOUT_ASSERT(Contains(entity), "{0} - Entity does not exists.", entity);

        const auto from = IndexEntity(entity);
        const auto to = m_size - 1;
        // Needed when removed entity is also the last one
        // In this special case calling m_dense[to].GetId() later is undefined
        const auto toInSparse = m_dense[to].GetId();

        // Child can use it to make more actions
        OnSwapAndPop(from);

        // Clear and swap (leaving null placeholder on the last entity)
        m_dense[from] = nullEntity;
        {
            using std::swap;
            swap(m_sparse[entity.GetId()], m_sparse[toInSparse]);
            swap(m_dense[from], m_dense[to]);
        }
        m_sparse[entity.GetId()] = nullEntity;
    }
};

// @TEST - AVOID REGRESSIONS
// Assert we CAN convert from const to const iterator
static_assert(std::is_convertible_v<SparseSet<5>::const_iterator, SparseSet<5>::const_iterator>);
// Assert we CAN convert from non-const to non-const iterator
static_assert(std::is_convertible_v<SparseSet<5>::iterator, SparseSet<5>::iterator>);

// Assert we CAN convert from non-const to const iterator
static_assert(std::is_convertible_v<SparseSet<5>::iterator, SparseSet<5>::const_iterator>);
// Assert we CANNOT convert from const to non-const iterator
static_assert(not std::is_convertible_v<SparseSet<5>::const_reverse_iterator,
                                        SparseSet<5>::reverse_iterator>);

// Both const and non-const construction are trivial
static_assert(std::is_trivially_copy_constructible_v<SparseSet<5>::const_reverse_iterator>);
static_assert(std::is_trivially_copy_constructible_v<SparseSet<5>::reverse_iterator>);
// @TEST - AVOID REGRESSIONS

} // namespace ECS
} // namespace Tefnout

#endif
