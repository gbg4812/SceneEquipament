#pragma once

#include <cassert>
#include <cmath>
#include <expected>

#include "../Texture.hpp"

/**
 * @return if successfuly loaded `true` else `false`
 */
std::expected<void, std::string> loadTexture(std::string_view path, gbg::Texture& texture, int mip_maps = 1);
