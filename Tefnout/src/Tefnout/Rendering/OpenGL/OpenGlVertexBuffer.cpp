#include "OpenGlVertexBuffer.hpp"

#include "glad/glad.h"

namespace Tefnout
{
namespace Rendering
{

OpenGlVertexBuffer::OpenGlVertexBuffer(float *data, uint32_t dataSize) : m_id(0)
{
    glCreateBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);

    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
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

// void OpenGlVertexBuffer::SendData(float *data, uint32_t dataSize)
// {
// }

} // namespace Rendering
} // namespace Tefnout
