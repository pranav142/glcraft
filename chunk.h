//
// Created by pknadimp on 3/11/25.
//

#ifndef CHUNK_H
#define CHUNK_H

#include <array>

#include "block.h"

namespace renderer {
    class ChunkMesh;
}

constexpr int CHUNK_HEIGHT = 32;
constexpr int CHUNK_WIDTH = 32;
constexpr int CHUNK_LENGTH = 32;

class Chunk {
public:
    // these are global coordinates
    Chunk() = default;

    Chunk(float x, float y, float z) : m_position(glm::vec3(x, y, z)) {
    }
    // x, y, z relative to chunk
    [[nodiscard]] const Block &get_block(int x, int y, int z) const;

    [[nodiscard]] const int &get_block_count() const;

    float get_distance() const;

    void set_distance(float distance);

    bool needs_remesh() const;

    void set_needs_remesh(bool remesh);

    void set_block(int x, int y, int z, const Block &block);

    [[nodiscard]] glm::vec3 position() const;

    void set_position(const glm::vec3& position);

    [[nodiscard]] renderer::ChunkMesh* get_mesh() const;

    void set_mesh(renderer::ChunkMesh* mesh);

    [[nodiscard]] bool coordinate_in_bounds(int x, int y, int z) const;

    [[nodiscard]] bool is_uninitialized() const;

    void set_unintialized(bool flag);
private:
    [[nodiscard]] int calculate_index(int x, int y, int z) const;

private:
    glm::vec3 m_position = glm::vec3(0, 0, 0);
    bool m_is_uninitialized = true;
    bool m_needs_remesh = true;

    float m_distance = 0.0f;

    int m_num_blocks = 0;
    std::array<Block, CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_LENGTH> m_blocks;
    renderer::ChunkMesh* m_mesh = nullptr;
};

inline glm::vec3 world_position_to_chunk_position(const glm::vec3 &world_position) {
    return {
        floor(world_position.x / CHUNK_WIDTH) * CHUNK_WIDTH,
        floor(world_position.y / CHUNK_HEIGHT) * CHUNK_HEIGHT,
        floor(world_position.z / CHUNK_LENGTH) * CHUNK_LENGTH
    };
}

bool compare_chunk(const Chunk& obj1, const Chunk& obj2);


#endif //CHUNK_H
