#pragma once

#include <cstdio>
#include <variant>

#include "Model.hpp"
#include "Resource.hpp"
#include "gbg_traits.hpp"

namespace gbg {

enum SceneObjectTypes {
    EMPTY = 0,
    MODEL = 1,
};

typedef std::variant<std::monostate, ModelHandle> scene_obj_vt;

template <SceneObjectTypes I>
using scene_obj_alt =
    std::variant_alternative_t<to_underlying(I), scene_obj_vt>;

struct SceneTreeHandle : public ResourceHandle {
    SceneTreeHandle() : ResourceHandle() {}
    SceneTreeHandle(size_t _nextid) : ResourceHandle() {}
    SceneTreeHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index) {}
};

class SceneTreeNode : public Resource {
    // TODO: rid 0 vol dir que és null
   public:
    SceneTreeNode() : Resource(){};
    SceneTreeNode(std::string name, uint32_t rid) : Resource(name, rid) {}

    template <SceneObjectTypes I>
    scene_obj_alt<I> getResourceH() {
        return std::get<to_underlying(I)>(_resource);
    }

    scene_obj_vt getResourceH() { return _resource; }

    void setResource(scene_obj_vt object) { _resource = object; }

   public:
    SceneTreeHandle parentH;
    SceneTreeHandle childH;
    SceneTreeHandle nextH;
    glm::mat4x4 transform = glm::mat4(1.0f);

   private:
    scene_obj_vt _resource;
};

class SceneTreeManager
    : public ResourceManager<SceneTreeNode, SceneTreeHandle> {
   public:
    SceneTreeManager(size_t initial_size = 0) : ResourceManager(initial_size) {}

    void prependChild(SceneTreeHandle parent, SceneTreeHandle child) {
        SceneTreeNode& parentn = this->get(parent);
        SceneTreeNode& childn = this->get(child);
        childn.nextH = parentn.childH;
        parentn.childH = child;
        childn.parentH = parent;
    }
};

}  // namespace gbg
