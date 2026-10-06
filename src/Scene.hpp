#pragma once
#include "Camera.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Resource.hpp"
#include "SceneTree.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace gbg {

struct SceneDefaults {
    ShaderHandle shader;
    MaterialHandle material;
    TextureHandle texture;
    MeshHandle mesh;
    CameraHandle camera;
    LightHandle light;
};

class Scene {
   public:
    Scene() { root = st_mg.create("Root").getRID(); };
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

    Camera& getActiveCamera() {
        auto& cn = st_mg.get(active_camera);
        return cm_mg.get(*cn.getResourceH<SceneObjectTypes::CAMERA>());
    }

    MaterialManager mat_mg;
    ShaderManager sh_mg;
    MeshManager ms_mg;
    ResourceManager<Model, ModelHandle> md_mg;
    ResourceManager<Camera, CameraHandle> cm_mg;
    TextureManager tx_mg;
    LightManager lh_mg;
    SceneTreeManager st_mg;

    SceneDefaults defaults;
    
    const Shader& getDefaultShader() {
        return sh_mg.get(defaults.shader);
    };
    const Material& getDefaultMaterial() {
        return mat_mg.get(defaults.material);
    };
    const Texture& getDefaultTexture() {
        return tx_mg.get(defaults.texture);
    };
    const Mesh& getDefaultMesh() {
        return ms_mg.get(defaults.mesh);
    };
    const Camera& getDefaultCamera() {
        return cm_mg.get(defaults.camera);
    };
    const Light& getDefaultLight() {
        return lh_mg.get(defaults.light);
    };
    
};

}  // namespace gbg
