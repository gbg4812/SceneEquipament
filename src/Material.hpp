#pragma once

#include <utility>

#include "DResource.hpp"
#include "Shader.hpp"
#include "macros.hpp"

namespace gbg {

class Material : public DResource {
   public:
    Material() : DResource() {}
    Material(std::string name, uint32_t rid) : DResource(name, rid) {}

    void setShader(ShaderHandle sh) {
        _shaderh = sh;
    }

    void clearParameters() {
        _parameters.clear();
    }


    void appendParameter(parm_vt init_value) {
        _parameters.push_back(init_value);
    }

    template <ParameterTypes I>
    void setParameterValue(size_t pos, parm_vt_alt<I> value) {
        _parameters[pos] = value;
    }

    template <ParameterTypes I>
    parm_vt_alt<I> getParameterValue(size_t pos) {
        return std::get<to_underlying(I)>(_parameters[pos]);
    }

    const std::vector<parm_vt>& getValues() const { return _parameters; }
    ShaderHandle getShaderHandle() const { return _shaderh; }

   private:
    std::vector<parm_vt> _parameters;
    ShaderHandle _shaderh;
};

RESOURCE_HANDLE(Material);

RESOURCE_MANAGER(Material);


}  // namespace gbg
