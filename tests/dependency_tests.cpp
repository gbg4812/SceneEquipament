#include "DependencyTree.hpp"
#include "DependencyTreeFunctions.hpp"
#include "Scene.hpp"
#include "SceneTree.hpp"
#include "gtest/gtest.h"

enum ResourceTypes {
    SCENE_TREE_NODE,
};

TEST(dependency_tests, create_propagate) {
    gbg::DependencyTreeManager manager;
    gbg::Scene sc;

    auto& e1 = sc.st_mg.create("Empty1");
    auto& e2 = sc.st_mg.create("Empty2");
    auto& e3 = sc.st_mg.create("Empty3");
    auto& e4 = sc.st_mg.create("Empty4");

    gbg::createRepresentative(manager, e1, SCENE_TREE_NODE);
    gbg::createRepresentative(manager, e2, SCENE_TREE_NODE);
    gbg::createRepresentative(manager, e3, SCENE_TREE_NODE);
    gbg::createRepresentative(manager, e4, SCENE_TREE_NODE);

    gbg::setDependent(manager, e1, 1, e2, 1);
    gbg::setDependent(manager, e2, 1, e3, 2);
    gbg::setDependent(manager, e3, 3, e4, 1);

    manager.propagateChange(e4.representative, 1);

    std::list<gbg::DependencyMask> comp = {1, 3, 1, 1};

    auto cit = comp.begin();

    for (auto mod : manager.getModified()) {
        auto& n = manager.get(mod);
        ASSERT_EQ(n.flags, *cit++);
    }
}
