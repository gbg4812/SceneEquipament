
#pragma once
#include "Material.hpp"
#include "Scene.hpp"
namespace gbg {

inline void setParametersFromShader(Scene& scene, Material& mat) {
    Shader& shader = scene.sh_mg.get(mat.getShaderHandle());

    mat.clearParameters();

    // TODO: centralize defaults
    for (ParameterTypes parmT : shader.getParameters()) {
        switch (parmT) {
            case gbg::ParameterTypes::INT_PARM:
                mat.appendParameter(1);
                break;
            case gbg::ParameterTypes::FLOAT_PARM:
                mat.appendParameter(1.0f);
                break;
            case gbg::ParameterTypes::VEC2_PARM:
                mat.appendParameter(glm::vec2(1.0f));
                break;
            case gbg::ParameterTypes::VEC3_PARM:
                mat.appendParameter(glm::vec3(1.f));
                break;
            case gbg::ParameterTypes::TEXTURE_PARM:
                mat.appendParameter(TextureHandle());
                break;
        }
    }
}

}  // namespace gbg
