#ifndef __RENDERINGFACTORY__HPP
#define __RENDERINGFACTORY__HPP

#include "Tefnout/Core/Platform.hpp"

#include "IIndexBuffer.hpp"
#include "ITexture.hpp"
#include "IVertexBuffer.hpp"
#include "Tefnout/Rendering/IShader.hpp"

// @NOTE
// Should add service pattern to handle graphic API initialization
// @NOTE
//

namespace Tefnout
{
namespace Rendering
{

/**
 * @brief      An enum that represent all Graphic API allowed in the engine.
 */
enum class TEFNOUT_API API : uint8_t
{
    None = 0,
    OpenGl = 1
};

/**
 * @brief      Global state used to get the currently used graphic API.
 *
 */
class TEFNOUT_API GraphicsAPI
{
  public:
    inline static API GetAPI()
    {
        return s_API;
    }

  private:
    // Hard coded for now
    static const API s_API = API::OpenGl;
};

/**
 * @brief      Factory to load and build a 2D texture from a file path using bound graphic
 *             API.
 *
 * @param[in]  path     The path used to find the texture picture
 * @param[in]  options  Default value to use when loading the texture
 *
 * @return     A shared pointer to loaded 2D Texture.
 */
std::shared_ptr<ITexture2D> TEFNOUT_API Create2DTexture(const std::string &path,
                                                        TextureOptions options);

/**
 * @brief      Creates a shader with a specific name by combining both Vertex and Fragment
 *             parts using bound graphic API.
 *
 * @param[in]  name                The name used as ID for the shader
 * @param[in]  vertexShaderPath    The vertex shader path
 * @param[in]  fragmentShaderPath  The fragment shader path
 *
 * @return     A shared pointer to created Shader program.
 */
std::shared_ptr<IShader> TEFNOUT_API CreateShader(const std::string name,
                                                  const std::string vertexShaderPath,
                                                  const std::string fragmentShaderPath);
/**
 * @brief      Factory that create a vertex buffer using bound graphic API
 *
 * @param      data      The data
 * @param[in]  dataSize  The size of data
 *
 * @return     A shared pointer to created Vertex buffer.
 */
std::shared_ptr<IVertexBuffer> TEFNOUT_API CreateVertexBuffer(float *data, uint32_t dataSize);

/**
 * @brief      Factory that create an index buffer using bound graphic API
 *
 * @param      indexes  The indexes
 * @param[in]  count    The number of indexes
 *
 * @return     A shared pointer to created Index buffer.
 */
std::shared_ptr<IIndexBuffer> TEFNOUT_API CreateIndexBuffer(uint32_t *indexes, uint32_t count);
} // namespace Rendering
} // namespace Tefnout

#endif
