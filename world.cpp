//
// Created by pknadimp on 3/13/25.
//

#include "world.h"
#include "mesh_generator.h"
#include "unordered_set"
#include "iostream"
#include "timer.h"
#include <algorithm>

void World::initialize(const glm::vec3 &player_position) {
    glm::ivec3 player_chunk_pos = world_position_to_chunk_position(player_position);
    m_player_chunk_position = player_chunk_pos;
    update_chunks();
}

std::optional<std::reference_wrapper<const Chunk>> World::get_chunk(const glm::vec3 &position) const {
    if (auto it = m_chunk_map.find(position); it != m_chunk_map.end()) {
        return m_chunks[it->second];
    }
    return std::nullopt;
}

void World::update(const glm::vec3 &player_position) {
    glm::ivec3 player_chunk_pos = world_position_to_chunk_position(player_position);

    if (player_chunk_pos != m_player_chunk_position) {
        m_player_chunk_position = player_chunk_pos;
        update_chunks();
    }
}

std::vector<Chunk> &World::get_chunks() {
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
    int chunk_grid_x = round((chunk_position.x - player_chunk_position.x) / CHUNK_WIDTH);
    int chunk_grid_z = round((chunk_position.z - player_chunk_position.z) / CHUNK_LENGTH);
    int chunk_grid_y = round((chunk_position.y - player_chunk_position.y) / CHUNK_HEIGHT);

    return (abs(chunk_grid_x) > SIMULATION_RADIUS || abs(chunk_grid_z) > SIMULATION_RADIUS || abs(chunk_grid_y) >
            SIMULATION_RADIUS);
}

void World::load_chunk(const glm::vec3 &chunk_position) {
    Chunk new_chunk(chunk_position.x, chunk_position.y, chunk_position.z);
    m_world_generator.generate_chunk(new_chunk);

    if (new_chunk.get_block_count() > 0) {
        m_chunks.push_back(new_chunk);
        m_chunk_map[chunk_position] = m_chunks.size() - 1;
    }
}

void World::update_chunks() {
    auto remove_it = std::remove_if(
        m_chunks.begin(), m_chunks.end(),
        [this](const Chunk &chunk) {
            bool out_of_range = is_chunk_out_of_range(chunk.position(), m_player_chunk_position);

            if (out_of_range) {
                if (renderer::ChunkMesh *mesh = chunk.get_mesh()) {
                    delete_chunk_mesh(mesh);
                }
                m_chunk_map.erase(chunk.position());
            }


            return out_of_range;
        });

    m_chunks.erase(remove_it, m_chunks.end());

    for (int x = -SIMULATION_RADIUS; x <= SIMULATION_RADIUS; x++) {
        for (int z = -SIMULATION_RADIUS; z <= SIMULATION_RADIUS; z++) {
            for (int y = -SIMULATION_RADIUS; y <= SIMULATION_RADIUS; y++) {
                auto chunk_position = glm::vec3(m_player_chunk_position.x + (x * CHUNK_WIDTH),
                                                m_player_chunk_position.y + (y * CHUNK_HEIGHT),
                                                m_player_chunk_position.z + (z * CHUNK_LENGTH));

                if (!is_chunk_loaded(chunk_position)) {
                    load_chunk(chunk_position);
                }
            }
        }
    }
}
