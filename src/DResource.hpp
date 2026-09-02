#pragma once
#include "DependencyTree.hpp"
#include "Resource.hpp"
#include "macros.hpp"

namespace gbg {
template <typename TH>
class DResource : public Resource<TH> {
   public:
    DResource() : Resource<TH>(){};
    DResource(uint32_t rid) : Resource<TH>(rid){};
    DResource(std ::string name, uint32_t rid) : Resource<TH>(name, rid){};

   public:
    DependencyTreeNodeHandle representative;
};

}  // namespace gbg
