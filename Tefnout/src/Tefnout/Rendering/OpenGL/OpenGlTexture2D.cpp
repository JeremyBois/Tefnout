#include "OpenGlTexture2D.hpp"

#include "glad/glad.h"
#include "stb_image/stb_image.h"

namespace Tefnout
{
namespace Rendering
{

OpenGlTexture2D::OpenGlTexture2D(const std::string& texturePath, TextureOptions options)
    : m_id(0), m_width(0), m_height(0), m_nbChannels(3), m_path(texturePath)
{
    int width, height, nbChannels;  // Ints required for stbi_load
    stbi_set_flip_vertically_on_load(static_cast<int>(options.ReverseY));
    stbi_uc *data = stbi_load(texturePath.c_str(), &width, &height, &nbChannels, 0);

    m_width = static_cast<uint32_t>(width);
    m_height = static_cast<uint32_t>(height);
    m_nbChannels = static_cast<uint32_t>(nbChannels);

    if (!data)
    {
        TEFNOUT_ERROR("Failed to select a format for input image at <{0}>", texturePath);
        return;
    }

    // Assign correct format based on number of channels and gamma correction
    GLenum internalFormat = 0;
    GLenum dataFormat = 0;

    if (m_nbChannels == static_cast<uint32_t>(1))
    {
        internalFormat = dataFormat = GL_RED;
    }
    else if (m_nbChannels == static_cast<uint32_t>(3))
    {
        internalFormat = options.GammaCorrection ? GL_SRGB : GL_RGB;
        dataFormat = GL_RGB;
    }
    else if (m_nbChannels == static_cast<uint32_t>(4))
    {
        internalFormat = options.GammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
        dataFormat = GL_RGBA;
    }
    else
    {
        TEFNOUT_ERROR("Format not supported for input image at <{0}>", texturePath);
    }

    // Gen + Bind (require OpenGL >= 4.5)
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    // Create an immutable texture allocating all minmap up-front (require OpenGL >= 4.5)
    glTextureStorage2D(m_id, 1, internalFormat, m_width, m_height);

    // @TODO Add this to options
    // Default wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // @TODO

    // First argument == Base level of detail
    // Second argument = texel offset in X direction
    // Third argument = texel offset in Y direction
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}
OpenGlTexture2D::~OpenGlTexture2D()
{
    glDeleteTextures(1, &m_id);
}

void OpenGlTexture2D::Bind(uint32_t unit) const
{
    glBindTextureUnit(unit, m_id);
}

} // namespace Rendering
} // namespace Tefnout
