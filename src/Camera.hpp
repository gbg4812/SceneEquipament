#pragma once
#include <numbers>
#include "Resource.hpp"
#include "DResource.hpp"
#include "glm/trigonometric.hpp"
#include "macros.hpp"

namespace gbg {

RESOURCE_HANDLE(Camera);
    
class Camera : public DResource<CameraHandle> {
    public:
    Camera() : DResource(){}
    Camera(std::string name, uint32_t rid): DResource(name, rid){};

    float fov = glm::radians(std::numbers::pi/2.0f);
    float znear = 0.1f;
    float zfar = 100.0f;
};


}
