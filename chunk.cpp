//
// Created by pknadimp on 3/11/25.
//

#include "chunk.h"

#include "mesh_generator.h"


const Block& Chunk::get_block(int x, int y, int z) const {
    int index = calculate_index(x, y, z);
    if (index > blocks.size() || !coordinate_in_bounds(x, y, z)) {
        return renderer::empty_block;
    }
    return blocks[calculate_index(x, y, z)];
}

void Chunk::set_block(int x, int y, int z, const Block &block) {
    int index = calculate_index(x, y, z);
    if (index > blocks.size() || !coordinate_in_bounds(x, y, z)) {
        return;
    }
    blocks[index] = block;
}

glm::vec3 Chunk::position() const {
    return m_position;
}

int Chunk::calculate_index(int x, int y, int z) const {
    return x * 256 + y * 16 + z;
}

bool Chunk::coordinate_in_bounds(int x, int y, int z) const {
    return (x < CHUNK_WIDTH && y < CHUNK_HEIGHT && z < CHUNK_LENGTH && x >= 0 && y >= 0 && z >= 0);
}

