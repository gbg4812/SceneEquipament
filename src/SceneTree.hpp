#pragma once

#include <cstdio>
#include <variant>

#include "Camera.hpp"
#include "Model.hpp"
#include "Resource.hpp"
#include "gbg_traits.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace gbg {

enum SceneObjectTypes {
    EMPTY = 0,
    MODEL,
    CAMERA,
};

typedef std::variant<std::monostate, ModelHandle, CameraHandle> scene_obj_vt;

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
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 translation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 getLocalTransform() {
        glm::mat4 trans(1.0f);
        trans = glm::translate(trans, translation);
        trans = glm::rotate(trans, rotation.y, {0.0, 1.0, 0.0});
        trans = glm::rotate(trans, rotation.x, {1.0, 0.0, 0.0});
        trans = glm::rotate(trans, rotation.z, {0.0, 0.0, 1.0});
        trans = glm::scale(trans, scale);
        return trans;
    }

    void localTranslate(glm::vec3 offset) {
        glm::mat4 trans(1.0f);
        trans = glm::rotate(trans, rotation.y, {0.0, 1.0, 0.0});
        translation += glm::vec3(trans * glm::vec4(offset, 1.));
    }

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

    glm::mat4 getGlobalTransform(SceneTreeHandle h) {
        SceneTreeNode& node = this->get(h);
        if (node.parentH)
            return node.getLocalTransform() * getGlobalTransform(node.parentH);
        else
            return node.getLocalTransform();
    }
};

}  // namespace gbg
