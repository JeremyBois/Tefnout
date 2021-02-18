#include "OpenGlIndexBuffer.hpp"

#include "glad/glad.h"

namespace Tefnout
{
namespace Rendering
{
OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t *indexes, uint32_t count) : m_id(0), m_count(count)
{
    glCreateBuffers(1, &m_id);
    // Bind to update data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indexes, GL_STATIC_DRAW);
    // @UNSURE Unbind to avoid linking to bind vertex buffer ??
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGlIndexBuffer::~OpenGlIndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

// IIndexBuffer interface
void OpenGlIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void OpenGlIndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// void OpenGlIndexBuffer::SendData(uint32_t *indexes, uint32_t count)
// {
// }

} // namespace Rendering
} // namespace Tefnout
