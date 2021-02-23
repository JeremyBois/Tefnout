#ifndef __RENDERINGFACTORY__HPP
#define __RENDERINGFACTORY__HPP

#include "Tefnout/Core/Platform.hpp"

#include "Tefnout/Rendering/IGraphicRenderer.hpp"
#include "Tefnout/Rendering/IIndexBuffer.hpp"
#include "Tefnout/Rendering/IShader.hpp"
#include "Tefnout/Rendering/ITexture.hpp"
#include "Tefnout/Rendering/IVertexArray.hpp"
#include "Tefnout/Rendering/IVertexBuffer.hpp"
#include <memory>

// @NOTE
// Should add service pattern to handle graphic API initialization
// @NOTE
//

namespace Tefnout
{
namespace Rendering
{
/**
 * @brief      Factory to load and build a 2D texture from a file path using bound graphic
 *             API.
 *
 * @param[in]  path     The path used to find the texture picture
 * @param[in]  options  Default value to use when loading the texture
 *
 * @return     A shared pointer to loaded 2D Texture.
 */
std::shared_ptr<ITexture2D> TEFNOUT_API CreateTexture2D(const std::string &path,
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

/**
 * @brief      Creates an empty array buffer.
 *
 * @return     A shared pointer to created Vertex array buffer.
 */
std::shared_ptr<IVertexArray> TEFNOUT_API CreateVertexArray();


/**
 * @brief      Factory that create a concrete implementation of a rendering API.
 *
 * @return     A unique pointer to created graphic renderer API.
 */
std::unique_ptr<IGraphicRenderer> TEFNOUT_API CreateGraphicRenderer();

} // namespace Rendering
} // namespace Tefnout

#endif
