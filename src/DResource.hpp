#pragma once
#include "DependencyTree.hpp"
#include "Resource.hpp"
#include "macros.hpp"

namespace gbg {
class DResource : public Resource {
   public:
    RESOURCE_CONSTR(DResource)

   public:
    DependencyTreeNodeHandle representative;
};

}  // namespace gbg
