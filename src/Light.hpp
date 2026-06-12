#pragma once
#include "Resource.hpp"
namespace gbg {
class Light : public Resource {
   public:
    Light() : Resource() {}
    Light(std::string name, uint32_t rid) : Resource(name, rid) {}

    float intensity;
    glm::vec3 color;
    glm::vec3 direction;
};

class LightHandle : public ResourceHandle {
   public:
    LightHandle() : ResourceHandle(){};
    LightHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index){};
};

}  // namespace gbg
