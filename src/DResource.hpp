#pragma once
#include "Resource.hpp"
#include "DependencyTree.hpp"
#include "macros.hpp"

namespace gbg {
    template <typename TH>
    class DResource : public Resource<TH> {
        public:
         DResource() : Resource<ResourceHandle>(){};
         DResource(std ::string name, uint32_t rid)
             : Resource<ResourceHandle>(name, rid){};
         ;

        public:
            DependencyTreeNodeHandle representative;

    };

}
