#ifndef __ITEXTURE__HPP
#define __ITEXTURE__HPP

#include "Tefnout/Core/CorePCH.hpp"

namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Interface for a Texture that provide minimal behavior that must implement a
 *             concrete class.
 */
class TEFNOUT_API ITexture
{
  public:
    virtual ~ITexture() = default;

    virtual void Bind(uint32_t slot = 0) const = 0;

    virtual uint32_t GetID() const = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetNbrChannels() const = 0;

    // virtual void SetData(void *data, uint32_t size) = 0;
};

/**
 * @brief      Interface for a 2D Texture.
 * @note       To later be able to use 1D or 3D ones
 */
class TEFNOUT_API ITexture2D : public ITexture
{
};

/**
 * @brief      Texture data used to load and prepare texture for rendering.
 */
struct TEFNOUT_API TextureOptions
{
    bool HasAlpha;
    bool ReverseY;
    bool GammaCorrection;

    TextureOptions() : HasAlpha(true), ReverseY(true), GammaCorrection(true)
    {
    }

    explicit TextureOptions(bool hasAlpha, bool reverseY)
        : HasAlpha(hasAlpha), ReverseY(reverseY), GammaCorrection(false)
    {
    }
};

} // namespace Rendering
} // namespace Tefnout
#endif
