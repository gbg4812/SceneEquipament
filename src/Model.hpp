#pragma once
#include "Material.hpp"
#include "Mesh.hpp"
#include "Resource.hpp"

namespace gbg {

class Model : Resource {
   public:
    Model(std::string name, uint32_t rid) : Resource(name, rid) {};

    void setMesh(MeshHandle mesh) { _mesh = mesh; }
    void setMaterial(MaterialHandle material) { _material = material; }

    MeshHandle getMesh() { return _mesh; }
    MaterialHandle getMaterial() { return _material; }

   private:
    MeshHandle _mesh;
    MaterialHandle _material;
};

class ModelHandle : public ResourceHandle {
   public:
    ModelHandle() : ResourceHandle() {};
    ModelHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index) {};
};

}  // namespace gbg
