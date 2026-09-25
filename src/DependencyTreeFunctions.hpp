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
    res.representative = dtn.getRID();

    if (initMask) {
        manager.propagateChange(dtn.getRID(), initMask);
    }

    return dtn.getRID();
}

template <typename AH, typename BH>
inline void setDependent(DependencyTreeManager& manager,
                         const DResource<AH>& dependent,
                         DependencyMask mask_cons,
                         const DResource<BH>& depended,
                         DependencyMask mask_pre) {
    auto it = manager.get(depended.representative)
                  .dependents.insert({{
                                          .dependent = dependent.representative,
                                          .mask_pre = mask_pre,
                                          .mask_cons = mask_cons,
                                      },
                                      false});
    if (manager.get(depended.representative).flags & mask_pre) {
        it.first->second = true;
        manager.get(dependent.representative).modifiedParents += 1;
        manager.propagateChange(dependent.representative, mask_cons);
    }
}

template <typename TH>
inline void deleteDependent(DependencyTreeManager& manager,
                            const DResource<TH>& dependent,
                            DependencyMask mask_cons,
                            const DResource<TH>& depended,
                            DependencyMask mask_pre) {
    manager.get(depended.representative)
        .dependents.erase({dependent.representative, mask_pre, mask_cons});
}
}  // namespace gbg
