#pragma once
#include "Material.hpp"
#include "Mesh.hpp"

namespace gbg {

struct Model {
    MeshHandle mesh;
    MaterialHandle material;
};

}  // namespace gbg
