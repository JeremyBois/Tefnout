#include "OpenGlShader.hpp"

#include "Tefnout/Utility/StreamIO.hpp"

#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp> // needed for glm::value_ptr

namespace Tefnout
{
namespace Rendering
{

OpenGlShader::OpenGlShader(const std::string name, const std::string vertexShaderPath,
                           const std::string fragmentShaderPath)
    : m_programId(0), m_name(name)
{
    auto vertexExt = Utility::StreamIO::GetExtensionFrom(vertexShaderPath);
    if (!vertexExt.has_value() || vertexExt.value() != "vert")
    {
        TEFNOUT_WARN("Vertex shader (<{0}>) does not have the <.vert> extension, continue anyway.",
                     vertexShaderPath);
    }

    auto fragExt = Utility::StreamIO::GetExtensionFrom(fragmentShaderPath);
    if (!fragExt.has_value() || fragExt.value() != "frag")
    {
        TEFNOUT_WARN("Fragment shader (<{0}>) does not have <.frag> extension, continue anyway.",
                     fragmentShaderPath);
    }

    Create(vertexShaderPath, fragmentShaderPath);
}

OpenGlShader::~OpenGlShader()
{
    glDeleteProgram(m_programId);
}

// General
void OpenGlShader::Bind() const
{
    glUseProgram(m_programId);
}

void OpenGlShader::UnBind() const
{
    glUseProgram(0);
}

// IO
void OpenGlShader::Create(const std::string vertexShaderPath, const std::string fragmentShaderPath)
{
    auto vertexCompilationResult = ShaderCheckResult::Ok;
    auto fragmentCompilationResult = ShaderCheckResult::Ok;

    // Load an compile vertex shader
    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vertexShaderString = Utility::StreamIO::ReadFile(vertexShaderPath);

    if (vertexShaderString.has_value())
    {
        const char *vertexShaderSource = vertexShaderString.value().c_str();
        // Assume null terminated string by using nullptr as last argument
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        vertexCompilationResult = Check(vertexShader, ShaderType::Fragment);
    }

    // Load an compile fragment shader
    uint32_t fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragmentShaderString = Utility::StreamIO::ReadFile(fragmentShaderPath);
    if (fragmentShaderString.has_value())
    {
        const char *fragShaderSource = fragmentShaderString.value().c_str();
        // Assume null terminated string by using nullptr as last argument
        glShaderSource(fragmentShader, 1, &fragShaderSource, nullptr);
        glCompileShader(fragmentShader);
        fragmentCompilationResult = Check(fragmentShader, ShaderType::Vertex);
    }

    // Create the program
    if (fragmentCompilationResult == ShaderCheckResult::Ok &&
        vertexCompilationResult == ShaderCheckResult::Ok)
    {
        // Create a shader program that link vertex and fragment together
        m_programId = glCreateProgram();
        glAttachShader(m_programId, vertexShader);
        glAttachShader(m_programId, fragmentShader);
        glLinkProgram(m_programId);
        if (Check(m_programId, ShaderType::Program) != ShaderCheckResult::Ok)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }
        else
        {
            TEFNOUT_DEBUG("Shader program <{0}> correctly created using <vertex shader - {1}> and "
                          "<fragment shader - {2}>.",
                          m_name, vertexShaderPath, fragmentShaderPath);
        }
    }
    else
    {
        TEFNOUT_WARN("Cannot create Shader program due to compilation error of Vertex and/or "
                     "Fragment parts.");
    }

    // Clean up, keeping only the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Uniform functions
void OpenGlShader::SetBool(std::string_view name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_programId, name.data()), (int)value);
}

void OpenGlShader::SetInt(std::string_view name, int value) const
{
    glUniform1i(glGetUniformLocation(m_programId, name.data()), value);
}

void OpenGlShader::SetFloat(std::string_view name, const float value) const
{
    glUniform1f(glGetUniformLocation(m_programId, name.data()), value);
}

void OpenGlShader::SetVec2(std::string_view name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(m_programId, name.data()), 1, glm::value_ptr(value));
}

void OpenGlShader::SetVec3(std::string_view name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(m_programId, name.data()), 1, glm::value_ptr(value));
}

void OpenGlShader::SetVec4(std::string_view name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(m_programId, name.data()), 1, glm::value_ptr(value));
}

void OpenGlShader::SetVec4(std::string_view name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(m_programId, name.data()), x, y, z, w);
}

void OpenGlShader::SetMat2(std::string_view name, const glm::mat2 &mat) const
{
    // GLM use column major ordering used by default by OpenGL
    glUniformMatrix2fv(glGetUniformLocation(m_programId, name.data()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}

void OpenGlShader::SetMat3(std::string_view name, const glm::mat3 &mat) const
{
    // GLM use column major ordering used by default by OpenGL
    glUniformMatrix3fv(glGetUniformLocation(m_programId, name.data()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}

void OpenGlShader::SetMat4(std::string_view name, const glm::mat4 &mat) const
{
    // GLM use column major ordering used by default by OpenGL
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.data()), 1, GL_FALSE,
                       glm::value_ptr(mat));
}

OpenGlShader::ShaderCheckResult OpenGlShader::Check(uint32_t shaderId,
                                                               ShaderType shaderType)
{
    ShaderCheckResult result(ShaderCheckResult::Ok);

    const uint32_t infoSizeMax = 1024;
    GLint success;
    GLchar infoLog[infoSizeMax];

    if (shaderType == ShaderType::Fragment || shaderType == ShaderType::Vertex)
    {
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            result = ShaderCheckResult::CompilationError;
            glGetShaderInfoLog(shaderId, infoSizeMax, nullptr, infoLog);
            TEFNOUT_ERROR("Shader compilation error (type {0}) - {1}", ToString(shaderType),
                          infoLog);
        }
    }
    else if (shaderType == ShaderType::Program)
    {
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success)
        {
            result = ShaderCheckResult::LinkError;
            glGetProgramInfoLog(shaderId, infoSizeMax, nullptr, infoLog);
            TEFNOUT_ERROR("Shader linking error (type {0}) - {1}", ToString(shaderType), infoLog);
        }
    }
    else
    {
        TEFNOUT_ERROR("Unknown shader type ({0}), cannot check it.", shaderType);
    }
    return result;
}

} // namespace Rendering
} // namespace Tefnout
