#include "Tefnout/Core/Logger.hpp"
#include "TefnoutPCH.hpp"

#include "OpenGlVertexArray.hpp"
#include "glad/glad.h"

namespace Tefnout
{
namespace Rendering
{

/**
 * @brief      OpensGL corresponding value for a specific AttributeType.
 *
 * @note       Internal only so only declared in the CPP.
 *
 * @param[in]  attributeType  The platform agnostic layout attribute.
 *
 * @return     OpenGL enum value matching our abstract attribute type.
 */
GLenum OpenGLTypeFromShaderType(AttributeType attributeType)
{
    switch (attributeType)
    {
    case AttributeType::Float:
    case AttributeType::Vec2:
    case AttributeType::Vec3:
    case AttributeType::Vec4:
    case AttributeType::Mat2:
    case AttributeType::Mat3:
    case AttributeType::Mat4: {
        return GL_FLOAT;
    }
    case AttributeType::Int: {
        return GL_INT;
    }
    case AttributeType::Bool: {
        return GL_BOOL;
    }
    case AttributeType::UInt: {
        return GL_UNSIGNED_INT;
    }
    default:
        TEFNOUT_ASSERT(false, "Unsupported/Unimplemented Shader attribute type <{0}>", attributeType);
        return GL_FLOAT;
    }
}

//--------------------------------------------------------------------------------
// OpenGL Vertex Array
//--------------------------------------------------------------------------------
OpenGlVertexArray::OpenGlVertexArray()
    : m_id(0), m_vertexAttributeIndex(0), m_vertexBuffers({}), m_indexBuffer({})
{
    m_vertexBuffers.reserve(2);
    glCreateVertexArrays(1, &m_id);

    // Does not bind to avoid side effect
    // because if bind all next vertex and index buffers will be
    // linked to this vertex array ...
    // glBindVertexArray(m_id);
}
OpenGlVertexArray::~OpenGlVertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void OpenGlVertexArray::Bind() const
{
    glBindVertexArray(m_id);
}
void OpenGlVertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void OpenGlVertexArray::AddVertexBuffer(const std::shared_ptr<IVertexBuffer> &vertexBuffer)
{
    glBindVertexArray(m_id);
    vertexBuffer->Bind();

    // Make sure there is a layout
    TEFNOUT_ASSERT(vertexBuffer->HasLayout(), "Assertion fails {0}.",
                   "Vertex buffer has no layout");

    // Make sure both vertex array and vertex buffer are bind
    const auto &layout = vertexBuffer->GetLayout();
    for (const auto &element : layout)
    {
        // Using internal m_vertexAttributeIndex to avoid to overwrite previously bind
        // shader layout index
        glEnableVertexAttribArray(m_vertexAttributeIndex);
        glVertexAttribPointer(m_vertexAttributeIndex, element.Attribute.Count(),
                              OpenGLTypeFromShaderType(element.Attribute.Type()),
                              element.Attribute.Normalized() ? GL_TRUE : GL_FALSE,
                              layout.GetStrideSize(), (void *)element.Offset);
        ++m_vertexAttributeIndex;
    }

    // Keep a reference
    m_vertexBuffers.emplace_back(vertexBuffer);

    // @UNSURE UnBind both ??
}
void OpenGlVertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer> &indexBuffer)
{
    glBindVertexArray(m_id);
    indexBuffer->Bind();
    m_indexBuffer = indexBuffer;

    // @UNSURE UnBind both ??
}
} // namespace Rendering
} // namespace Tefnout
