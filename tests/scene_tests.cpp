
#include <gtest/gtest.h>

#include "Material.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Scene.hpp"
#include "SceneTree.hpp"
#include "Shader.hpp"

using namespace gbg;

TEST(scene_tests, create_resources) {
    Scene sc;

    // Mesh
    auto& msh_mg = sc.getMeshManager();

    MeshHandle mh1 = msh_mg.create("Mesh1");
    MeshHandle mh2 = msh_mg.create("Mesh2");

    auto& mesh = msh_mg.get(mh2);
    ASSERT_EQ(mesh.getName(), "Mesh2");

    // Shader
    auto& sh_mg = sc.getShaderManager();

    ShaderHandle sh1 = sh_mg.create("Shader1");
    ShaderHandle sh2 = sh_mg.create("Shader2");

    auto& shader = sh_mg.get(sh2);
    ASSERT_EQ(shader.getName(), "Shader2");

    shader.addParameter(ParameterTypes::FLOAT_PARM);
    shader.addParameter(ParameterTypes::VEC3_PARM);
    shader.addParameter(ParameterTypes::VEC2_PARM);

    auto& mt_mg = sc.getMaterialManager();

    MaterialHandle mth1 = mt_mg.create("Material");
    Material& mt = mt_mg.get(mth1);
    mt.setShader(sh2, shader);

    auto& vals = mt.getValues();

    auto it = vals.begin();
    for (ParameterTypes parmt : shader.getParameters()) {
        ASSERT_EQ(parmt, it->index());
        it++;
    }
}

TEST(scene_tests, scene_tree) {
    Scene sc;
    auto& md_mg = sc.getModelManager();
    ModelHandle mdl = md_mg.create("Model1");
    ModelHandle mdl2 = md_mg.create("Model2");

    auto& st_mg = sc.getSceneTreeManger();
    SceneTreeHandle rooth = st_mg.create("Root");
    st_mg.get(rooth).resource = mdl;
    SceneTreeHandle child1h = st_mg.create("Child1");
    st_mg.get(child1h).resource = mdl2;

    st_mg.prependChild(rooth, child1h);

    SceneTreeNode& root = st_mg.get(rooth);

    ASSERT_EQ(mdl, std::get<ModelHandle>(root.resource));
    ASSERT_EQ(child1h, root.childH);
    ASSERT_EQ(st_mg.get(child1h).parentH, rooth);
}
