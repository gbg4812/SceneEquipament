#pragma once
#include <span>

#include "DResource.hpp"
#include "macros.hpp"

namespace gbg {

RESOURCE_HANDLE(Texture);

class Texture : public DResource {
   public:
    DRESOURCE_CONSTR(Texture)

   public:
    std::string path;
    std::span<unsigned char> data;
    int width = 0;
    int height = 0;
    int mip_levels = 0;
    bool raw = false;
};

RESOURCE_MANAGER(Texture);

}  // namespace gbg
