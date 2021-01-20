#ifndef __GAMEOBJECT__HPP
#define __GAMEOBJECT__HPP

#include "Tefranout_macro.hpp" // Needed for macro definition

#include "glm.hpp"
#include "gtc/quaternion.hpp"

namespace Tefranout
{

class TEFRANOUT_API GameObject
{
  public:
    inline uint32_t GetID()
    {
        return m_id;
    }

  private:
    uint32_t m_id;
};

} // namespace Tefranout
#endif
