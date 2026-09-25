
#include <gtest/gtest.h>

#include "Material.hpp"
#include "MaterialFunctions.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "SceneTree.hpp"
#include "Shader.hpp"

using namespace gbg;

TEST(scene_tests, create_resources) {
    Scene sc;

    // Mesh
    auto& msh_mg = sc.getMeshManager();

    auto& mesh = msh_mg.create("Mesh1");
    auto& m2 = msh_mg.create("Mesh2");

    ASSERT_EQ(mesh.getName(), "Mesh1");

    // Shader
    auto& sh_mg = sc.getShaderManager();

    auto& shader = sh_mg.create("Shader1");
    auto& sh2 = sh_mg.create("Shader2");

    ASSERT_EQ(shader.getName(), "Shader1");

    shader.addParameter(ParameterTypes::FLOAT);
    shader.addParameter(ParameterTypes::VEC3);
    shader.addParameter(ParameterTypes::VEC2);

    auto& mt_mg = sc.getMaterialManager();

    auto& mt = mt_mg.create("Material");
    mt.setShader(shader.getHandle());

    setParametersFromShader(sc, mt);

    auto& vals = mt.getValues();

    auto it = vals.begin();
    for (ParameterTypes parmt : shader.getParameters()) {
        ASSERT_EQ(to_underlying(parmt), it->index());
        it++;
    }
}

TEST(scene_tests, scene_tree) {
    Scene sc;
    auto& md_mg = sc.getModelManager();
    auto& m1 = md_mg.create("Model1");
    auto& m2 = md_mg.create("Model2");

    auto& st_mg = sc.getSceneTreeManager();
    auto& root = st_mg.create("Root");
    root.setResource(m1.getHandle());

    // a lot of createion makes root break

    for (int i = 0; i < 10; i++) {
        auto& child = st_mg.create("Child" + std::to_string(i));
        child.setResource(m2.getHandle());
        st_mg.prependChild(root.getHandle(), child.getHandle());
    }

    SceneTreeNode& fchild = st_mg.get(root.childH);

    for (int i = 0; i < 10; i++) {
        auto& ch1 = st_mg.create("Child1" + std::to_string(i));
        ch1.setResource(m2.getHandle());
        st_mg.prependChild(root.childH, ch1.getHandle());
    }

    ASSERT_EQ(m1.getHandle(), root.getResourceH<SceneObjectTypes::MODEL>());
    int i = 9;
    for (SceneTreeHandle nh = root.childH; nh; nh = st_mg.get(nh).nextH) {
        SceneTreeNode& n = st_mg.get(nh);
        ASSERT_EQ(m2.getHandle(), n.getResourceH<SceneObjectTypes::MODEL>());
        ASSERT_EQ("Child" + std::to_string(i), n.getName());
        i--;
    }

    ASSERT_EQ(i, -1);
}
