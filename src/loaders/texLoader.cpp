#include "texLoader.hpp"
#include <expected>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

std::expected<void, std::string> loadTexture(std::string_view path, gbg::Texture& texture,int mip_maps) {
    if(not std::filesystem::exists(path)) {
        return std::unexpected(std::format("Texture:: {} :: does not exist", path));
    }
    texture.mip_levels = mip_maps;
    texture.path = path;
    if (texture.data.size() > 0) {
        delete texture.data.data();
    }

    int channels;
    unsigned char* data =
        stbi_load(path.data(), &texture.width, &texture.height, &channels, 4);
    texture.data = std::span(
        data, texture.width * texture.height * 4 * sizeof(unsigned char));
    if(data == nullptr)
        return std::unexpected(std::format("Texture:: {} :: could not be loaded, may be the format is not supported", path));
    return {};
}
