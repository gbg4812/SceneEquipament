#pragma once

#include <variant>

#include "Resource.hpp"
#include "Shader.hpp"

namespace gbg {

class Scene;

class Material : public Resource {
   public:
    Material() : Resource() {}
    Material(std::string name, uint32_t rid) : Resource(name, rid) {}
    Material(ShaderHandle shader, const Scene& scene) : Resource() {
        setShader(shader, scene);
    }

    void setShader(ShaderHandle shader, const Scene& scene) {
        ResourceManager<Shader, ShaderHandle>& sm = scene.getShaderManager();
        Shader& sh = sm.get(shader);
        _parameters.clear();

        // TODO: centralize defaults
        for (ParameterTypes parmT : sh.getParameters()) {
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
        _shader = shader;
    }

    template <ParameterTypes I>
    void setParameterValue(size_t pos, parm_vt_alt<I> value) {
        _parameters[pos] = value;
    }

    const std::vector<parm_vt>& getValues() { return _parameters; }

   private:
    std::vector<parm_vt> _parameters;
    ShaderHandle _shader;
};

class MaterialHandle : public ResourceHandle {};

}  // namespace gbg
