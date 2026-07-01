#pragma once
#include "Resource.hpp"
#include "glm/vec3.hpp"
#include "macros.hpp"
namespace gbg {
class Light : public Resource {
   public:
    Light() : Resource() {}
    Light(std::string name, uint32_t rid) : Resource(name, rid) {}

    float intensity = 0;
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 direction;
};

class LightHandle : public ResourceHandle {
   public:
    LightHandle() : ResourceHandle(){};
    LightHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index){};
};

RESOURCE_MANAGER(Light);

}  // namespace gbg
