#pragma once

#include <cstdio>
#include <variant>

#include "Camera.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Resource.hpp"
#include "gbg_traits.hpp"
#include "macros.hpp"

namespace gbg {

enum ResourceTypes {
    EMPTY = 0,
    MODEL,
    CAMERA,
    LIGHT,
};

typedef std::variant<std::monostate, ModelHandle, CameraHandle, LightHandle>
    resources_vt;

template <ResourceTypes I>
using scene_obj_alt =
    std::variant_alternative_t<to_underlying(I), resources_vt>;

RESOURCE_HANDLE(DependencyTreeNode);

class DependencyTreeNode : public Resource {
    // TODO: rid 0 vol dir que és null
   public:
    RESOURCE_CONSTR(DependencyTreeNode);

    template <ResourceTypes I>
    scene_obj_alt<I> getResourceH() {
        return std::get<to_underlying(I)>(_resource);
    }

    resources_vt getResourceH() { return _resource; }

    void setResource(resources_vt object) { _resource = object; }

   public:
    DependencyTreeNodeHandle childH;
    DependencyTreeNodeHandle nextH;
    DependencyFlags childFlags;
    DependencyFlags nextFlags;

   private:
    resources_vt _resource;
};

class DependencyTreeManager
    : public ResourceManager<DependencyTreeNode, DependencyTreeNodeHandle> {
   public:
    DependencyTreeManager(size_t initial_size = 0)
        : ResourceManager(initial_size) {}

    void prependChild(DependencyTreeNodeHandle parent,
                      DependencyTreeNodeHandle child) {
        DependencyTreeNode& parentn = this->get(parent);
        DependencyTreeNode& childn = this->get(child);
        childn.nextH = parentn.childH;
        parentn.childH = child;
        childn.parentH = parent;
    }
};

}  // namespace gbg
