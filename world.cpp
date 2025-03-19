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

std::optional<std::reference_wrapper<const Chunk> > World::get_chunk(const glm::vec3 &position) const {
    auto index = chunk_position_to_index(position);
    if (index == -1) {
        return std::nullopt;
    }

    return m_new_chunks[index];
}

void World::update(const glm::vec3 &player_position) {
    glm::vec3 player_chunk_pos = world_position_to_chunk_position(player_position);

    if (player_chunk_pos != m_player_chunk_position) {
        m_player_chunk_position = player_chunk_pos;
        update_chunks();
    }
}

std::vector<Chunk> &World::get_chunks() {
    return m_new_chunks;
}

bool World::is_chunk_loaded(const glm::vec3 &chunk_position) const {
    for (auto &chunk: m_new_chunks) {
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

void World::load_chunk(Chunk &chunk, const glm::vec3 &chunk_position) {
    chunk.set_position(chunk_position);
    m_world_generator.generate_chunk(chunk);
    chunk.set_state(Chunk::State::REMESH);
}

int World::chunk_position_to_index(const glm::vec3 &chunk_position) const {
    int chunk_grid_x = static_cast<int>(round((chunk_position.x - m_player_chunk_position.x) / CHUNK_WIDTH));
    int chunk_grid_y = static_cast<int>(round((chunk_position.y - m_player_chunk_position.y) / CHUNK_HEIGHT));
    int chunk_grid_z = static_cast<int>(round((chunk_position.z - m_player_chunk_position.z) / CHUNK_LENGTH));

    if (abs(chunk_grid_x) > SIMULATION_RADIUS ||
        abs(chunk_grid_y) > SIMULATION_RADIUS ||
        abs(chunk_grid_z) > SIMULATION_RADIUS) {
        return -1;
    }

    int x_index = chunk_grid_x + SIMULATION_RADIUS;
    int y_index = chunk_grid_y + SIMULATION_RADIUS;
    int z_index = chunk_grid_z + SIMULATION_RADIUS;

    const int width = 2 * SIMULATION_RADIUS + 1;
    const int height = 2 * SIMULATION_RADIUS + 1;

    return x_index + (y_index * width) + (z_index * width * height);
}

// TODO: Are we using clear and
void World::update_chunks() {
    std::swap(m_new_chunks, m_old_chunks);
    m_new_chunks.clear();

    constexpr int total_chunks = (2 * SIMULATION_RADIUS + 1) * (2 * SIMULATION_RADIUS + 1) * (
                                     2 * SIMULATION_RADIUS + 1);
    m_new_chunks.resize(total_chunks);

    for (auto &chunk: m_new_chunks) {
        chunk.set_state(Chunk::State::REGENERATE);
    }

    for (auto &chunk: m_old_chunks) {
        if (is_chunk_out_of_range(chunk.position(), m_player_chunk_position)) {
            renderer::ChunkMesh *mesh = chunk.get_mesh();
            if (mesh) {
                delete_chunk_mesh(mesh);
            }
        }
        int index = chunk_position_to_index(chunk.position());
        if (index != -1) {
            m_new_chunks[index] = std::move(chunk);
        }
    }

    // Load any new chunks that are in range but not yet loaded
    for (int x = -SIMULATION_RADIUS; x <= SIMULATION_RADIUS; x++) {
        for (int y = -SIMULATION_RADIUS; y <= SIMULATION_RADIUS; y++) {
            for (int z = -SIMULATION_RADIUS; z <= SIMULATION_RADIUS; z++) {
                auto chunk_position = glm::vec3(
                    m_player_chunk_position.x + (x * CHUNK_WIDTH),
                    m_player_chunk_position.y + (y * CHUNK_HEIGHT),
                    m_player_chunk_position.z + (z * CHUNK_LENGTH)
                );


                int index = chunk_position_to_index(chunk_position);
                Chunk &chunk = m_new_chunks[index];
                if (index == -1) continue;

                chunk.set_distance(glm::distance(m_player_chunk_position, chunk_position));

                if (chunk.get_state() == Chunk::State::REGENERATE) {
                    load_chunk(chunk, chunk_position);

                    // Remesh Neighboring Chunks
                    glm::vec3 neighbor_positions[6] = {
                        chunk_position + glm::vec3(CHUNK_WIDTH, 0, 0),
                        chunk_position - glm::vec3(CHUNK_WIDTH, 0, 0),
                        chunk_position + glm::vec3(0, CHUNK_HEIGHT, 0),
                        chunk_position - glm::vec3(0, CHUNK_HEIGHT, 0),
                        chunk_position + glm::vec3(0, 0, CHUNK_LENGTH),
                        chunk_position - glm::vec3(0, 0, CHUNK_LENGTH)
                    };

                    for (const auto &neighbor_pos: neighbor_positions) {
                        int neighbor_index = chunk_position_to_index(neighbor_pos);
                        if (neighbor_index != -1) {
                            if (m_new_chunks[neighbor_index].get_state() == Chunk::State::READY) {
                                m_new_chunks[neighbor_index].set_state(Chunk::State::REMESH);
                            }
                        }
                    }
                }
            }
        }
    }

    m_old_chunks.clear();
}
