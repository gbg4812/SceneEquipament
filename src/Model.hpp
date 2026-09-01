#pragma once
#include "DResource.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "macros.hpp"

namespace gbg {

RESOURCE_HANDLE(Model);

class Model : public DResource<ModelHandle> {
   public:
    Model() : DResource(){};
    Model(std::string name, uint32_t rid) : DResource(name, rid){};

    void setMesh(MeshHandle mesh) { _mesh = mesh; }
    void setMaterial(MaterialHandle material) { _material = material; }

    MeshHandle getMesh() { return _mesh; }
    MaterialHandle getMaterial() { return _material; }

   private:
    MeshHandle _mesh;
    MaterialHandle _material;
};

RESOURCE_MANAGER(Model);

}  // namespace gbg
