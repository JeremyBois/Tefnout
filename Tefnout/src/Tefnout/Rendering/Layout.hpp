#ifndef __LAYOUT__HPP
#define __LAYOUT__HPP

#include "Tefnout/Core/Platform.hpp"
#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

#include <iostream>

namespace Tefnout
{
namespace Rendering
{

/**
 * @brief      Abstract representation of allowed data types in a buffer.
 * @note       Naming convention from GLSL data types
 *             (https://www.khronos.org/opengl/wiki/Data_Type_(GLSL))
 */
enum class AttributeType
{
    // Scalars
    Float = 0,
    Int,
    UInt,
    Bool,

    // Vectors
    Vec2,
    Vec3,
    Vec4,
    // IVec2,
    // IVec3,
    // IVec4,
    // BVec2,
    // BVec3,
    // BVec4,
    // UVec2,
    // UVec3,
    // UVec4,

    // Matrices
    Mat2 = 16, // Used to keep the same id if new vec are added
    Mat3,
    Mat4,
};

/**
 * @brief      Convert an attribute type to a std:string
 *
 * @param[in]  attributeType  The attribute type
 *
 * @return     String representation of @p attributeType
 */
std::string ToString(AttributeType attributeType);

/**
 * @brief      Abstract immutable representation of a Shader attribute.
 * @note       Name for both shader layout attribute and attribute should be the same but
 *             this is not required. What is important is that the attribute at a specific
 *             location match the declared attribute in the used shader.
 */
struct TEFNOUT_API ShaderAttribute
{
  public:
    // Must be build explicitely when used in an initializer_list
    // https://stackoverflow.com/a/14821937
    explicit ShaderAttribute(const std::string &name, AttributeType attributeType);
    explicit ShaderAttribute(const std::string &name, AttributeType attributeType, bool normalized);

    inline std::string Name() const
    {
        return m_name;
    };
    inline AttributeType Type() const
    {
        return m_type;
    };
    inline uint32_t Size() const
    {
        return m_size;
    };
    inline bool Normalized() const
    {
        return m_normalized;
    };
    inline uint32_t Count() const
    {
        return m_count;
    }

  private:
    // Should not be const to avoid removing asigment operator
    // Struct still somehow immutable because there is not setter
    std::string m_name;
    AttributeType m_type;
    uint32_t m_size;
    bool m_normalized;
    uint32_t m_count;
};

/**
 * @brief      An extended version of ShaderAttribute with offset information.
 */
struct TEFNOUT_API LayoutElement
{
  public:
    ShaderAttribute Attribute;
    std::size_t Offset;

    /// Default constructor is deleted explicitly
    LayoutElement() = delete;
    LayoutElement(const ShaderAttribute &attribute, std::size_t offset) noexcept;
};

/**
 * @brief      Container of layout attribute that describe how Vertex buffer raw data
 *             should be interpreted.
 * @note       Offset is automatically calculated using provide attributes.
 */
class TEFNOUT_API Layout
{
  public:
    /// Explicit default constructor
    Layout() noexcept;
    Layout(std::initializer_list<ShaderAttribute> attributes);

    inline uint32_t GetStrideSize() const
    {
        return m_strideSize;
    };

    // Provide C++11 for loop
    std::vector<LayoutElement>::iterator begin()
    {
        return m_elements.begin();
    }
    std::vector<LayoutElement>::iterator end()
    {
        return m_elements.end();
    }

    std::vector<LayoutElement>::const_iterator begin() const
    {
        return m_elements.cbegin();
    }
    std::vector<LayoutElement>::const_iterator end() const
    {
        return m_elements.cend();
    }

    std::vector<LayoutElement>::const_iterator cbegin() const
    {
        return m_elements.cbegin();
    }
    std::vector<LayoutElement>::const_iterator cend() const
    {
        return m_elements.cend();
    }

  private:
    std::vector<LayoutElement> m_elements;
    // Size of all attributes combined
    uint32_t m_strideSize;

    void StoreAttributes(std::initializer_list<ShaderAttribute> attributes);
};

// Needed for SPDLOG formatter
inline std::ostream &operator<<(std::ostream &os, const AttributeType &attributeType)
{
    return os << ToString(attributeType);
}

// Needed for SPDLOG formatter
inline std::ostream &operator<<(std::ostream &os, const ShaderAttribute &element)
{
    return os << element.Name() << "(Type == " << element.Type()
              << ", Normalized == " << (element.Normalized() ? "true" : "false")
              << ", Size == " << element.Size() << ")";
}

// Needed for SPDLOG formatter
inline std::ostream &operator<<(std::ostream &os, const LayoutElement &element)
{
    return os << element.Attribute << element.Offset;
}

} // namespace Rendering
} // namespace Tefnout

#endif
