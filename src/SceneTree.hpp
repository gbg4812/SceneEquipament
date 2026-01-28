#pragma once

#include <cstdio>
#include <list>
#include <variant>

#include "DepDataHandle.hpp"
#include "Model.hpp"
#include "glm/glm.hpp"

namespace gbg {

enum SceneObjectTypes {
    EMPTY = 0,
    MODEL = 1,
};

enum ObjectFlags {
    DIRTY = 0x01,
};

typedef std::variant<std::monostate, ModelHandle> scene_obj_vt;

template <SceneObjectTypes I>
using scene_obj_alt =
    std::variant_alternative_t<to_underlying(I), scene_obj_vt>;

class SceneTree {
   public:
    ~SceneTree() {
        for (SceneTree* child : _children) {
            delete child;
        }
    }

    const std::list<SceneTree*>& getChildren() { return _children; }
    SceneTree* getParent() { return _parent; }
    const glm::mat4x4& getLocalTransform() { return _transform; }

    SceneObjectTypes getType() { return (SceneObjectTypes)_resource.index(); }

    template <SceneObjectTypes I>
    scene_obj_alt<I> getResourceHandle() {
        return std::get<I>(_resource);
    }

    const scene_obj_vt& getResourceHandle() { return _resource; }

    template <SceneObjectTypes I>
    void setSceneObjectHandle(scene_obj_alt<I> object) {
        _resource = object;
    }

    void multiplyLocalTransform(const glm::mat4x4& transform) {
        _transform = _transform * transform;
    }

    glm::mat4x4 computeTransform() {
        if (_parent) return _parent->computeTransform() * _transform;
        return _transform;
    };

    template <SceneObjectTypes I>
    SceneTree* addChild(scene_obj_alt<I> child) {
        SceneTree* newT = new SceneTree();
        newT->setSceneObjectHandle<I>(child);
        _children.push_back(newT);
        return *_children.end();
    }

    void removeChild(SceneTree* child) {
        _children.remove(child);
        delete child;
    }

    void setParent(SceneTree* parent) {
        if (_parent) _parent->removeChild(this);
        _parent = parent;
    }

    void setFlags(ObjectFlags flags) { _handle.flags = flags; }
    DepDataHandle getDepDataHandle() { return _handle; }

   private:
    SceneTree* _parent = nullptr;
    std::list<SceneTree*> _children;
    glm::mat4x4 _transform;
    scene_obj_vt _resource;
    DepDataHandle _handle;
};

}  // namespace gbg
