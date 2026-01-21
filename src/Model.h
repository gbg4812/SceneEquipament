#pragma once
#include "Material.h"
#include "Mesh.h"

namespace gbg {

class Model {
   public:
    Mesh& getMesh() { return _mesh; }
    Material& getMaterial() { return _material; }

   private:
    Mesh _mesh;
    Material _material;
};

}  // namespace gbg
