#pragma once
#include <span>

#include "DResource.hpp"
#include "macros.hpp"

namespace gbg {
class Texture : public DResource {
   public:
    Texture() : DResource() {}
    Texture(std::string name, uint32_t rid) : DResource(name, rid) {}

   public:
    std::string path;
    std::span<unsigned char> data;
    int width = 0;
    int height = 0;
    int mip_levels = 0;
    bool raw = false;
};


RESOURCE_HANDLE(Texture);
RESOURCE_MANAGER(Texture);

}  // namespace gbg
