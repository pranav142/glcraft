//
// Created by pknadimp on 3/11/25.
//

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

#include "texture_manager.h"

struct Block {
    // Just have a default type right now
    renderer::TextureType type;
    glm::vec3 position;
};

Block create_block(const glm::vec3 &position);

#endif //BLOCK_H
