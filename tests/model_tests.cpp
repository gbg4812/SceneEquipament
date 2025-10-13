#include <gtest/gtest.h>

TEST(model_tests, test_tests) {
    EXPECT_EQ(10, 20) << "They are not equal!" << std::endl;
}
