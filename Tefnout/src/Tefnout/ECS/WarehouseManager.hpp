#ifndef __TEFNOUT_WAREHOUSEMANAGER__HPP
#define __TEFNOUT_WAREHOUSEMANAGER__HPP

#include "Tefnout/Core/Debug.hpp"
#include "Tefnout/ECS/Entity.hpp"
#include "Tefnout/ECS/FamilyGenerator.hpp"
#include "Tefnout/ECS/SparseSet.hpp"
#include "Tefnout/ECS/Warehouse.hpp"
#include <memory>
#include <utility>

namespace Tefnout
{
namespace ECS
{

class WarehouseManager
{

  public:
    using size_type = std::size_t;

    WarehouseManager() : m_warehouses()
    {
        m_warehouses.reserve(100);
    };

    ~WarehouseManager() = default;

    template <typename T> Warehouse<T>& Get()
    {
        return *PrepareWarehouse<T>();
    }


    template <typename T> T& GetComponent(Entity entity)
    {
        auto* warehouse = PrepareWarehouse<T>();
        return warehouse->Get(entity);
    }

    template <typename T> T& AddComponent(Entity entity, T component)
    {
        auto* warehouse = PrepareWarehouse<T>();
        return warehouse->EmplaceBack(std::move(entity), std::move(component));
    }

    template <typename T, typename... Args> T& AddComponent(Entity entity, Args&&... args)
    {
        auto* warehouse = PrepareWarehouse<T>();
        return warehouse->EmplaceBack(std::move(entity), std::forward<Args>(args)...);
    }

    template <typename T> void RemoveComponent(Entity entity)
    {
        auto* warehouse = PrepareWarehouse<T>();
        return warehouse->Remove(entity);
    }

  private:
    std::vector<std::unique_ptr<SparseSet>> m_warehouses;

    template <typename T> Warehouse<T>* PrepareWarehouse()
    {
        const size_type warehouseId = FamilyGenerator::TypeIdentifier<T>();

        // Reserve enough place
        InsureSpace(warehouseId);

        // Get warehouse or create it first if needed
        auto& mgr = m_warehouses.at(warehouseId);
        if (mgr == nullptr)
        {
            m_warehouses[warehouseId] = std::make_unique<Warehouse<T>>();
            TEFNOUT_ASSERT(mgr != nullptr,
                           "Warehouse at <{0}> should have been created and reference updated.",
                           warehouseId);
        }

        // Extract a reference to wrapped data
        return static_cast<Warehouse<T>*>(mgr.get());
    }

    void InsureSpace(const size_type index)
    {
        if (index >= m_warehouses.size())
        {
            m_warehouses.resize(index + 1u);
        }
    }
};

} // namespace ECS
} // namespace Tefnout

#endif
