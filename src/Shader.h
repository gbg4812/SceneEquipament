#pragma once

#include <variant>

#include "ParameterTypes.h"
#include "Resource.h"

namespace gbg {
typedef std::variant<float_t, vec2_t, vec3_t> parm_vt;

enum ParameterTypes { FLOAT_PARM = 0, VEC2_PARM, VEC3_PARM };

class Shader : public Resource {
   public:
    Shader() : Resource() {}
    Shader(std::string frag_path, std::string vert_path)
        : Resource(), _frag_path(frag_path), _vert_path(vert_path) {}
    Shader(std::string frag_path, std::string vert_path, std::string name,
           uint32_t rid)
        : Resource(name, rid), _frag_path(frag_path), _vert_path(vert_path) {}

    // returns the position
    size_t addParameter(ParameterTypes I) {
        _parameters.push_back(I);
        return _parameters.size() - 1;
    };

    void removeParameter(size_t pos) {
        auto it = _parameters.begin();
        for (int i = 0; i < pos; ++i, ++it) {
            _parameters.erase(it, it);
        }
    }

    const std::vector<ParameterTypes>& getParameters() { return _parameters; }

    const std::string& getFragShaderPath() { return _frag_path; }

    const std::string& getVertShaderPath() { return _vert_path; }

   private:
    std::vector<ParameterTypes> _parameters;
    std::string _frag_path;
    std::string _vert_path;
};
}  // namespace gbg
