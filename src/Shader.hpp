#pragma once

#include <cstdint>
#include <variant>

#include "DResource.hpp"
#include "Mesh.hpp"
#include "ParameterTypes.hpp"
#include "Texture.hpp"
#include "gbg_traits.hpp"
#include "macros.hpp"

namespace gbg {
typedef std::variant<int32_t, float_t, vec2_t, vec3_t, TextureHandle> parm_vt;

enum class ParameterTypes { INT, FLOAT, VEC2, VEC3, TEXTURE, _MAX };

inline std::array<std::string_view, to_underlying(ParameterTypes::_MAX)>
    parmTypeToString = {"int", "float", "vec2", "vec3", "texture"};

enum class PrimitiveInterpretation { TRIANGLES, POINTS, LINES, _MAX };

enum class ShaderTypes { VERTEX, FRAGMENT, _MAX };

template <ParameterTypes I>
using parm_vt_alt = std::variant_alternative_t<to_underlying(I), parm_vt>;

RESOURCE_HANDLE(Shader);

class Shader : public DResource<ShaderHandle> {
   public:
    DRESOURCE_CONSTR(Shader)

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

    void setCode(const std::vector<uint32_t>& code, ShaderTypes shader_type) {
        _codes[to_underlying(shader_type)] = code;
    }

    const std::vector<uint32_t>& getCode(ShaderTypes shader_type) const {
        return _codes[to_underlying(shader_type)];
    }

    PrimitiveInterpretation topology = PrimitiveInterpretation::TRIANGLES;
    bool shadow = true;

   private:
    std::vector<ParameterTypes> _parameters;
    std::map<uint, AttributeTypes> _attributes;
    std::array<std::vector<uint32_t>, to_underlying(ShaderTypes::_MAX)> _codes;
};

RESOURCE_MANAGER(Shader);

}  // namespace gbg
