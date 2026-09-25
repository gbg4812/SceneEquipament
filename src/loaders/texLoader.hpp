#pragma once
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "../Texture.hpp"
#include "external/stb_image.h"

inline bool loadTexture(std::string_view path, gbg::Texture& texture) {
    texture.mip_levels = 1;
    texture.path = path;
    if (texture.data.size() > 0) {
        delete texture.data.data();
    }

    int channels;
    unsigned char* data =
        stbi_load(path.data(), &texture.width, &texture.height, &channels, 4);
    texture.data = std::span(
        data, texture.width * texture.height * 4 * sizeof(unsigned char));
    return data == nullptr;
}
