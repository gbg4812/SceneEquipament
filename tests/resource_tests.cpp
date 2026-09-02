#include <gtest/gtest.h>

#include <ostream>

#include "AttributeTypes.hpp"
#include "Mesh.hpp"
#include "Resource.hpp"

using namespace gbg;

TEST(resource_tests, create_resource) {
    ResourceManager<Resource<ResourceHandle>, ResourceHandle> mg;
    // ou ou ou la referènci a r1 mor per culpa del relocació. per arreglar-ho
    // he allargat la mida inicial a 20 (és dificil que algu es guardi 20
    // referencies alhora no?)
    // millor tornem a handles igualment
    auto h1 = mg.create("Resource1");
    auto h2 = mg.create("Resource2");

    auto& r1 = mg.get(h1);
    auto& r2 = mg.get(h2);

    std::cout << r1.getName() << std::endl;

    ASSERT_EQ(r1.getName(), "Resource1");
    ASSERT_EQ(r2.getName(), "Resource2");
}

TEST(resource_tests, create_mesh_resource) {
    ResourceManager<Mesh, MeshHandle> mg;
    auto h1 = mg.create("Mesh1");
    auto h2 = mg.create("Mesh2");
    
    auto& m1 = mg.get(h1);
    auto& m2 = mg.get(h2);

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
    auto h4 = mg.create("Mesh4");
    auto& m4 = mg.get(h4);
    mg.create("Mesh5");
    mg.create("Mesh6");
    mg.destroy(m4.getRID());

    int i = 1;
    for (auto res : mg) {
        ASSERT_EQ(mg.get(res).getName(), "Mesh" + std::to_string(i));
        if (i == 3) i++;
        i++;
    }

    ASSERT_EQ(i, 7);
}
