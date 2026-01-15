#pragma once
#include <list>
#include <map>
#include <variant>

#include "AttributeTypes.h"
#include "gbg_traits.h"

namespace gbg {

typedef std::variant<float_attr, vec2_attr, vec3_attr> attr_vt;
typedef std::list<uint> face_t;

enum AttributeTypes { FLOAT_ATTR = 0, VEC2_ATTR, VEC3_ATTR };

template <AttributeTypes I>
using attr_vt_alt = std::variant_alternative_t<to_underlying(I), attr_vt>;

class Mesh {
    std::map<uint, attr_vt> _attributes;
    std::vector<face_t> _faces;

    struct _DefaultVertex_V {
        void operator()(float_attr& v) { v.push_back(0); }
        void operator()(vec2_attr& v) { v.push_back(glm::vec2(0)); }
        void operator()(vec3_attr& v) { v.push_back(glm::vec3(0)); }
    };

   public:
    template <AttributeTypes I>
    attr_vt_alt<I>& getAttribute(uint location) {
        return std::get<to_underlying(I)>(_attributes[location]);
    }

    template <AttributeTypes I>
    void createAttribute(uint location) {
        _attributes.insert({location, attr_vt_alt<I>()});
    }

    void addVertex() {
        for (auto& attrb : _attributes) {
            std::visit<void>(_DefaultVertex_V{}, attrb.second);
        }
    }

    void createFace(std::list<uint> vertices) { _faces.push_back(vertices); }

    std::vector<face_t>& getFaces() { return _faces; }
};

}  // namespace gbg
