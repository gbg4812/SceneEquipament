
#pragma once
#include <ranges>

#include "Material.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
namespace gbg {

inline void setParametersFromShader(Scene& scene, Material& mat) {
    Shader& shader = scene.sh_mg.get(mat.getShaderHandle());

    auto& vals = mat.getValues();
    // TODO: centralize defaults
    for (auto [idx, parmT] : shader.getParameters() | std::views::enumerate) {
        if (idx >= (int)vals.size() || (int)vals[idx].index() != (int)parmT) {
            mat.clearParameters(idx);
            switch (parmT) {
                case gbg::ParameterTypes::INT:
                    mat.appendParameter(1);
                    break;
                case gbg::ParameterTypes::FLOAT:
                    mat.appendParameter(1.0f);
                    break;
                case gbg::ParameterTypes::VEC2:
                    mat.appendParameter(glm::vec2(1.0f));
                    break;
                case gbg::ParameterTypes::VEC3:
                    mat.appendParameter(glm::vec3(1.f));
                    break;
                case gbg::ParameterTypes::TEXTURE:
                    mat.appendParameter(TextureHandle());
                    break;
            }
        }
    }
}

}  // namespace gbg
