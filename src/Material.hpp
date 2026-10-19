#pragma once

#include "DResource.hpp"
#include "Shader.hpp"
#include "macros.hpp"

namespace gbg {

RESOURCE_HANDLE(Material);

class Material : public DResource<MaterialHandle> {
   public:
    DRESOURCE_CONSTR(Material)

    void setShader(ShaderHandle sh) { _shaderh = sh; }

    void removeParameter(size_t idx) {
        _parameters.erase(_parameters.begin() + idx);
    }

    void clearParameters(size_t idx = 0) {
        _parameters.erase(_parameters.begin() + idx, _parameters.end());
    }

    size_t appendParameter(parm_vt init_value) {
        _parameters.push_back(init_value);
        return _parameters.size() - 1;
    }

    template <ParameterTypes I>
    void setParameterValue(size_t pos, parm_vt_alt<I> value) {
        _parameters.at(pos) = value;
    }

    void setParameter(size_t pos, parm_vt value) {
        _parameters.at(pos) = value;
    }

    const std::vector<parm_vt>& getValues() const { return _parameters; }
    ShaderHandle getShaderHandle() const { return _shaderh; }

   private:
    std::vector<parm_vt> _parameters;
    ShaderHandle _shaderh;
};

RESOURCE_MANAGER(Material);

}  // namespace gbg
