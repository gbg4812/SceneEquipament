#pragma once
#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Resource.hpp"
#include "SceneTree.hpp"
#include "Texture.hpp"

namespace gbg {

class Scene {
   public:
    Scene() { root = _st_mg.create("Root"); };
    Scene(Scene& other) = delete;

    ResourceManager<Mesh, MeshHandle>& getMeshManager() { return _ms_mg; }
    ResourceManager<Shader, ShaderHandle>& getShaderManager() { return _sh_mg; }
    ResourceManager<Material, MaterialHandle>& getMaterialManager() {
        return _mat_mg;
    }
    ResourceManager<Model, ModelHandle>& getModelManager() { return _md_mg; }
    SceneTreeManager& getSceneTreeManager() { return _st_mg; }
    ResourceManager<Camera, CameraHandle>& getCameraManager() { return _cm_mg; }
    ResourceManager<Texture, TextureHandle>& getTextureManager() {
        return _tx_mg;
    }

    SceneTreeHandle root;

   private:
    ResourceManager<Material, MaterialHandle> _mat_mg;
    ResourceManager<Shader, ShaderHandle> _sh_mg;
    ResourceManager<Mesh, MeshHandle> _ms_mg;
    ResourceManager<Model, ModelHandle> _md_mg;
    ResourceManager<Camera, CameraHandle> _cm_mg;
    ResourceManager<Texture, TextureHandle> _tx_mg;
    SceneTreeManager _st_mg;
};

}  // namespace gbg
