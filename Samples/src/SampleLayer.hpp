#ifndef __SAMPLELAYER__HPP
#define __SAMPLELAYER__HPP

#include "Tefnout.hpp"


class SampleLayer : public Tefnout::ApplicationLayer
{
  public:
    SampleLayer();
    virtual ~SampleLayer() = default;

    // Disable copy
    SampleLayer &operator=(SampleLayer other) = delete;
    SampleLayer(const SampleLayer &other) = delete;

    void OnUpdate() override;
    void OnRender() override;

private:
    std::shared_ptr<Tefnout::Rendering::IVertexArray> m_pVertexArray;
    std::shared_ptr<Tefnout::Rendering::IShader> m_pShader;
    std::shared_ptr<Tefnout::Rendering::Camera::ICamera> m_pCamera;
};

#endif
