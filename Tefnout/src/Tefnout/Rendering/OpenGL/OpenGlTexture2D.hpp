#ifndef __OPENGLTEXTURE2D__HPP
#define __OPENGLTEXTURE2D__HPP

#include "Tefnout/Rendering/ITexture.hpp"
#include "Tefnout/Rendering/TextureFactory.hpp"


namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Concrete implementation of a ITexture using OpenGL.
 */
class OpenGlTexture2D : public ITexture2D
{
  public:
    OpenGlTexture2D(const std::string& texturePath, TextureOptions information);
    virtual ~OpenGlTexture2D();

    // ITexture interface
    void Bind(uint32_t unit = 0) const override;

    inline uint32_t GetID() const override
    {
        return m_id;
    };
    inline uint32_t GetWidth() const override
    {
        return m_width;
    };
    inline uint32_t GetHeight() const override
    {
        return m_height;
    };
    inline uint32_t GetNbrChannels() const override
    {
        return m_nbChannels;
    };

  private:
    uint32_t m_id;
    uint32_t m_width;
    uint32_t m_height;
    uint8_t m_nbChannels;
    std::string m_path;
};
} // namespace Rendering
} // namespace Tefnout

#endif
