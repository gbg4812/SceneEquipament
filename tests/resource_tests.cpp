#include <gtest/gtest.h>

#include <iostream>

#include "AttributeTypes.h"
#include "Mesh.h"
#include "Resource.h"

using namespace gbg;

TEST(resource_tests, create_resource) {
    ResourceManager<Resource, ResourceHandle> mg;
    auto h0 = mg.create("Resource1");
    auto h1 = mg.create("Resource2");

    auto r1 = mg.get(h0);
    auto r2 = mg.get(h1);

    EXPECT_EQ(r1.getName(), "Resource1");
    EXPECT_EQ(r2.getName(), "Resource2");
}

TEST(resource_tests, create_mesh_resource) {
    ResourceManager<Mesh, MeshHandle> mg;
    auto h1 = mg.create("Mesh1");
    auto h2 = mg.create("Mesh2");

    Mesh& m1 = mg.get(h1);
    Mesh& m2 = mg.get(h2);

    m1.addVertex();
    m1.addVertex();
    m1.addVertex();

    m1.createAttribute<AttributeTypes::FLOAT_ATTR>(0);
    float_attr& at1 = m1.getAttribute<AttributeTypes::FLOAT_ATTR>(0);
    at1[0] = 1.f;
    at1[1] = 2.f;
    at1[2] = 0.f;

    m1.createFace({0, 0, 1});
}
