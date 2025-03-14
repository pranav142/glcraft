//
// Created by pknadimp on 3/13/25.
//

#include "world.h"
#include "mesh_generator.h"
#include "unordered_set"

#include <algorithm>

void World::initialize() {
    // create_example_chunks();
}

// TODO: ADJUST MAGIC NUMBERS FOR RENDER DISTANCE
void World::update(const glm::vec3 &player_position) {
    glm::ivec3 player_chunk_pos = world_position_to_chunk_position(player_position);

    auto remove_it = std::remove_if(
        m_chunks.begin(), m_chunks.end(),
        [this, &player_chunk_pos](const Chunk &chunk) {
            bool out_of_range = is_chunk_out_of_range(chunk.position(), player_chunk_pos);

            if (out_of_range) {
                if (renderer::ChunkMesh *mesh = chunk.get_mesh()) {
                    delete_chunk_mesh(mesh);
                }
            }

            return out_of_range;
        });

    m_chunks.erase(remove_it, m_chunks.end());

    for (int x = -3; x <= 3; x++) {
        for (int z = -3; z <= 3; z++) {
            auto chunk_position = glm::vec3(player_chunk_pos.x + (x * CHUNK_WIDTH),
                                            0,
                                            player_chunk_pos.z + (z * CHUNK_LENGTH));

            if (!is_chunk_loaded(chunk_position)) {
                load_chunk(chunk_position);
            }
        }
    }
}

const std::vector<Chunk> &World::get_chunks() const {
    return m_chunks;
}

bool World::is_chunk_loaded(const glm::vec3 &chunk_position) const {
    for (auto &chunk: m_chunks) {
        if (chunk.position() == chunk_position) {
            return true;
        }
    }
    return false;
}

bool World::is_chunk_out_of_range(const glm::vec3 &chunk_position, const glm::vec3 &player_chunk_position) const {
    int chunk_grid_x = round((chunk_position.x - player_chunk_position.x) / 16.0f);
    int chunk_grid_z = round((chunk_position.z - player_chunk_position.z) / 16.0f);

    return (abs(chunk_grid_x) > 3 || abs(chunk_grid_z) > 3);
}

void World::load_chunk(const glm::vec3 &chunk_position) {
    Chunk new_chunk(chunk_position.x, chunk_position.y, chunk_position.z);
    create_stone_chunk(CHUNK_LENGTH, CHUNK_WIDTH, CHUNK_HEIGHT, new_chunk);

    if (new_chunk.get_block_count() > 0) {
        renderer::ChunkMesh *mesh = renderer::create_chunk_mesh(new_chunk);
        new_chunk.set_mesh(mesh);
    }

    m_chunks.push_back(new_chunk);
}

void World::create_stone_chunk(int length, int width, int height, Chunk &chunk) const {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < height; k++) {
                if (k == height - 1) {
                    auto block = create_block(BlockTypeID::GRASS);
                    chunk.set_block(i, k, j, block);
                } else {
                    auto block = create_block(BlockTypeID::DIRT);
                    chunk.set_block(i, k, j, block);
                }
            }
        }
    }
}
