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

    gbg::SceneTreeHandle e1 = sc.st_mg.create("Empty1").getRID();
    gbg::SceneTreeHandle e2 = sc.st_mg.create("Empty2").getRID();
    gbg::SceneTreeHandle e3 = sc.st_mg.create("Empty3").getRID();
    gbg::SceneTreeHandle e4 = sc.st_mg.create("Empty4").getRID();

    gbg::createRepresentative(manager, e1, sc.st_mg, SCENE_TREE_NODE);
    gbg::createRepresentative(manager, e2, sc.st_mg, SCENE_TREE_NODE);
    gbg::createRepresentative(manager, e3, sc.st_mg, SCENE_TREE_NODE);
    gbg::createRepresentative(manager, e4, sc.st_mg, SCENE_TREE_NODE);

    gbg::setDependent(manager, sc.st_mg.get(e1), 1, sc.st_mg.get(e2), 1);
    gbg::setDependent(manager, sc.st_mg.get(e2), 1, sc.st_mg.get(e3), 2);
    gbg::setDependent(manager, sc.st_mg.get(e3), 3, sc.st_mg.get(e4), 1);

    manager.propagateChange(sc.st_mg.get(e4).representative, 1);

    std::list<gbg::DependencyMask> comp = {1, 3, 1, 1};

    auto cit = comp.begin();

    for (auto mod : manager.getModified()) {
        auto& n = manager.get(mod);
        ASSERT_EQ(n.flags, *cit++);
    }
}
