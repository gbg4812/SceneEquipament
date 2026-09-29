#pragma once
#include "DResource.hpp"
#include "DependencyTree.hpp"

namespace gbg {

template <typename T>
inline DependencyTreeNodeHandle createRepresentative(
    DependencyTreeManager& manager, DResource<T>& res, ResourceType type,
    DependencyMask initMask = 0) {
    // create representative
    DependencyTreeNode& dtn = manager.create(res.getHandle(), type);
    res.representative = dtn.getHandle();

    if (initMask) {
        manager.propagateChange(dtn.getRID(), initMask);
    }

    return dtn.getRID();
}

inline void setDependent(DependencyTreeManager& manager,
                         DependencyTreeNodeHandle dependent_h,
                         DependencyMask mask_cons,
                         DependencyTreeNodeHandle depended_h,
                         DependencyMask mask_pre) {
    auto it = manager.get(depended_h)
                  .dependents.insert({{
                                          .dependent = dependent_h,
                                          .mask_pre = mask_pre,
                                          .mask_cons = mask_cons,
                                      },
                                      false});
    if (manager.get(depended_h).flags & mask_pre) {
        it.first->second = true;
        manager.get(dependent_h).modifiedParents += 1;
        manager.propagateChange(dependent_h, mask_cons);
    }
}

template <typename TH>
inline void deleteDependent(DependencyTreeManager& manager,
                            DependencyTreeNodeHandle dependent_h,
                            DependencyMask mask_cons,
                            DependencyTreeNodeHandle depended_h,
                            DependencyMask mask_pre) {
    manager.get(depended_h)
        .dependents.erase({dependent_h, mask_pre, mask_cons});
}
}  // namespace gbg
