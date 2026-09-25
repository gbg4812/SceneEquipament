#include <gtest/gtest.h>

#include "AttributeTypes.hpp"
#include "Mesh.hpp"
#include "Resource.hpp"

using namespace gbg;

TEST(resource_tests, create_resource) {
    ResourceManager<Resource<ResourceHandle>, ResourceHandle> mg;

    auto& r1 = mg.create("Resource1");

    auto& r2 = mg.create("Resource2");

    ASSERT_EQ(r1.getName(), "Resource1");
    ASSERT_EQ(r2.getName(), "Resource2");

    for (int i = 3; i < 10000; i++) {
        mg.create("Resource" + std::to_string(i));
    }

    ASSERT_EQ(mg.get(MeshHandle(99)).getName(), "Resource99");
    ASSERT_EQ(mg.get(MeshHandle(23)).getName(), "Resource23");

    mg.destroy(MeshHandle(23));

    for (auto& res : mg) {
        ASSERT_NE(res.getName(), "Resource23");
    }
}

TEST(resource_tests, create_mesh_resource) {
    ResourceManager<Mesh, MeshHandle> mg;
    auto& m1 = mg.create("Mesh1");
    auto& m2 = mg.create("Mesh2");

    m1.addVertex();
    m1.addVertex();
    m1.addVertex();

    m1.createAttribute<AttributeTypes::FLOAT_ATTR>(0);
    float_attr& at1 = m1.getAttribute<AttributeTypes::FLOAT_ATTR>(0);
    at1[0] = 1.f;
    at1[1] = 2.f;
    at1[2] = 0.f;

    m1.createFace({0, 0, 1});

    mg.create("Mesh3");
    auto& m4 = mg.create("Mesh4");
    mg.create("Mesh5");
    mg.create("Mesh6");
    mg.destroy(m4.getRID());

    int i = 1;
    for (auto& res : mg) {
        ASSERT_EQ(res.getName(), "Mesh" + std::to_string(i));
        if (i == 3) i++;
        i++;
    }

    ASSERT_EQ(i, 7);
}
