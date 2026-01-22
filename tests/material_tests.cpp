#include <gtest/gtest.h>

#include "Material.hpp"
#include "Shader.hpp"

using namespace gbg;

TEST(shader_tests, create_shader_parameters) {
    gbg::Shader shader("test.frag", "test.vert");

    shader.addParameter(gbg::ParameterTypes::FLOAT_PARM);
    shader.addParameter(gbg::ParameterTypes::VEC2_PARM);

    auto& parameters = shader.getParameters();

    std::vector<ParameterTypes> parms;
    parms.push_back(ParameterTypes::FLOAT_PARM);
    parms.push_back(ParameterTypes::VEC2_PARM);

    ASSERT_EQ(parameters, parms);

    ASSERT_EQ(shader.getFragShaderPath(), "test.frag");
    ASSERT_EQ(shader.getVertShaderPath(), "test.vert");
}
