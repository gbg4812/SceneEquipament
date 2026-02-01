#pragma once

#include "Resource.hpp"
#include "Shader.hpp"

namespace gbg {

class Material : public Resource {
   public:
    Material(std::string name, uint32_t rid) : Resource(name, rid) {}

    void setShader(ShaderHandle sh, const Shader& shader) {
        _parameters.clear();

        // TODO: centralize defaults
        for (ParameterTypes parmT : shader.getParameters()) {
            switch (parmT) {
                case FLOAT_PARM:
                    _parameters.push_back(0.0f);
                    break;
                case VEC2_PARM:
                    _parameters.push_back(glm::vec2());
                case VEC3_PARM:
                    _parameters.push_back(glm::vec3());
            }
        }
    }

    template <ParameterTypes I>
    void setParameterValue(size_t pos, parm_vt_alt<I> value) {
        _parameters[pos] = value;
    }

    const std::vector<parm_vt>& getValues() { return _parameters; }
    ShaderHandle getShaderHandle() const { return _shaderh; }

   private:
    std::vector<parm_vt> _parameters;
    ShaderHandle _shaderh;
};

class MaterialHandle : public ResourceHandle {
   public:
    MaterialHandle() : ResourceHandle() {};
    MaterialHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index) {};
};

}  // namespace gbg
