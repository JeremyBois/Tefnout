#include "OpenGlShader.hpp"

#include "Tefnout/Utility/StreamIO.hpp"

#include <glm/gtc/type_ptr.hpp> // needed for glm::value_ptr
#include "glad/glad.h"

namespace Tefnout
{
namespace Rendering
{

OpenGlShader::OpenGlShader(const std::string name, const std::string vertexSrc,
                           const std::string fragmentSrc)
    : m_programId(0), m_name(name)
{
    Create(vertexSrc, fragmentSrc);
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
void OpenGlShader::Create(const std::string vertexPath, const std::string fragPath)
{
    // Load an compile vertex shader
    uint32_t vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderString = Utility::StreamIO::ReadFile(vertexPath);
    const char *vertexShaderSource = vertexShaderString.c_str();
    // Assume null terminated string
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    auto fragmentResult = CheckCompilation(vertexShader, ShaderType::Fragment);

    // Load an compile fragment shader
    uint32_t fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragShaderString = Utility::StreamIO::ReadFile(fragPath);
    const char *fragShaderSource = fragShaderString.c_str();
    // Assume null terminated string
    glShaderSource(fragmentShader, 1, &fragShaderSource, nullptr);
    glCompileShader(fragmentShader);
    auto vertexResult = CheckCompilation(vertexShader, ShaderType::Vertex);

    // Create the program
    if (fragmentResult == CompilationResult::Error || vertexResult == CompilationResult::Error)
    {
        // Create a shader program that link vertex and fragment together
        m_programId = glCreateProgram();
        glAttachShader(m_programId, vertexShader);
        glAttachShader(m_programId, fragmentShader);
        glLinkProgram(m_programId);
        if (CheckCompilation(vertexShader, ShaderType::Program) == CompilationResult::Error)
        {
            glDeleteProgram(m_programId);
        }
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

OpenGlShader::CompilationResult OpenGlShader::CheckCompilation(uint32_t shaderId,
                                                               ShaderType shaderType)
{
    CompilationResult result(CompilationResult::Ok);

    const uint32_t infoSizeMax = 1024;
    GLint success;
    GLchar infoLog[infoSizeMax];

    if (shaderType != ShaderType::Program)
    {
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            result = CompilationResult::Error;
            glGetShaderInfoLog(shaderId, infoSizeMax, nullptr, infoLog);
            TEFNOUT_ERROR("Shader compilation error (type {0}) - {1}", ToString(shaderType),
                          infoLog);
        }
    }
    else
    {
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success)
        {
            result = CompilationResult::Error;
            glGetProgramInfoLog(shaderId, infoSizeMax, nullptr, infoLog);
            TEFNOUT_ERROR("Shader linking error (type {0}) - {1}", ToString(shaderType), infoLog);
        }
    }
    return result;
}

} // namespace Rendering
} // namespace Tefnout
