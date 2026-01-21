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
    Shader(std::string name, uint32_t rid) : Resource(name, rid) {}

    // returns the position
    size_t addParameter(ParameterTypes I) {
        _parameters.push_back(I);
        return _parameters.size() - 1;
    };

    void removeParameter(size_t pos) { _parameters.remove(pos); }

   private:
    std::vector<ParameterTypes> _parameters;
};
}  // namespace gbg
