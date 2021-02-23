#include "Tefnout/Core/ApplicationLayer.hpp"
#include "Tefnout/Core/Logger.hpp"
#include "TefnoutPCH.hpp"

#include "SampleLayer.hpp"

SampleLayer::SampleLayer() : Tefnout::ApplicationLayer()
{
    using namespace Tefnout::Rendering;

    // Create Camera with specific aspect ratio
    auto ratio = 1280.0f / 720.0f;
    m_pCamera =
        std::make_shared<Camera::Orthographic>(Camera::FrustrumBounds(-ratio, ratio, -1.0f, 1.0f));
    auto test = static_cast<Camera::Orthographic *>(m_pCamera.get());
    test->SetPosition({1.0f, 0.5f, 0.0f});
    test->SetRotationZ(30.0f);

    // 1) Create Shader
    m_pShader = CreateShader("Basic", "Assets/Shaders/Basic.vert", "Assets/Shaders/Basic.frag");

    // 2) Create vertex array
    m_pVertexArray = CreateVertexArray();

    // Create vertex buffer with
    //   - position
    //   - color
    //   - normals (currently not used)
    float tempPos[3 * 4 * 3 * 3] = {0.0f,   0.5f,   0.0f,  0.521f, 0.6f, 0.f,    1.0f,   0.f,
                                    0.f,    0.f,    -0.5f, -0.5f,  0.0f, 0.862f, 0.196f, 0.184f,
                                    1.0f,   0.f,    0.f,   0.f,    0.5f, -0.5f,  0.0f,   0.164f,
                                    0.631f, 0.596f, 1.0f,  0.f,    0.f,  0.f};
    Layout layout = {
        ShaderAttribute{"a_pos", AttributeType::Vec3},
        ShaderAttribute{"a_color", AttributeType::Vec4},
        ShaderAttribute{"a_normal", AttributeType::Vec3, true},
    };
    auto vertexBuffer = CreateVertexBuffer(tempPos, sizeof(tempPos));
    vertexBuffer->SetLayout(layout);

    // 3) Create index buffer
    uint32_t indexes[3] = {0, 1, 2};
    auto indexBuffer = CreateIndexBuffer(indexes, sizeof(indexes) / sizeof(uint32_t));

    // 4) Assign data to vertex array
    m_pVertexArray->AddVertexBuffer(vertexBuffer);
    m_pVertexArray->SetIndexBuffer(indexBuffer);
}

void SampleLayer::OnUpdate()
{
    // USER_INFO("HERE");
}

void SampleLayer::OnRender()
{
    using namespace Tefnout::Rendering;

    Renderer::ClearColor({0.149f, 0.545f, 0.823f, 1.0f});
    Renderer::Clear();

    Renderer::Begin(m_pCamera);
    Renderer::Push(m_pShader, m_pVertexArray);
    Renderer::End();
}
