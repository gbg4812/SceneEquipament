#pragma once

#include <fstream>
#include <variant>

#include "Mesh.hpp"
#include "ParameterTypes.hpp"
#include "Resource.hpp"
#include "Texture.hpp"
#include "gbg_traits.hpp"

namespace gbg {
typedef std::variant<float_t, vec2_t, vec3_t, TextureHandle> parm_vt;

enum ParameterTypes { FLOAT_PARM = 0, VEC2_PARM, VEC3_PARM, TEXTURE_PARM };

template <ParameterTypes I>
using parm_vt_alt = std::variant_alternative_t<to_underlying(I), parm_vt>;

class Shader : public Resource {
   public:
    Shader() : Resource(){};
    Shader(std::string name, uint32_t rid) : Resource(name, rid){};

    // returns the position
    size_t addParameter(ParameterTypes I) {
        _parameters.push_back(I);
        return _parameters.size() - 1;
    };

    size_t addAttribute(uint loc, AttributeTypes I) {
        _attributes.emplace(loc, I);
        return _attributes.size() - 1;
    };

    void removeParameter(size_t pos) {
        auto it = _parameters.begin();
        for (size_t i = 0; i < pos; ++i, ++it) {
            _parameters.erase(it, it);
        }
    }

    void clear() {
        _parameters.clear();
        _attributes.clear();
    }

    void removeAttribute(uint loc) { _attributes.erase(loc); }

    const std::vector<ParameterTypes>& getParameters() const {
        return _parameters;
    }

    const std::map<uint, AttributeTypes>& getAttributes() const {
        return _attributes;
    }

    void setFragShaderCode(std::vector<uint32_t> code) {
        _frag_code = code;
    }

    void setVertShaderCode(std::vector<uint32_t> code) {
        _vert_code = code;
    }

    const std::vector<uint32_t>& getFragShaderCode() const { return _frag_code; }

    const std::vector<uint32_t>& getVertShaderCode() const { return _vert_code; }

   private:
    std::vector<ParameterTypes> _parameters;
    std::map<uint, AttributeTypes> _attributes;
    std::vector<uint32_t> _frag_code;
    std::vector<uint32_t> _vert_code;
};

class ShaderHandle : public ResourceHandle {
   public:
    ShaderHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index) {}
    ShaderHandle() : ResourceHandle() {}
};

}  // namespace gbg
