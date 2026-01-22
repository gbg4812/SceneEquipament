#pragma once
#include "ParameterTypes.hpp"
#include "Resource.hpp"
#include "Shader.hpp"
namespace gbg {

class Material : public Resource {
   public:
    Material() : Resource() {}
    Material(std::string name, uint32_t rid) : Resource(name, rid) {}
    Material(ShaderHandle shader) : Resource() { setShader(shader); }

    void setShader(ShaderHandle shader) {

    };

   private:
    std::vector<parm_vt> _parameters;
    ShaderHandle _shader;
};

class MaterialHandle : public ResourceHandle {};

}  // namespace gbg
