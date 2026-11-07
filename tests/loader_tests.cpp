#include <gtest/gtest.h>

#include "Material.hpp"
#include "MaterialFunctions.hpp"
#include "Mesh.hpp"
#include "SceneTree.hpp"
#include "Texture.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "loaders/objLoader.hpp"

#define TRACY_ENABLE 1
#include "tracy/Tracy.hpp"
using namespace gbg;

std::ostream& operator<<(std::ostream& os, const glm::vec3 vec) {
    os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2 vec) {
    os << "(" << vec.x << "," << vec.y << ")";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& vec) {
    os << "[";
    for (const T& el : vec) {
        os << el << ", ";
    }
    os << "]" << std::endl;
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "(";
    for (const T& el : vec) {
        os << el << ", ";
    }
    os << ")" << std::endl;
    return os;
}

TEST(loader_tests, loader_load) {
    Scene sc;

    auto& mt_mg = sc.getMaterialManager();
    auto& sh_mg = sc.getShaderManager();
    auto& md_mg = sc.getModelManager();
    auto& ms_mg = sc.getMeshManager();
    auto& st_mg = sc.getSceneTreeManager();

    SceneTreeHandle parent = st_mg.create("Root").getRID();

    auto& mt = mt_mg.create("Default Material");
    auto& sh = sh_mg.create("Default Shader");
    /**
     * Material parameters:
     * vec3 ambient_color;
     * vec3 diffuse_color;
     * vec3 specular_color;
     * float specular_exponent;
     * texture albedo;
     * texture bump;
     */
    sh.addParameter(vec3_t(0.1, 0.1, 0.1));
    sh.addParameter(vec3_t(1, 1, 1));
    sh.addParameter(vec3_t(1, 1, 1));
    sh.addParameter(100.0f);
    sh.addParameter(TextureHandle());
    sh.addParameter(TextureHandle());
    gbg::setParametersFromShader(sh, mt);

    objLoadMeshes("./data/Cube.obj", &sc, parent, {});

    ASSERT_EQ(md_mg.begin()->getName(), "Cube");

    ASSERT_EQ(md_mg.begin()->getMesh(), ms_mg.begin()->getHandle());

    auto& msh = *ms_mg.begin();

    ASSERT_EQ(msh.getAttribute<gbg::AttributeTypes::VEC3_ATTR>(0).size(), 24);
}

TEST(loader_tests, loader_stress) {
    ZoneScoped;
    Scene sc;

    auto& mt_mg = sc.getMaterialManager();
    auto& md_mg = sc.getModelManager();
    auto& ms_mg = sc.getMeshManager();
    auto& st_mg = sc.getSceneTreeManager();

    auto& parent = st_mg.create("Root");

    auto& mt = mt_mg.create("Default Material");
    auto& sh = sc.sh_mg.create("Default Shader");

    objLoadMeshes("./data/sponza/sponza.obj", &sc, parent.getHandle(), {});

    ASSERT_EQ(ms_mg.begin()->getAttribute<AttributeTypes::VEC3_ATTR>(0).size(),
              184603);
}
