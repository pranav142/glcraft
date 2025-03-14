//
// Created by pknadimp on 3/11/25.
//

#include "chunk.h"
#include "mesh_generator.h"

const Block &Chunk::get_block(int x, int y, int z) const {
    int index = calculate_index(x, y, z);
    if (index > m_blocks.size() || !coordinate_in_bounds(x, y, z)) {
        return renderer::empty_block;
    }
    return m_blocks[index];
}

const int &Chunk::get_block_count() const {
    return m_num_blocks;
}

void Chunk::set_block(int x, int y, int z, const Block &block) {
    int index = calculate_index(x, y, z);
    if (index > m_blocks.size() || !coordinate_in_bounds(x, y, z)) {
        return;
    }

    if (m_blocks[index].type == BlockTypeID::EMPTY && block.type != BlockTypeID::EMPTY) {
        m_num_blocks++;
    }

    if (m_blocks[index].type != BlockTypeID::EMPTY && block.type == BlockTypeID::EMPTY) {
        m_num_blocks--;
    }

    m_blocks[index] = block;
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

renderer::ChunkMesh *Chunk::get_mesh() const {
    return m_mesh;
}

void Chunk::set_mesh(renderer::ChunkMesh *mesh) {
    m_mesh = mesh;
}
