#include <gtest/gtest.h>

#include "Shader.hpp"

using namespace gbg;

TEST(shader_tests, DISABLED_create_shader_parameters) {
    gbg::Shader shader("Shader1", 1);

    shader.addParameter(0.0f);
    shader.addParameter(vec2_t(0, 2));

    auto& parameters = shader.getParameters();
}
