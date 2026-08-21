#pragma once
#include "glm/vec3.hpp"
#include "DResource.hpp"
#include "macros.hpp"
namespace gbg {
class Light : public DResource {
   public:
    Light() : DResource() {}
    Light(std::string name, uint32_t rid) : DResource(name, rid) {}

    float intensity = 0;
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 direction;
};

RESOURCE_HANDLE(Light);

RESOURCE_MANAGER(Light);

}  // namespace gbg
