#pragma once

#include <map>
#include <stdexcept>
#include <string>

#include "Resource.hpp"
#include "macros.hpp"

namespace gbg {

typedef uint32_t ResourceType;

typedef uint32_t DependencyMask;

const DependencyMask EMPTY_MASK = 0;

RESOURCE_HANDLE(DependencyTreeNode);

struct Dependency {
    DependencyTreeNodeHandle dependent;
    DependencyMask mask_pre = EMPTY_MASK;
    DependencyMask mask_cons = EMPTY_MASK;

    bool operator<(const Dependency& other) const {
        if (dependent.getIndex() < other.dependent.getIndex()) return true;
        if (dependent.getIndex() > other.dependent.getIndex()) return false;
        if (mask_pre < other.mask_pre) return true;
        if (mask_pre > other.mask_pre) return false;
        if (mask_cons < other.mask_cons) return true;
        return false;
    }
    bool operator==(const Dependency& other) const {
        return (dependent == other.dependent) &&
               (mask_cons == other.mask_cons) && (mask_pre == other.mask_pre);
    }
};

class DependencyTreeNode : public Resource<DependencyTreeNodeHandle> {
    // TODO: rid 0 vol dir que és null
   public:
    RESOURCE_CONSTR(DependencyTreeNode);

   public:
    std::map<Dependency, bool> dependents;

    ResourceType type = -1;
    ResourceHandle represented;
    DependencyMask flags = 0;
    uint32_t modifiedParents = 0;
};

class DependencyTreeManager
    : public ResourceManager<DependencyTreeNode, DependencyTreeNodeHandle> {
   public:
    DependencyTreeManager(size_t chunk_size = 64)
        : ResourceManager(chunk_size) {}

    DependencyTreeNode& create(ResourceHandle represented, ResourceType type) {
        if (not represented.getIndex()) {
            throw std::runtime_error("Represented handle can't be empty");
        }
        auto& n =
            ResourceManager<DependencyTreeNode, DependencyTreeNodeHandle>::
                create("Representant" + std::to_string(represented.getRID()));
        n.type = type;
        n.represented = represented;
        n.flags = EMPTY_MASK;
        return n;
    };

    void propagateChange(DependencyTreeNodeHandle handle, DependencyMask mask) {
        auto& n = get(handle);
        if (mask == EMPTY_MASK or ((n.flags | mask) == n.flags)) return;

        if (not n.flags) {
            _modified.push_back(handle);
        }
        n.flags |= mask;

        for (auto& [dep, vis] : n.dependents) {
            if (dep.mask_pre & mask) {
                get(dep.dependent).modifiedParents += 1 - vis;
                vis = true;
                propagateChange(dep.dependent, dep.mask_cons);
            }
        }
    }

    const std::list<DependencyTreeNodeHandle>& getModified() {
        if (_ordered.size() != _modified.size()) {
            _ordered.clear();
            for (auto n : _modified) {
                if (get(n).modifiedParents == 0) _ordered.push_back(n);
            }

            auto ord_it = _ordered.begin();
            while (ord_it != _ordered.end()) {
                auto& n = get(*ord_it);
                for (auto& [dh, vis] : n.dependents) {
                    auto& dn = get(dh.dependent);
                    dn.modifiedParents -= vis;
                    vis = 0;
                    if (dn.modifiedParents == 0) {
                        _ordered.push_back(dh.dependent);
                    }
                }
                ++ord_it;
            }

            if (_modified.size() != _ordered.size())
                throw std::logic_error(
                    "If the algorithm is correct this should never happen");
        }
        return _ordered;
    }

    void clearModified() {
        for (auto h : _modified) {
            get(h).flags = EMPTY_MASK;
        }
        _modified.clear();
        _ordered.clear();
    }

   private:
    std::list<DependencyTreeNodeHandle> _modified;
    std::list<DependencyTreeNodeHandle> _ordered;
};

}  // namespace gbg
