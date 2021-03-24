#ifndef __APPLICATIONLAYER__HPP
#define __APPLICATIONLAYER__HPP

#include "Tefnout/Core/Platform.hpp"
#include "Tefnout/Event/Event.hpp"
#include <memory>

namespace Tefnout
{
/**
 * @brief      Represent a subsystem within our global Application. Allow to control
 *             operation orders based on layer stack order.
 * @note       Needed to separate GUI and dispatch events in the specific order.
 * @see        LayerStack
 */
class TEFNOUT_API ApplicationLayer
{
  public:
    ApplicationLayer() = default;
    virtual ~ApplicationLayer() = default;

    // Disable copy
    ApplicationLayer &operator=(ApplicationLayer other) = delete;
    ApplicationLayer(const ApplicationLayer &other) = delete;

    // Constraints for Stack
    virtual void OnPush(){};
    virtual void OnPop(){};

    // Optional interface
    virtual void OnUpdate(){};
    virtual void OnRender(){};
    virtual void OnGui(){};

    virtual void OnEvent(const std::shared_ptr<Event::IEvent> &){};
};
} // namespace Tefnout

#endif
