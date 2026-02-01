#pragma once
#include <list>
#include <map>
#include <variant>

#include "AttributeTypes.hpp"
#include "Resource.hpp"
#include "gbg_traits.hpp"

namespace gbg {

typedef std::variant<float_attr, vec2_attr, vec3_attr> attr_vt;
typedef std::list<uint> face_t;

enum AttributeTypes { FLOAT_ATTR = 0, VEC2_ATTR, VEC3_ATTR };

template <AttributeTypes I>
using attr_vt_alt = std::variant_alternative_t<to_underlying(I), attr_vt>;

class Mesh : public Resource {
    std::map<uint, attr_vt> _attributes;
    std::vector<face_t> _faces;
    size_t _nvertices = 0;

    struct _DefaultVertex_V {
        void operator()(float_attr& v) { v.push_back(0); }
        void operator()(vec2_attr& v) { v.push_back(glm::vec2(0)); }
        void operator()(vec3_attr& v) { v.push_back(glm::vec3(0)); }
    };

   public:
    Mesh() : Resource() {}
    Mesh(std::string name, uint32_t rid) : Resource(name, rid) {}

    template <AttributeTypes I>
    attr_vt_alt<I>& getAttribute(uint location) {
        return std::get<to_underlying(I)>(_attributes.at(location));
    }

    template <AttributeTypes I>
    void createAttribute(uint location) {
        _attributes.insert({location, attr_vt_alt<I>(_nvertices)});
    }

    void addVertex() {
        _nvertices++;
        for (auto& attrb : _attributes) {
            std::visit<void>(_DefaultVertex_V{}, attrb.second);
        }
    }

    const std::map<uint, attr_vt>& getAttributes() const { return _attributes; }

    void createFace(std::list<uint> vertices) { _faces.push_back(vertices); }

    const std::vector<face_t>& getFaces() const { return _faces; }
};

class MeshHandle : public ResourceHandle {
   public:
    MeshHandle() : ResourceHandle() {};
    MeshHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index) {};
};

}  // namespace gbg
