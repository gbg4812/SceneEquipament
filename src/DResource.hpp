#pragma once
#include "Resource.hpp"
#include "DependencyTree.hpp"
#include "macros.hpp"

namespace gbg {
    class DResource : public Resource {
        public:
            RESOURCE_CONSTR(DResource);
        public:
            DependencyTreeNodeHandle representative;

    };

}
