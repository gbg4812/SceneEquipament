#include <gtest/gtest.h>

#include "Material.hpp"
#include "Shader.hpp"

using namespace gbg;

TEST(shader_tests, create_shader_parameters) {
    gbg::Shader shader("Shader1", 1);
    shader.loadFragShaderCode("./data/test.frag");
    shader.loadVertShaderCode("./data/test.vert");

    shader.addParameter(gbg::ParameterTypes::FLOAT_PARM);
    shader.addParameter(gbg::ParameterTypes::VEC2_PARM);

    auto& parameters = shader.getParameters();

    std::vector<ParameterTypes> parms;
    parms.push_back(ParameterTypes::FLOAT_PARM);
    parms.push_back(ParameterTypes::VEC2_PARM);

    ASSERT_EQ(parameters, parms);

    ASSERT_EQ(shader.getFragShaderCode(),
              "#version 330 core\nout vec4 fragColor;\nvoid main() { fragColor "
              "= vec4(1.0, 1.0, "
              "1.0, 1.0); }");
    ASSERT_EQ(shader.getVertShaderCode(),
              "#version 330 core\nlayout(location = 0)in vec3 vpos;\nvoid "
              "main() { gl_Position "
              "= vec4(1.0, 1.0, "
              "1.0, 1.0); }");
}
