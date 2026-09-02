
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

    auto& mesh = msh_mg.create("Mesh1");
    auto& msh2 = msh_mg.create("Mesh2");

    ASSERT_EQ(mesh.getName(), "Mesh2");

    // Shader
    auto& sh_mg = sc.getShaderManager();

    auto& shader = sh_mg.create("Shader1");
    auto& sh2 = sh_mg.create("Shader2");

    ASSERT_EQ(shader.getName(), "Shader2");

    shader.addParameter(ParameterTypes::FLOAT_PARM);
    shader.addParameter(ParameterTypes::VEC3_PARM);
    shader.addParameter(ParameterTypes::VEC2_PARM);

    auto& mt_mg = sc.getMaterialManager();

    auto& mt = mt_mg.create("Material");
    mt.setShader(sh2.getRID());

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
    auto& mdl = md_mg.create("Model1");
    auto& mdl2 = md_mg.create("Model2");

    auto& st_mg = sc.getSceneTreeManager();
    auto& root = st_mg.create("Root");
    root.setResource((ModelHandle)mdl.getRID());

    for (int i = 0; i < 10; i++) {
        auto& child = st_mg.create("Child" + std::to_string(i));
        child.setResource((ModelHandle)mdl2.getRID());
        st_mg.prependChild(root.getRID(), child.getRID());
    }

    SceneTreeNode& fchild = st_mg.get(root.childH);

    for (int i = 0; i < 10; i++) {
        auto& child1 = st_mg.create("Child1" + std::to_string(i));
        child1.setResource((ModelHandle)mdl2.getRID());
        st_mg.prependChild(root.childH, child1.getRID());
    }

    ASSERT_EQ((ModelHandle)mdl.getRID(),
              root.getResourceH<SceneObjectTypes::MODEL>());
    int i = 9;
    for (SceneTreeHandle nh = root.childH; nh != SceneTreeHandle();
         nh = st_mg.get(nh).nextH) {
        SceneTreeNode& n = st_mg.get(nh);
        ASSERT_EQ((ModelHandle)mdl2.getRID(),
                  n.getResourceH<SceneObjectTypes::MODEL>());
        ASSERT_EQ("Child" + std::to_string(i), n.getName());
        i--;
    }
    ASSERT_EQ(i, -1);
}
