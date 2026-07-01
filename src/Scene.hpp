#pragma once
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Resource.hpp"
#include "SceneTree.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace gbg {

class Scene {
   public:
    Scene() { root = st_mg.create("Root"); };
    Scene(Scene& other) = delete;

    MeshManager& getMeshManager() { return ms_mg; }
    ShaderManager& getShaderManager() { return sh_mg; }
    MaterialManager& getMaterialManager() { return mat_mg; }
    ResourceManager<Model, ModelHandle>& getModelManager() { return md_mg; }
    SceneTreeManager& getSceneTreeManager() { return st_mg; }
    ResourceManager<Camera, CameraHandle>& getCameraManager() { return cm_mg; }
    TextureManager& getTextureManager() { return tx_mg; }

    SceneTreeHandle root;
    SceneTreeHandle active_camera;

    MaterialManager mat_mg;
    ShaderManager sh_mg;
    MeshManager ms_mg;
    ResourceManager<Model, ModelHandle> md_mg;
    ResourceManager<Camera, CameraHandle> cm_mg;
    TextureManager tx_mg;
    LightManager lh_mg;
    SceneTreeManager st_mg;
};

}  // namespace gbg
