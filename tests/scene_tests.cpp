
#include <gtest/gtest.h>

#include "Material.hpp"
#include "MaterialFunctions.hpp"
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

    auto h1 = msh_mg.create("Mesh1");
    auto h2 = msh_mg.create("Mesh2");

    auto& mesh = msh_mg.get(h1);

    ASSERT_EQ(mesh.getName(), "Mesh1");

    // Shader
    auto& sh_mg = sc.getShaderManager();

    auto shh = sh_mg.create("Shader1");
    auto shh2 = sh_mg.create("Shader2");

    auto& shader = sh_mg.get(shh);

    ASSERT_EQ(shader.getName(), "Shader1");

    shader.addParameter(ParameterTypes::FLOAT);
    shader.addParameter(ParameterTypes::VEC3);
    shader.addParameter(ParameterTypes::VEC2);

    auto& mt_mg = sc.getMaterialManager();

    auto mth = mt_mg.create("Material");
    auto& mt = mt_mg.get(mth);
    mt.setShader(sh_mg.get(shh).getHandle());

    setParametersFromShader(sc, mt);

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
    auto m1_h = md_mg.create("Model1");
    auto m2_h = md_mg.create("Model2");

    auto& st_mg = sc.getSceneTreeManager();
    auto root_h = st_mg.create("Root");
    st_mg.get(root_h).setResource(m1_h);

    // a lot of createion makes root break

    for (int i = 0; i < 10; i++) {
        auto child_h = st_mg.create("Child" + std::to_string(i));
        auto& child = st_mg.get(child_h);
        child.setResource(m2_h);
        st_mg.prependChild(root_h, child_h);
    }

    SceneTreeNode& fchild = st_mg.get(st_mg.get(root_h).childH);

    for (int i = 0; i < 10; i++) {
        auto ch1_h = st_mg.create("Child1" + std::to_string(i));
        auto& ch1 = st_mg.get(ch1_h);
        ch1.setResource(m2_h);
        st_mg.prependChild(st_mg.get(root_h).childH, ch1_h);
    }

    ASSERT_EQ(m1_h, st_mg.get(root_h).getResourceH<SceneObjectTypes::MODEL>());
    int i = 9;
    for (SceneTreeHandle nh = st_mg.get(root_h).childH; nh;
         nh = st_mg.get(nh).nextH) {
        SceneTreeNode& n = st_mg.get(nh);
        ASSERT_EQ(m2_h, n.getResourceH<SceneObjectTypes::MODEL>());
        ASSERT_EQ("Child" + std::to_string(i), n.getName());
        i--;
    }

    ASSERT_EQ(i, -1);
}
