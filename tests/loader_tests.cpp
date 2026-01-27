#include <gtest/gtest.h>

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

    std::cout << sc.getMeshManager()
                     .getAll()[0]
                     .getAttribute<AttributeTypes::VEC3_ATTR>(0)
              << std::endl;
}
