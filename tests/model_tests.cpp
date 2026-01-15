#include <gtest/gtest.h>

#include <iostream>

#include "Resource.h"

TEST(model_tests, test_tests) {
    ResourceManager<Resource, ResourceHandle> mg;
    auto h0 = mg.create("Resource1");
    auto h1 = mg.create("Resource2");

    auto r1 = mg.get(h0);
    auto r2 = mg.get(h1);

    EXPECT_EQ(r1.getName(), "Resource1");
    EXPECT_EQ(r2.getName(), "Resource2");
}
