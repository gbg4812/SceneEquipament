#pragma once
#include "DResource.hpp"
#include "glm/vec3.hpp"
#include "macros.hpp"
namespace gbg {

RESOURCE_HANDLE(Light);

class Light : public DResource {
   public:
    DRESOURCE_CONSTR(Light)

    float intensity = 0;
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 direction;
};

RESOURCE_MANAGER(Light);

}  // namespace gbg
