#pragma once
#include "DResource.hpp"
#include "glm/vec3.hpp"
#include "macros.hpp"
namespace gbg {

enum class LightType {
    SPOT=0,
    DIRECTIONAL,
    _MAX,
};

const std::map<LightType, std::string_view> light_tToStr = {{LightType::SPOT, "Spot"}, {LightType::DIRECTIONAL, "Directional"}};

RESOURCE_HANDLE(Light);

class Light : public DResource<LightHandle> {
   public:
    DRESOURCE_CONSTR(Light)

    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 10.0f;
    float fov = 45.0f;  // degrees
    LightType type = LightType::SPOT;
};

RESOURCE_MANAGER(Light);

}  // namespace gbg
