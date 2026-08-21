#pragma once
#include <numbers>
#include "Resource.hpp"
#include "DResource.hpp"
#include "glm/trigonometric.hpp"

namespace gbg {
class Camera : public DResource {
    public:
    Camera() : DResource(){}
    Camera(std::string name, uint32_t rid): DResource(name, rid){};

    float fov = glm::radians(std::numbers::pi/2.0f);
    float znear = 0.1f;
    float zfar = 100.0f;
};

class CameraHandle : public ResourceHandle {
    public:
    CameraHandle() : ResourceHandle(){};
    CameraHandle(uint32_t rid,size_t index ) : ResourceHandle(rid, index){};
};

}
