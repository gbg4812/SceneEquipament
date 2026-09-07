#pragma once
#include "DResource.hpp"
#include "glm/vec3.hpp"
#include "macros.hpp"
namespace gbg {

RESOURCE_HANDLE(Light);

class Light : public DResource<LightHandle> {
   public:
    DRESOURCE_CONSTR(Light)

    glm::vec3 color = glm::vec3(1.0f);
};

RESOURCE_MANAGER(Light);

}  // namespace gbg
