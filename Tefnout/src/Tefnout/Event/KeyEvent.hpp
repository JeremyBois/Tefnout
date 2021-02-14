#ifndef __KEYEVENT__HPP
#define __KEYEVENT__HPP

#include "Event.hpp"
#include "Tefnout/Input/KeyCode.hpp"
#include <sstream>

namespace Tefnout
{
namespace Event
{
class TEFNOUT_API KeyPressed : public IEvent
{
  public:
    explicit KeyPressed(Input::KeyCode keycode)
        : m_info(Information(Category::Input | Category::Keyboard, Kind::KeyPressed, "KeyPressed")),
          m_keycode(keycode)
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

    inline Input::KeyCode GetKeyCode() const
    {
        return m_keycode;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << m_prefixDebug;
        ss << m_keycode;
        return ss.str();
    }

  private:
    Information m_info;
    Input::KeyCode m_keycode;

    static constexpr auto m_prefixDebug = "KeyPressed Event - ";
};

class TEFNOUT_API KeyReleased : public IEvent
{
  public:
    explicit KeyReleased(Input::KeyCode keycode)
        : m_info(
              Information(Category::Input | Category::Keyboard, Kind::KeyReleased, "KeyReleased")),
          m_keycode(keycode)
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

    inline Input::KeyCode GetKeyCode() const
    {
        return m_keycode;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << m_prefixDebug;
        ss << m_keycode;
        return ss.str();
    }

  private:
    Information m_info;
    Input::KeyCode m_keycode;

    static constexpr auto m_prefixDebug = "KeyReleased Event - ";
};

class TEFNOUT_API KeyHeld : public IEvent
{
  public:
    explicit KeyHeld(Input::KeyCode keycode)
        : m_info(Information(Category::Input | Category::Keyboard, Kind::KeyHeld, "KeyHeld")),
          m_keycode(keycode)
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

    inline Input::KeyCode GetKeyCode() const
    {
        return m_keycode;
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << m_prefixDebug;
        ss << m_keycode;
        return ss.str();
    }

  private:
    Information m_info;
    Input::KeyCode m_keycode;

    static constexpr auto m_prefixDebug = "KeyHeld Event - ";
};
} // namespace Event
} // namespace Tefnout
#endif
