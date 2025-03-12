//
// Created by pknadimp on 3/11/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <array>

#include "block.h"

constexpr int CHUNK_HEIGHT = 16;
constexpr int CHUNK_WIDTH = 16;
constexpr int CHUNK_LENGTH = 16;

class Chunk {
public:
    // these are global coordinates
    Chunk(float x, float y, float z) : m_position(glm::vec3(x, y, z)) {
    }

    // x, y, z relative to chunk
    [[nodiscard]] const Block &get_block(int x, int y, int z) const;

    void set_block(int x, int y, int z, const Block &block);

    [[nodiscard]] glm::vec3 position() const;

private:
    [[nodiscard]] int calculate_index(int x, int y, int z) const;

    [[nodiscard]] bool coordinate_in_bounds(int x, int y, int z) const;

private:
    glm::vec3 m_position = glm::vec3(0, 0, 0);
    std::array<Block, CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_LENGTH> blocks;
};

#endif //CHUNK_H
