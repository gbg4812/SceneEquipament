#pragma once
#include <cassert>
#define STB_INCLUDE_IMPLEMENTATION
#include "../Scene.hpp"
#include "../Texture.hpp"
#include "external/stb_image.h"

inline void loadTexture(std::string_view path, gbg::Scene* scene,
                        gbg::TextureHandle textureH) {
    auto& tex_mn = scene->getTextureManager();
    gbg::Texture& texture = tex_mn.get(textureH);
    if (texture.data.size() > 0) {
        delete texture.data.data();
    }

    assert(texture.getRID() != 0);

    int channels;
    unsigned char* data =
        stbi_load(path.data(), &texture.width, &texture.height, &channels, 4);
    texture.data = std::span(data, texture.width * texture.height);
}
