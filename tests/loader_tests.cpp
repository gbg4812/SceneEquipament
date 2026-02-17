#include <gtest/gtest.h>

#include "Mesh.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "loaders/objLoader.hpp"

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
    SceneTree tree;

    auto& mt_mg = sc.getMaterialManager();
    auto& md_mg = sc.getModelManager();
    auto& ms_mg = sc.getMeshManager();

    auto mth = mt_mg.create("Default Material");

    objLoader("./data/Cube.obj", &sc, &tree, mth);

    ASSERT_EQ(md_mg.getAll()[0].getName(), "Cube");

    ASSERT_EQ(md_mg.getAll()[0].getMesh().getRID(), ms_mg.getAll()[0].getRID());
    ASSERT_EQ(md_mg.getAll()[0].getMaterial().getRID(), mth.getRID());

    ASSERT_EQ(ms_mg.getAll()[0]
                  .getAttribute<gbg::AttributeTypes::VEC3_ATTR>(0)
                  .size(),
              16);

    std::cout << ms_mg.getAll()[0].getFaces() << std::endl;
}
