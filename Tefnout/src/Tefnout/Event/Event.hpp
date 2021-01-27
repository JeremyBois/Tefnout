#ifndef __EVENT__HPP
#define __EVENT__HPP

#include "TefnoutPCH.hpp"

#include <iostream>

namespace Tefnout
{
namespace Event
{

enum class State : int32_t
{
    Pending = 0,
    Handled = 1
};

/**
 * @brief   Category/Categories that can be assign to an event. Can be combined in a set.
 */
enum class Category : int32_t
{
    Unknown = 0,
    Window = 1 << 0, // binary 0001
    // All inputs
    Input = 1 << 1,    // binary 0010
    Keyboard = 1 << 2, // binary 0100
    // Mouse
    Mouse = 1 << 3, // binary 1000
    MouseButton = 1 << 4
};

/**
 * @brief      Bitwise 'or' operator for Category
 *
 * @param[in]  selfValue  The self value
 * @param[in]  inValue    In value
 *
 * @return     The result of the bitwise 'or'
 */
constexpr Category operator|(const Category selfValue, const Category inValue)
{
    return static_cast<Category>(static_cast<uint32_t>(selfValue) | static_cast<uint32_t>(inValue));
}

/**
 * @brief      Bitwise 'and' operator for Category.
 *
 * @param[in]  selfValue  The self value
 * @param[in]  inValue    In value
 *
 * @return     The result of the bitwise 'and'
 */
constexpr Category operator&(const Category selfValue, const Category inValue)
{
    return static_cast<Category>(static_cast<uint32_t>(selfValue) | static_cast<uint32_t>(inValue));
}

/**
 * @brief      Type of event that can be sent to the event system.
 */
enum class Kind : int32_t
{
    Unknown = 0,
    // Window
    WindowClosed,
    WindowResized,
    WindowFocused,
    WindowLostFocus,
    // Keyboard
    KeyPressed,
    KeyReleased,
    KeyHeld,
    // Mouse
    MouseMoved,
    // Mouse Button
    MouseButtonPressed,
    MouseButtonReleased,
    MouseScrolled
};

/**
 * @brief      Minimal information describing an event.
 */
struct Information
{
  public:
    explicit Information(Category categories, Kind type, std::string name)
        : Categories(categories), Type(type), Name(name), Status(State::Pending)
    {
    }
    Category Categories;
    Kind Type;
    std::string Name;
    State Status;
};

// @NEEDED ??
// inline std::ostream& operator<<(std::ostream& os, const Information& information)
// {
//     return os << information.ToString();
// }

/**
 * @brief      Interface that should be implemented by each event.
 */
class IEvent
{
  public:
    virtual ~IEvent() = default;

    /**
     * @brief      Get a copy of event internal description.
     *
     * @return     Event description.
     */
    virtual Information GetInformation() const = 0;
    /**
     * @brief      Update event state, marking it as handled.
     */
    virtual void MarkAsHandled() = 0;

    /**
     * @brief      String representation of the event. Mainly for debug pupose.
     *
     * @return     String representation of the event.
     */
    virtual std::string ToString() const = 0;
};

inline std::ostream &operator<<(std::ostream &os, const IEvent &event)
{
    return os << event.ToString();
}

} // namespace Event
} // namespace Tefnout
#endif
