#include <gtest/gtest.h>

#include <iostream>

#include "Mesh.h"

TEST(model_tests, attrib_add_get) {
    gbg::Mesh mesh;
    using AT = gbg::AttributeTypes;
    mesh.createAttribute<AT::FLOAT_ATTR>(1);
    mesh.createAttribute<AT::VEC2_ATTR>(2);
    mesh.createAttribute<AT::VEC3_ATTR>(3);

    for (int i = 0; i < 100; i++) {
        mesh.addVertex();
    }

    {
        auto& vf = mesh.getAttribute<AT::FLOAT_ATTR>(1);
        auto& v2 = mesh.getAttribute<AT::VEC2_ATTR>(2);
        auto& v3 = mesh.getAttribute<AT::VEC3_ATTR>(3);

        vf[99] = 3.0;

        v2[0] = glm::vec2(1, 2);
    }

    {
        auto& vf = mesh.getAttribute<AT::FLOAT_ATTR>(1);
        auto& v2 = mesh.getAttribute<AT::VEC2_ATTR>(2);
        auto& v3 = mesh.getAttribute<AT::VEC3_ATTR>(3);

        ASSERT_EQ(3.0, vf[99]);
        ASSERT_EQ(glm::vec2(1, 2), v2[0]);
    }
}

TEST(model_tests, face_add_get) {
    gbg::Mesh mesh;
    using AT = gbg::AttributeTypes;
    mesh.createAttribute<AT::FLOAT_ATTR>(1);
    mesh.createAttribute<AT::VEC2_ATTR>(2);
    mesh.createAttribute<AT::VEC3_ATTR>(3);

    for (int i = 0; i < 100; i++) {
        mesh.addVertex();
    }

    mesh.createFace({1, 2, 3});
    mesh.createFace({1, 3, 4});

    std::vector<gbg::face_t> faces = mesh.getFaces();

    gbg::face_t face1 = {1, 2, 3};
    ASSERT_EQ(face1, faces[0]);

    gbg::face_t face2 = {1, 3, 4};
    ASSERT_EQ(face1, faces[0]);
}
