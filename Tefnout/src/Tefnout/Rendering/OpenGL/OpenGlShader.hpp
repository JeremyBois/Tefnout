#ifndef __OPENGLSHADER__HPP
#define __OPENGLSHADER__HPP

#include "Tefnout/Rendering/IShader.hpp"
#include "Tefnout/Core/Core.hpp"


namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Concrete implementation of a shader for OpenGL specifications.
 */
class TEFNOUT_API OpenGlShader : public IShader
{
  public:
    OpenGlShader(const std::string name, const std::string vertexShaderPath,
                 const std::string fragmentShaderPath);
    virtual ~OpenGlShader();

    // General
    void Bind() const override;
    void UnBind() const override;
    std::string_view GetName() const override
    {
        return m_name;
    }

    // IO
    void Create(const std::string vertexShaderPath, const std::string fragmentShaderPath) override;

    // Uniform functions
    void SetBool(std::string_view name, bool value) const override;
    void SetInt(std::string_view name, int value) const override;
    void SetFloat(std::string_view name, const float value) const override;

    void SetVec2(std::string_view name, const glm::vec2 &value) const override;
    void SetVec3(std::string_view name, const glm::vec3 &value) const override;
    void SetVec4(std::string_view name, const glm::vec4 &value) const override;
    void SetVec4(std::string_view name, float x, float y, float z, float w) const override;

    void SetMat2(std::string_view name, const glm::mat2 &mat) const override;
    void SetMat3(std::string_view name, const glm::mat3 &mat) const override;
    void SetMat4(std::string_view name, const glm::mat4 &mat) const override;

  private:
    uint32_t m_programId;
    std::string m_name;

    /**
     * @brief      Enum helper only used to check compilation or linking errors.
     */
    enum class ShaderType : int8_t
    {
        Fragment = 0,
        Vertex,
        Program
    };

    /**
     * @brief      Enum helper used to store shader compilation result.
     * @note       Avoid boolean blindness.
     */
    enum class ShaderCheckResult : int8_t
    {
        Ok = 0,
        CompilationError,
        LinkError,
    };

    inline const std::string ToString(ShaderType kind) const
    {
        switch (kind)
        {
        case ShaderType::Fragment:
            return "Fragment";
            break;
        case ShaderType::Vertex:
            return "Vertex";
            break;
        case ShaderType::Program:
            return "Program";
            break;
        default:
            TEFNOUT_ERROR("Unknown shader type.");
            return "";
            break;
        }
    }

    ShaderCheckResult Check(uint32_t shaderId, ShaderType kind);
};

} // namespace Rendering
} // namespace Tefnout

#endif
