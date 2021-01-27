#ifndef __MOUSEEVENT__HPP
#define __MOUSEEVENT__HPP

#include "Event.hpp"
#include "Tefnout/Input/MouseCode.hpp"
#include "TefnoutPCH.hpp"

#include <sstream>

namespace Tefnout
{
namespace Event
{

/**
 * @brief      Container for mouse offset.
 */
struct MouseOffset
{
  public:
    explicit MouseOffset(double xOffset, double yOffset) : X(xOffset), Y(yOffset)
    {
    }
    double X = 0.0;
    double Y = 0.0;

    std::string ToString() const
    {
        std::stringstream ss("MouseOffset(X=");
        ss << X << ", Y=" << Y << ")";
        return ss.str();
    }
};

inline std::ostream &operator<<(std::ostream &os, const MouseOffset &offset)
{
    return os << offset.ToString();
}

/**
 * @brief      Container for mouse position.
 */
struct MousePosition
{
    explicit MousePosition(double xPosition, double yPosition) : X(xPosition), Y(yPosition)
    {
    }
    double X = 0.0;
    double Y = 0.0;

    std::string ToString() const
    {
        std::stringstream ss("MousePosition(X=");
        ss << X << ", Y=" << Y << ")";
        return ss.str();
    }
};

inline std::ostream &operator<<(std::ostream &os, const MousePosition &position)
{
    return os << position.ToString();
}

class MouseButtonPressed : public IEvent
{
  public:
    explicit MouseButtonPressed(Input::MouseCode mousecode)
        : m_info(Information(Category::Input | Category::Mouse | Category::MouseButton,
                             Kind::MouseButtonPressed, "MouseButtonPressed")),
          m_mousecode(mousecode)
    {
    }

    inline Information GetInformation() const override
    {
        return m_info;
    }
    inline void MarkAsHandled() override
    {
        m_info.Status = State::Handled;
    }

    inline Input::MouseCode GetMouseCode() const
    {
        return m_mousecode;
    }

    std::string ToString() const override
    {
        std::stringstream ss(m_prefixDebug);
        ss << m_mousecode;
        return ss.str();
    }

  private:
    Information m_info;
    Input::MouseCode m_mousecode;

    static constexpr auto m_prefixDebug = "MouseButtonPressed Event - ";
};
class MouseButtonReleased : public IEvent
{
  public:
    explicit MouseButtonReleased(Input::MouseCode mousecode)
        : m_info(Information(Category::Input | Category::Mouse | Category::MouseButton,
                             Kind::MouseButtonReleased, "MouseButtonReleased")),
          m_mousecode(mousecode)
    {
    }

    inline Information GetInformation() const override
    {
        return m_info;
    }
    inline void MarkAsHandled() override
    {
        m_info.Status = State::Handled;
    }

    inline Input::MouseCode GetMouseCode() const
    {
        return m_mousecode;
    }

    std::string ToString() const override
    {
        std::stringstream ss(m_prefixDebug);
        ss << m_mousecode;
        return ss.str();
    }

  private:
    Information m_info;
    Input::MouseCode m_mousecode;

    static constexpr auto m_prefixDebug = "MouseButtonReleased Event - ";
};

class MouseScrolled : public IEvent
{
  public:
    explicit MouseScrolled(MouseOffset offset)
        : m_info(Information(Category::Input | Category::Mouse | Category::MouseButton,
                             Kind::MouseScrolled, "MouseScrolled")),
          m_offset(offset)
    {
    }

    inline Information GetInformation() const override
    {
        return m_info;
    }
    inline void MarkAsHandled() override
    {
        m_info.Status = State::Handled;
    }

    inline MouseOffset GetMouseOffset() const
    {
        return m_offset;
    }

    std::string ToString() const override
    {
        std::stringstream ss(m_prefixDebug);
        ss << m_offset;
        return ss.str();
    }

  private:
    Information m_info;
    MouseOffset m_offset;

    static constexpr auto m_prefixDebug = "MouseScrolled Event - ";
};

class MouseMoved : public IEvent
{
  public:
    explicit MouseMoved(MousePosition position)
        : m_info(Information(Category::Input | Category::Mouse | Category::MouseButton,
                             Kind::MouseMoved, "MouseMoved")),
          m_position(position)
    {
    }

    inline Information GetInformation() const override
    {
        return m_info;
    }
    inline void MarkAsHandled() override
    {
        m_info.Status = State::Handled;
    }

    inline MousePosition GetMousePosition() const
    {
        return m_position;
    }

    std::string ToString() const override
    {
        std::stringstream ss(m_prefixDebug);
        ss << m_position;
        return ss.str();
    }

  private:
    Information m_info;
    MousePosition m_position;

    static constexpr auto m_prefixDebug = "MouseMoved Event - ";
};
} // namespace Event
} // namespace Tefnout
#endif
