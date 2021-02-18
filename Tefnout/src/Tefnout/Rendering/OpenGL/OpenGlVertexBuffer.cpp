#include "OpenGlVertexBuffer.hpp"

#include "glad/glad.h"

namespace Tefnout
{
namespace Rendering
{

OpenGlVertexBuffer::OpenGlVertexBuffer(float *data, uint32_t dataSize) : m_id(0), m_layout({})
{
    glCreateBuffers(1, &m_id);
    // Bind to update data
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    // @UNSURE Unbind to avoid linking to bind vertex buffer ??
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGlVertexBuffer::~OpenGlVertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

// IVertexBuffer interface
void OpenGlVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void OpenGlVertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGlVertexBuffer::UpdateData(float *data, uint32_t dataSize)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
}

} // namespace Rendering
} // namespace Tefnout
