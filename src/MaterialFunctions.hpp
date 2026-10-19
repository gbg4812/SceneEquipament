
#pragma once
#include <ranges>

#include "Material.hpp"
#include "Shader.hpp"
namespace gbg {

inline void setParametersFromShader(Shader& shader, Material& mat) {
    auto& vals = mat.getValues();
    for (auto [idx, parm] : shader.getParameters() | std::views::enumerate) {
        if (idx >= vals.size()) {
            mat.appendParameter(parm.default_value);
        } else if (vals[idx].index() != parm.default_value.index()) {
            mat.setParameter(idx, parm.default_value);
        }
    }

    mat.clearParameters(shader.getParameters().size());
}

}  // namespace gbg
