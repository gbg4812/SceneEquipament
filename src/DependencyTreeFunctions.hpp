#pragma once
#include "DResource.hpp"
#include "DependencyTree.hpp"

namespace gbg {
template <typename T, typename H>
    requires std::derived_from<T, DResource>
inline DependencyTreeNodeHandle createRepresentative(
    DependencyTreeManager& manager, H res_h, ResourceManager<T, H>& res_manager,
    ResourceType type, DependencyMask initMask = 0) {
    DependencyTreeNode& dtn = manager.create(res_h, type);
    res_manager.get(res_h).representative = dtn.getRID();
    if (initMask) {
        manager.propagateChange(dtn.getRID(), initMask);
    }
    return dtn.getRID();
}

inline void setDependent(DependencyTreeManager& manager,
                         const DResource& dependent, DependencyMask mask_cons,
                         const DResource& depended, DependencyMask mask_pre) {
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
inline void deleteDependent(DependencyTreeManager& manager,
                            const DResource& dependent,
                            DependencyMask mask_cons, const DResource& depended,
                            DependencyMask mask_pre) {
    manager.get(depended.representative)
        .dependents.erase({dependent.representative, mask_pre, mask_cons});
}
}  // namespace gbg
