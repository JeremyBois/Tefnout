#ifndef __GLFWBACKEND__HPP
#define __GLFWBACKEND__HPP

#include "TefnoutPCH.hpp"

#include "Tefnout/Window/IWindow.hpp"

// Avoid public include of "GLFW/glfw3.h"
struct GLFWwindow;

namespace Tefnout
{
namespace Window
{

/**
 * @brief      Concrete Window implementation using GLFW as a backend. Support for OpenGL
 *             and Vulkan on Window and GNU/Linux.
 */
class TEFNOUT_LOCAL GLFWBackend : public IWindow
{
  public:
    GLFWBackend(const GenericProperties &properties);
    virtual ~GLFWBackend();

    // Disable copy
    GLFWBackend &operator=(GLFWBackend other) = delete;
    GLFWBackend(const GLFWBackend &other) = delete;
    // Disable move
    GLFWBackend &operator=(GLFWBackend &&other) = delete;
    GLFWBackend(GLFWBackend &&other) = delete;

    // IWindow interface
    void OnRender() override;
    void OnUpdate() override;
    void SetEventCallback(const std::function<void(Event::IEvent &)> &callback) override;
    void *GetImplementation() const override;

  private:
    /**
     * @brief      Minimal description of Window data that can be accessed in GLFW
     *             callback using UserPointer. Avoid exposing the whole class in callbacks.
     */
    struct Description
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;
        bool Focused;

        std::function<void(Event::IEvent &)> Callback;
    };

    struct Version
    {
        int const Major = 3;
        int const Minor = 3;
    };

    void Init(const GenericProperties &properties);
    void SetupCallbacks();
    void Clear();

    GLFWwindow *m_pGlfwWindow;
    Description m_information;
    Version m_version;

    static uint8_t s_instanceCount;
};
} // namespace Window
} // namespace Tefnout
#endif
