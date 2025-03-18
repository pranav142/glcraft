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

float Chunk::get_distance() const {
    return m_distance;
}

void Chunk::set_distance(float distance) {
    m_distance = distance;
}

bool Chunk::needs_remesh() const {
    return m_needs_remesh;
}

void Chunk::set_needs_remesh(bool remesh) {
    m_needs_remesh = remesh;
}

void Chunk::set_block(int x, int y, int z, const Block &block) {
    int index = calculate_index(x, y, z);
    if (index > m_blocks.size() || !coordinate_in_bounds(x, y, z)) {
        return;
    }

    if (m_blocks[index].type == BlockTypeID::AIR && block.type != BlockTypeID::AIR) {
        m_num_blocks++;
    }

    if (m_blocks[index].type != BlockTypeID::AIR && block.type == BlockTypeID::AIR) {
        m_num_blocks--;
    }

    m_blocks[index] = block;
}

glm::vec3 Chunk::position() const {
    return m_position;
}

void Chunk::set_position(const glm::vec3 &position) {
    m_position = position;
}

renderer::ChunkMesh *Chunk::get_mesh() const {
    return m_mesh;
}

void Chunk::set_mesh(renderer::ChunkMesh *mesh) {
    m_mesh = mesh;
}

int Chunk::calculate_index(int x, int y, int z) const {
    return x * CHUNK_WIDTH * CHUNK_HEIGHT + y * CHUNK_WIDTH + z;
}

bool compare_chunk(const Chunk &obj1, const Chunk &obj2) {
    return obj1.get_distance() < obj2.get_distance();
}

bool Chunk::coordinate_in_bounds(int x, int y, int z) const {
    return (x < CHUNK_WIDTH && y < CHUNK_HEIGHT && z < CHUNK_LENGTH && x >= 0 && y >= 0 && z >= 0);
}

bool Chunk::is_uninitialized() const {
    return m_is_uninitialized;
}

void Chunk::set_unintialized(bool flag) {
    m_is_uninitialized = flag;
}