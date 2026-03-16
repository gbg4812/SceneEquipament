#pragma once

#include <cstdio>
#include <list>
#include <variant>
#include <vector>

#include "Model.hpp"

namespace gbg {

enum SceneObjectTypes {
    EMPTY = 0,
    MODEL = 1,
};

typedef std::variant<std::monostate, ModelHandle> scene_obj_vt;

template <SceneObjectTypes I>
using scene_obj_alt =
    std::variant_alternative_t<to_underlying(I), scene_obj_vt>;

struct SceneTreeHandle {
    SceneTreeHandle(size_t idx) : idx(idx) {}
    size_t idx;
};

struct SceneTreeNode {
    SceneTreeHandle parent = 0;
    std::list<SceneTreeHandle> children;
    glm::mat4x4 transform;
    scene_obj_vt resource;
    size_t id;
};

class SceneTree {
    SceneTree(size_t initial_size)
        : _nodes(initial_size), _next_id(0), _next_idx(0) {};

   private:
    std::vector<SceneTreeNode> _nodes;
    std::list<size_t> _empty_lst;
    size_t _next_idx;
    size_t _next_id;
};

}  // namespace gbg
