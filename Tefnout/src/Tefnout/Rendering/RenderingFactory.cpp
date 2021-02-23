#include "TefnoutPCH.hpp"

#include "RenderingFactory.hpp"

#include "Tefnout/Rendering/IGraphicRenderer.hpp"

// OpenGL
#include "Tefnout/Rendering/OpenGL/OpenGlGraphicRenderer.hpp"
#include "Tefnout/Rendering/OpenGL/OpenGlIndexBuffer.hpp"
#include "Tefnout/Rendering/OpenGL/OpenGlShader.hpp"
#include "Tefnout/Rendering/OpenGL/OpenGlTexture2D.hpp"
#include "Tefnout/Rendering/OpenGL/OpenGlVertexArray.hpp"
#include "Tefnout/Rendering/OpenGL/OpenGlVertexBuffer.hpp"

#include <memory>

namespace Tefnout
{
namespace Rendering
{

std::shared_ptr<ITexture2D> CreateTexture2D(const std::string &path, TextureOptions options)
{
    switch (IGraphicRenderer::GetAPI())
    {
    case API::None: {
        TEFNOUT_ERROR("Null graphic API not yet implemented. Come back later !");
        return nullptr;
    }
    case API::OpenGl: {
        // Only supported right now
        return std::make_shared<OpenGlTexture2D>(path, options);
    }
    default: {
        TEFNOUT_CRITICAL("Non existing graphic API.");
        return nullptr;
    }
    }
}

std::shared_ptr<IShader> CreateShader(const std::string name, const std::string vertexShaderPath,
                                      const std::string fragmentShaderPath)
{
    switch (IGraphicRenderer::GetAPI())
    {
    case API::None: {
        TEFNOUT_ERROR("Null graphic API not yet implemented. Come back later !");
        return nullptr;
    }
    case API::OpenGl: {
        return std::make_shared<OpenGlShader>(name, vertexShaderPath, fragmentShaderPath);
    }
    default: {
        TEFNOUT_CRITICAL("Non existing graphic API.");
        return nullptr;
    }
    }
}

std::shared_ptr<IVertexBuffer> CreateVertexBuffer(float *data, uint32_t dataSize)
{
    switch (IGraphicRenderer::GetAPI())
    {
    case API::None: {
        TEFNOUT_ERROR("Null graphic API not yet implemented. Come back later !");
        return nullptr;
    }
    case API::OpenGl: {
        return std::make_shared<OpenGlVertexBuffer>(data, dataSize);
    }
    default: {
        TEFNOUT_CRITICAL("Non existing graphic API.");
        return nullptr;
    }
    }
}

std::shared_ptr<IIndexBuffer> CreateIndexBuffer(uint32_t *indexes, uint32_t count)
{
    switch (IGraphicRenderer::GetAPI())
    {
    case API::None: {
        TEFNOUT_ERROR("Null graphic API not yet implemented. Come back later !");
        return nullptr;
    }
    case API::OpenGl: {
        return std::make_shared<OpenGlIndexBuffer>(indexes, count);
    }
    default: {
        TEFNOUT_CRITICAL("Non existing graphic API.");
        return nullptr;
    }
    }
}

std::shared_ptr<IVertexArray> CreateVertexArray()
{
    switch (IGraphicRenderer::GetAPI())
    {
    case API::None: {
        TEFNOUT_ERROR("Null graphic API not yet implemented. Come back later !");
        return nullptr;
    }
    case API::OpenGl: {
        return std::make_shared<OpenGlVertexArray>();
    }
    default: {
        TEFNOUT_CRITICAL("Non existing graphic API.");
        return nullptr;
    }
    }
}

std::unique_ptr<IGraphicRenderer> CreateGraphicRenderer()
{
    switch (IGraphicRenderer::GetAPI())
    {
    case API::None: {
        TEFNOUT_ERROR("Null graphic API not yet implemented. Come back later !");
        return nullptr;
    }
    case API::OpenGl: {
        return std::make_unique<OpenGlGraphicRenderer>();
    }
    default: {
        TEFNOUT_CRITICAL("Non existing graphic API.");
        return nullptr;
    }
    }
}

} // namespace Rendering
} // namespace Tefnout
