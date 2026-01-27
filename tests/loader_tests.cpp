#include <gtest/gtest.h>

#include "Mesh.hpp"
#include "glm/gtx/string_cast.hpp"
#include "loaders/objLoader.hpp"

using namespace gbg;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "(";
    for (const T& el : vec) {
        os << glm::to_string(el) << ", ";
    }
    os << ")" << std::endl;
    return os;
}

TEST(loader_tests, loader_load) {
    Scene sc;
    SceneTree tree;
    objLoader("./data/Cube.obj", sc, &tree);

    auto& md_mg = sc.getModelManager();
    auto& ms_mg = sc.getMeshManager();

    ASSERT_EQ(md_mg.getAll()[0].getName(), "Cube");

    ASSERT_EQ(md_mg.getAll()[0].getMesh().getRID(), ms_mg.getAll()[0].getRID());

    ASSERT_EQ(ms_mg.getAll()[0]
                  .getAttribute<gbg::AttributeTypes::VEC3_ATTR>(0)
                  .size(),
              8);
}
