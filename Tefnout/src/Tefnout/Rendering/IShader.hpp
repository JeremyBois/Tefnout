#ifndef __ISHADER__HPP
#define __ISHADER__HPP

#include "TefnoutPCH.hpp"

#include "glm/glm.hpp"


namespace Tefnout
{

namespace Rendering
{
/**
 * @brief      Interface describing what is a minimal implementation of a concrete shader
 *             looks like.
 * @note       This class is and should remain platform agnostic.
 */
class TEFNOUT_API IShader
{
  public:
    virtual ~IShader() = default;

    // General
    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;
    virtual std::string_view GetName() const = 0;

    // IO
    virtual void Create(const std::string vertexPath, const std::string fragPath) = 0;


    // Uniform functions
    virtual void SetBool(const std::string_view name, bool value) const = 0;
    virtual void SetInt(const std::string_view name, int value) const = 0;
    virtual void SetFloat(const std::string_view name, const float value) const = 0;

    virtual void SetVec2(const std::string_view name, const glm::vec2 &value) const = 0;
    virtual void SetVec3(const std::string_view name, const glm::vec3 &value) const = 0;
    virtual void SetVec4(const std::string_view name, const glm::vec4 &value) const = 0;
    virtual void SetVec4(const std::string_view name, float x, float y, float z, float w) const = 0;

    virtual void SetMat2(const std::string_view name, const glm::mat2 &mat) const = 0;
    virtual void SetMat3(const std::string_view name, const glm::mat3 &mat) const = 0;
    virtual void SetMat4(const std::string_view name, const glm::mat4 &mat) const = 0;
};
} // namespace Rendering
} // namespace Tefnout
#endif
