#ifndef __WINDOWEVENT__HPP
#define __WINDOWEVENT__HPP

#include "Event.hpp"

#include <sstream>

namespace Tefnout
{

namespace Event
{

/**
 * @brief      Event raised when a window is resized.
 */
class WindowResized : public IEvent
{
  public:
    explicit WindowResized(uint32_t width, uint32_t height)
        : m_info(Information(Category::Window, Kind::WindowClosed, "WindowResized")),
          m_width(width), m_height(height)
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

    uint32_t GetWidth() const
    {
        return m_width;
    }
    uint32_t GetHeight() const
    {
        return m_height;
    }

    std::string ToString() const override
    {
        std::stringstream ss{"WindowResize Event - "};
        ss << m_width << ", " << m_height;
        return ss.str();
    }

  private:
    Information m_info;
    uint32_t m_width;
    uint32_t m_height;
};

/**
 * @brief      Event raised when a window is closed.
 */
class WindowClosed : public IEvent
{
  public:
    explicit WindowClosed()
        : m_info(Information(Category::Window, Kind::WindowClosed, "WindowClosed"))
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

    std::string ToString() const override
    {
        return "WindowClosed Event";
    }

  private:
    Information m_info;
};
} // namespace Event
} // namespace Tefnout
#endif
