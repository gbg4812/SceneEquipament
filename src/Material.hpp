#pragma once

#include <utility>

#include "Resource.hpp"
#include "Shader.hpp"

namespace gbg {

class Material : public Resource {
   public:
    Material() : Resource() {}
    Material(std::string name, uint32_t rid) : Resource(name, rid) {}

    void setShader(ShaderHandle sh, const Shader& shader,
                   TextureHandle defaultTexture) {
        _parameters.clear();
        _shaderh = sh;

        // TODO: centralize defaults
        for (ParameterTypes parmT : shader.getParameters()) {
            switch (parmT) {
                case FLOAT_PARM:
                    _parameters.push_back(1.0f);
                    break;
                case VEC2_PARM:
                    _parameters.push_back(glm::vec2(1.0f));
                    break;
                case VEC3_PARM:
                    _parameters.push_back(glm::vec3(1.f));
                    break;
                case TEXTURE_PARM:
                    _parameters.push_back(defaultTexture);
                    break;
            }
        }
    }

    template <ParameterTypes I>
    void setParameterValue(size_t pos, parm_vt_alt<I> value) {
        _parameters[pos] = value;
    }

    template <ParameterTypes I>
    parm_vt_alt<I> getParameterValue(size_t pos) {
        return std::get<to_underlying(I)>(_parameters[pos]);
    }

    const std::vector<parm_vt>& getValues() { return _parameters; }
    ShaderHandle getShaderHandle() const { return _shaderh; }

   private:
    std::vector<parm_vt> _parameters;
    ShaderHandle _shaderh;
};

class MaterialHandle : public ResourceHandle {
   public:
    MaterialHandle() : ResourceHandle(){};
    MaterialHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index){};
};

}  // namespace gbg
