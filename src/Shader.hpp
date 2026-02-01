#pragma once

#include <fstream>
#include <variant>

#include "Mesh.hpp"
#include "ParameterTypes.hpp"
#include "Resource.hpp"
#include "gbg_traits.hpp"

namespace gbg {
typedef std::variant<float_t, vec2_t, vec3_t> parm_vt;

enum ParameterTypes { FLOAT_PARM = 0, VEC2_PARM, VEC3_PARM };

template <ParameterTypes I>
using parm_vt_alt = std::variant_alternative_t<to_underlying(I), parm_vt>;

class Shader : public Resource {
   public:
    Shader(std::string name, uint32_t rid) : Resource(name, rid) {};

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

    void removeAttribute(uint loc) { _attributes.erase(loc); }

    const std::vector<ParameterTypes>& getParameters() const {
        return _parameters;
    }

    const std::map<uint, AttributeTypes>& getAttributes() const {
        return _attributes;
    }

    void loadFragShaderCode(std::string file_path) {
        std::ifstream ifs(file_path);

        char rdaux[64];

        while (ifs) {
            ifs.read(rdaux, 64);
            _frag_code.append(rdaux, ifs.gcount());
        }
    }

    void loadVertShaderCode(std::string file_path) {
        std::ifstream ifs(file_path);

        char rdaux[64];

        while (ifs) {
            ifs.read(rdaux, 64);
            _vert_code.append(rdaux, ifs.gcount());
        }
    }

    const std::string& getFragShaderCode() const { return _frag_code; }

    const std::string& getVertShaderCode() const { return _vert_code; }

   private:
    std::vector<ParameterTypes> _parameters;
    std::map<uint, AttributeTypes> _attributes;
    std::string _frag_code;
    std::string _vert_code;
};

class ShaderHandle : public ResourceHandle {
   public:
    ShaderHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index) {}
    ShaderHandle() : ResourceHandle() {}
};

}  // namespace gbg
