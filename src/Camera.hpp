#pragma once
#include <numbers>

#include "DResource.hpp"
#include "Resource.hpp"
#include "glm/trigonometric.hpp"
#include "macros.hpp"

namespace gbg {

RESOURCE_HANDLE(Camera);

class Camera : public DResource<CameraHandle> {
   public:
    DRESOURCE_CONSTR(Camera)

    float fov = glm::radians(std::numbers::pi / 2.0f);
    float znear = 0.1f;
    float zfar = 100.0f;
};

}  // namespace gbg
