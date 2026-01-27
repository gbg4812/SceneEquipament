#pragma once
#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Resource.hpp"

namespace gbg {

class Scene {
   public:
    Scene() = default;
    Scene(Scene& other) = delete;

    ResourceManager<Mesh, MeshHandle>& getMeshManager() { return _ms_mg; }
    ResourceManager<Shader, ShaderHandle>& getShaderManager() { return _sh_mg; }
    ResourceManager<Material, MaterialHandle>& getMaterialManager() {
        return _mat_mg;
    }
    ResourceManager<Model, ModelHandle>& getModelManager() { return _md_mg; }

   private:
    ResourceManager<Material, MaterialHandle> _mat_mg;
    ResourceManager<Shader, ShaderHandle> _sh_mg;
    ResourceManager<Mesh, MeshHandle> _ms_mg;
    ResourceManager<Model, ModelHandle> _md_mg;
};

}  // namespace gbg
