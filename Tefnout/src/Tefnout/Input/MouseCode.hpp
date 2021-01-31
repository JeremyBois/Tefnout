#ifndef __MOUSECODE__HPP
#define __MOUSECODE__HPP

#include "TefnoutPCH.hpp"

namespace Tefnout
{
namespace Input
{

/**
 * @brief      Mouse code as describe in GLFW
 * Mirror https://github.com/glfw/glfw/blob/master/include/GLFW/glfw3.h
 */
enum class MouseCode : uint32_t
{
    Button_1 = 0,
    Button_2 = 1,
    Button_3 = 2,
    Button_4 = 3,
    Button_5 = 4,
    Button_6 = 5,
    Button_7 = 6,
    Button_8 = 7,
    // Alias ??
    Button_last = Button_8,
    Button_left = Button_1,
    Button_right = Button_2,
    Button_middle = Button_3
};

/**
 * @brief      Convert @ref MouseCode "MouseCode" to its integer internal reprsentation.
 *
 * @param[in]  mousecode  The mousecode value
 *
 * @return     Integer representation of the @
 */
constexpr int32_t toInt(const MouseCode mousecode)
{
    return static_cast<int32_t>(mousecode);
}

/**
 * @brief      Convert a GLFW mousecode value to Tefnout @ref MouseCode "MouseCode"
 *
 * @param[in]  glfwMousecode  The glfw mousecode
 *
 * @return     Tefnout @ref MouseCode "MouseCode"
 */
constexpr MouseCode MousecodeFromGLFW(const int glfwMousecode)
{
    return static_cast<MouseCode>(glfwMousecode);
}

/**
 * @brief      Convert @ref MouseCode "MouseCode" to integer string representation.
 *
 * @param[in]  mousecode  The mousecode value
 *
 * @return     String representation of a @ref MouseCode "MouseCode"
 */
inline std::string toString(MouseCode mousecode)
{
    return std::to_string(toInt(mousecode));
}

inline std::ostream &operator<<(std::ostream &os, const MouseCode &mousecode)
{
    return os << toInt(mousecode);
}
} // namespace Input

} // namespace Tefnout
#endif
