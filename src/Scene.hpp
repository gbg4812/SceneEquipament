#pragma once
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Resource.hpp"
#include "SceneTree.hpp"
#include "Texture.hpp"

namespace gbg {

class Scene {
   public:
    Scene() { root = st_mg.create("Root"); };
    Scene(Scene& other) = delete;

    ResourceManager<Mesh, MeshHandle>& getMeshManager() { return ms_mg; }
    ResourceManager<Shader, ShaderHandle>& getShaderManager() { return sh_mg; }
    ResourceManager<Material, MaterialHandle>& getMaterialManager() {
        return mat_mg;
    }
    ResourceManager<Model, ModelHandle>& getModelManager() { return md_mg; }
    SceneTreeManager& getSceneTreeManager() { return st_mg; }
    ResourceManager<Camera, CameraHandle>& getCameraManager() { return cm_mg; }
    ResourceManager<Texture, TextureHandle>& getTextureManager() {
        return tx_mg;
    }

    SceneTreeHandle root;

    ResourceManager<Material, MaterialHandle> mat_mg;
    ResourceManager<Shader, ShaderHandle> sh_mg;
    ResourceManager<Mesh, MeshHandle> ms_mg;
    ResourceManager<Model, ModelHandle> md_mg;
    ResourceManager<Camera, CameraHandle> cm_mg;
    ResourceManager<Texture, TextureHandle> tx_mg;
    ResourceManager<Light, LightHandle> lh_mg;
    SceneTreeManager st_mg;
};

}  // namespace gbg
