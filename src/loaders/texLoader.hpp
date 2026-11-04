#pragma once

#include <cassert>

#include "../Texture.hpp"

/**
 * @return if successfuly loaded `true` else `false`
 */
bool loadTexture(std::string_view path, gbg::Texture& texture);
