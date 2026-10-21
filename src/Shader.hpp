#pragma once

#include <cstdint>
#include <variant>

#include "DResource.hpp"
#include "Mesh.hpp"
#include "ParameterTypes.hpp"
#include "Texture.hpp"
#include "extras/File.hpp"
#include "gbg_traits.hpp"
#include "macros.hpp"

namespace gbg {
typedef std::variant<int32_t, float_t, vec2_t, vec3_t, TextureHandle> parm_vt;

enum class ParameterTypes { INT, FLOAT, VEC2, VEC3, TEXTURE, _MAX };

inline std::array<std::string_view, to_underlying(ParameterTypes::_MAX)>
    parmTypeToString = {"int", "float", "vec2", "vec3", "texture"};

enum class PrimitiveInterpretation { TRIANGLES, POINTS, LINES, _MAX };

enum class ShaderTypeFlags {
    NONE = 0,
    SHADER_TYPE_VERTEX = 1 << 0,
    SHADER_TYPE_FRAGMENT = 1 << 1,
    SHADER_TYPE_MASK_COMBINED = SHADER_TYPE_VERTEX & SHADER_TYPE_FRAGMENT,
    _MAX
};

inline ShaderTypeFlags operator&(ShaderTypeFlags flags1, ShaderTypeFlags flags2) {
    return (ShaderTypeFlags)(to_underlying(flags1) & to_underlying(flags2));
}
inline ShaderTypeFlags operator|(ShaderTypeFlags flags1, ShaderTypeFlags flags2) {
    return (ShaderTypeFlags)(to_underlying(flags1) | to_underlying(flags2));
}

template <ParameterTypes I>
using parm_vt_alt = std::variant_alternative_t<to_underlying(I), parm_vt>;

RESOURCE_HANDLE(Shader);

struct ShaderCode {
    std::vector<uint32_t> code;
    FileHandle source_file;
    ShaderTypeFlags flags;
};

struct ShaderParameter {
    parm_vt default_value;
    std::string name;
};

class Shader : public DResource<ShaderHandle> {
   public:
    DRESOURCE_CONSTR(Shader)

    // returns the position
    size_t addParameter(parm_vt default_value, std::string name = "") {
        _parameters.push_back({default_value, name});
        return _parameters.size() - 1;
    };

    size_t addAttribute(uint loc, AttributeTypes I) {
        _attributes.emplace(loc, I);
        return _attributes.size() - 1;
    };

    void removeParameter(size_t idx) {
        _parameters.erase(_parameters.begin() + idx);
    }

    void clear() {
        _parameters.clear();
        _attributes.clear();
    }

    void removeAttribute(uint loc) { _attributes.erase(loc); }

    const std::vector<ShaderParameter>& getParameters() const {
        return _parameters;
    }

    const std::map<uint, AttributeTypes>& getAttributes() const {
        return _attributes;
    }

    // all code related
    int appendCodeFile(FileHandle file, ShaderTypeFlags flags = ShaderTypeFlags::NONE) {
        ShaderCode code{};
        code.flags = flags;
        code.source_file = file;
        _codes.push_back(code);
        return _codes.size() - 1;
    }

    void setByteCode(int idx, const std::vector<uint32_t>& code) {
        _codes[idx].code = code;
    }

    void setCodeTypeFlags(int idx, ShaderTypeFlags flags) {
        _codes[idx].flags = _codes[idx].flags | flags;
    }

    const std::vector<uint32_t>& getByteCode(int idx) const {
        return _codes[idx].code;
    }

    FileHandle getCodeFile(int idx) const {
        return _codes[idx].source_file;
    }

    const std::vector<ShaderCode>& getCodes() {
        return _codes;
    }

    PrimitiveInterpretation topology = PrimitiveInterpretation::TRIANGLES;
    bool shadow = true;

   private:
    std::vector<ShaderParameter> _parameters;
    std::map<uint, AttributeTypes> _attributes;
    std::vector<ShaderCode> _codes;
};

RESOURCE_MANAGER(Shader);

}  // namespace gbg
