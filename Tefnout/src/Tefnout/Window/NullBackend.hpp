#ifndef __NULLBACKEND__HPP
#define __NULLBACKEND__HPP

#include "TefnoutPCH.hpp"

#include "IWindow.hpp"

namespace Tefnout
{
namespace Window
{
/**
 * @brief      Null implementation for the IWindow service. Does nothing.
 */
class TEFNOUT_LOCAL NullBackend : public IWindow
{
  public:
    NullBackend() = default;
    virtual ~NullBackend() = default;

    // Disable copy
    NullBackend &operator=(NullBackend other) = delete;
    NullBackend(const NullBackend &other) = delete;
    // Disable move
    NullBackend &operator=(NullBackend &&other) = delete;
    NullBackend(NullBackend &&other) = delete;

    // IWindow interface
    void OnRender() override{};
    void OnUpdate() override{};
    void SetEventCallback(
        const std::function<void(Event::IEvent &)> &callback) override{}; // NOLINT
    void *GetImplementation() const override
    {
        return nullptr;
    };
};
} // namespace Window
} // namespace Tefnout

#endif
