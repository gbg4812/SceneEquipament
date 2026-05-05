#pragma once
#include <span>

#include "Resource.hpp"

namespace gbg {
class Texture : public Resource {
   public:
    Texture() : Resource() {}
    Texture(std::string name, uint32_t rid) : Resource(name, rid) {}

   public:
    std::string path;
    std::span<unsigned char> data;
    int width = 0;
    int height = 0;
    int mip_levels = 0;
};

class TextureHandle : public ResourceHandle {
    public:
    TextureHandle() : ResourceHandle(){};
    TextureHandle(uint32_t rid, size_t index) : ResourceHandle(rid, index){};
};

}  // namespace gbg
