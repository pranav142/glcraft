//
// Created by pknadimp on 3/11/25.
//

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

#include "texture_manager.h"

struct Block {
    renderer::TextureType type = renderer::TextureType::DIRT;
    // glm::vec3 position;
};


Block create_block(renderer::TextureType type);

#endif //BLOCK_H
