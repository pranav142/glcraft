//
// Created by pknadimp on 3/11/25.
//

#include "block.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Block create_block(const glm::vec3 &position) {
    return Block(renderer::TextureType::STONE, position);
}