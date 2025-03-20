//
// Created by pknadimp on 3/11/25.
//

#include "chunk.h"
#include "mesh_generator.h"

void Chunk::initialize() {
    m_blocks = new std::array<Block, CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_LENGTH>();
}

void Chunk::clear_blocks() {
    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_LENGTH; z++) {
                Block air_block = create_block(BlockTypeID::AIR);
                set_block(x, y, z, air_block);
            }
        }
    }
}

const Block &Chunk::get_block(int x, int y, int z) const {
    int index = calculate_index(x, y, z);
    if (index > m_blocks->size() || !coordinate_in_bounds(x, y, z)) {
        return renderer::empty_block;
    }
    return (*m_blocks)[index];
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

void Chunk::set_block(int x, int y, int z, const Block &block) {
    int index = calculate_index(x, y, z);
    if (index >= m_blocks->size() || !coordinate_in_bounds(x, y, z)) {
        return;
    }

    if ((*m_blocks)[index].type == BlockTypeID::AIR && block.type != BlockTypeID::AIR) {
        m_num_blocks++;
    }

    if ((*m_blocks)[index].type != BlockTypeID::AIR && block.type == BlockTypeID::AIR) {
        m_num_blocks--;
    }

    (*m_blocks)[index] = block;
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

Chunk::State Chunk::get_state() const {
    return m_state;
}

void Chunk::set_state(Chunk::State state) {
    m_state = state;
}

void Chunk::destroy() {
    if (m_blocks) {
        delete m_blocks;
    }
}
