#include "Tefnout/Core/Logger.hpp"
#include "TefnoutPCH.hpp"

#include "Layout.hpp"
#include <bits/c++config.h>
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <sys/types.h>
#include <utility>

namespace Tefnout
{
namespace Rendering
{

/**
 * @brief      Calculates the size taken by the abstract type in bytes.
 *
 * @param[in]  attributeType  The attribute type used to compute the size in bytes.
 *
 * @return     The size in bytes.
 */
constexpr std::pair<uint32_t, uint32_t> ComputeSizeAndCount(const AttributeType attributeType)
{
    switch (attributeType)
    {
    // Scalars
    case AttributeType::Float: {
        return {sizeof(float), 1};
    }
    case AttributeType::Int: {
        return {sizeof(int32_t), 1};
    }
    case AttributeType::UInt: {
        return {sizeof(uint32_t), 1};
    }
    case AttributeType::Bool: {
        return {sizeof(bool), 1};
    }

    // Vectors
    case AttributeType::Vec2: {
        return {sizeof(float) * 2, 2};
    }
    case AttributeType::Vec3: {
        return {sizeof(float) * 3, 3};
    }
    case AttributeType::Vec4: {
        return {sizeof(float) * 4, 4};
    }

    // Matrices
    case AttributeType::Mat2: {
        return {sizeof(float) * 3 * 3, 2};
    }
    case AttributeType::Mat3: {
        return {sizeof(float) * 3 * 3, 3};
    }
    case AttributeType::Mat4: {
        return {sizeof(float) * 4 * 3, 4};
    }

        TEFNOUT_WARN("Unsupported/Unimplemented Shader attribute type <{0}>", attributeType);
        return {0, 0};
    }
}

std::string ToString(AttributeType attributeType)
{
    switch (attributeType)
    {
    // Scalars
    case AttributeType::Float: {
        return "Float";
    }
    case AttributeType::Int: {
        return "Int";
    }
    case AttributeType::UInt: {
        return "UInt";
    }
    case AttributeType::Bool: {
        return "Bool";
    }

    // Vectors
    case AttributeType::Vec2: {
        return "Vec2";
    }
    case AttributeType::Vec3: {
        return "Vec3";
    }
    case AttributeType::Vec4: {
        return "Vec4";
    }

    // Matrices
    case AttributeType::Mat2: {
        return "Mat2";
    }
    case AttributeType::Mat3: {
        return "Mat3";
    }
    case AttributeType::Mat4: {
        return "Mat4";
    }
        TEFNOUT_WARN("Unsupported/Unimplemented Shader attribute type <{0}>", attributeType);
        return "";
    }
}

//--------------------------------------------------------------------------------
// Shader attribute
//--------------------------------------------------------------------------------
ShaderAttribute::ShaderAttribute(const std::string &name, AttributeType attributeType)
    : m_name(name), m_type(attributeType), m_size(0), m_normalized(false), m_count(0)
{
    std::tie(m_size, m_count) = ComputeSizeAndCount(attributeType);
}

ShaderAttribute::ShaderAttribute(const std::string &name, AttributeType attributeType,
                                 bool normalized)
    : m_name(name), m_type(attributeType), m_size(0), m_normalized(normalized), m_count(0)
{
    std::tie(m_size, m_count) = ComputeSizeAndCount(attributeType);
}

//--------------------------------------------------------------------------------
// Layout Element
//--------------------------------------------------------------------------------
LayoutElement::LayoutElement(const ShaderAttribute &attribute, std::size_t offset) noexcept
    : Attribute(attribute), Offset(offset)
{
}

//--------------------------------------------------------------------------------
// Layout
//--------------------------------------------------------------------------------
Layout::Layout() noexcept : m_elements({}), m_strideSize(0)
{
}
Layout::Layout(std::initializer_list<ShaderAttribute> attributes)
    : m_elements({}), m_strideSize(0)
{
    StoreAttributes(attributes);
}

void Layout::StoreAttributes(std::initializer_list<ShaderAttribute> attributes)
{
    m_elements = std::vector<LayoutElement>{};
    m_elements.reserve(attributes.size());

    std::size_t offsetSize = 0;
    m_strideSize = 0;

    for (auto attribute : attributes)
    {
        m_elements.emplace_back(attribute, offsetSize);
        offsetSize += attribute.Size();
        m_strideSize += attribute.Size();
    }
}

} // namespace Rendering
} // namespace Tefnout
