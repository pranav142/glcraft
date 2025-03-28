//
// Created by pknadimp on 3/13/25.
//

#include "world.h"
#include "mesh_generator.h"
#include "unordered_set"
#include "iostream"
#include "timer.h"
#include <algorithm>
#include <oneapi/tbb/detail/_utils.h>

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

    if ((*m_new_chunks)[index].get_state() == Chunk::State::REGENERATE || (*m_new_chunks)[index].get_state() ==
        Chunk::State::UNINITIALIZED) {
        return std::nullopt;
    }

    return (*m_new_chunks)[index];
}

Block World::get_block(const glm::vec3 &position) {
    glm::vec3 chunk_position = world_position_to_chunk_position(position);
    int index = chunk_position_to_index(chunk_position);
    if (index == -1) {
        return create_block(BlockTypeID::EMPTY);
    }

    glm::vec3 block_position = position - chunk_position;
    return (*m_new_chunks)[index].get_block(block_position.x, block_position.y, block_position.z);
}

void World::set_block(const glm::vec3 &position, const Block &block) {
    glm::vec3 chunk_position = world_position_to_chunk_position(position);
    int index = chunk_position_to_index(chunk_position);
    if (index == -1) {
        return;
    }

    glm::vec3 block_position = position - chunk_position;

    (*m_new_chunks)[index].set_block(block_position.x, block_position.y, block_position.z, block);
    (*m_new_chunks)[index].set_state(Chunk::State::REMESH);

    if (static_cast<int>(block_position.x) == 0) {
        glm::vec3 neighbor_chunk = chunk_position - glm::vec3(CHUNK_WIDTH, 0, 0);
        index = chunk_position_to_index(neighbor_chunk);
        // std::cout << "Remeshing neighbor: " << neighbor_chunk.x << " " << neighbor_chunk.y << " " << neighbor_chunk.z << " " << std::endl;
        if (index != -1) {
            (*m_new_chunks)[index].set_state(Chunk::State::REMESH);
        }
    }

    if (static_cast<int>(block_position.x) == CHUNK_WIDTH - 1) {
        glm::vec3 neighbor_chunk = chunk_position + glm::vec3(CHUNK_WIDTH, 0, 0);
        // std::cout << "Remeshing neighbor: " << neighbor_chunk.x << " " << neighbor_chunk.y << " " << neighbor_chunk.z << " " << std::endl;
        index = chunk_position_to_index(neighbor_chunk);
        if (index != -1) {
            (*m_new_chunks)[index].set_state(Chunk::State::REMESH);
        }
    }

    if (static_cast<int>(block_position.y) == CHUNK_HEIGHT - 1) {
        glm::vec3 neighbor_chunk = chunk_position + glm::vec3(0, CHUNK_HEIGHT, 0);
        // std::cout << "Remeshing neighbor: " << neighbor_chunk.x << " " << neighbor_chunk.y << " " << neighbor_chunk.z << " " << std::endl;
        index = chunk_position_to_index(neighbor_chunk);
        if (index != -1) {
            (*m_new_chunks)[index].set_state(Chunk::State::REMESH);
        }
    }

    if (static_cast<int>(block_position.y) == 0) {
        glm::vec3 neighbor_chunk = chunk_position - glm::vec3(0, CHUNK_HEIGHT, 0);
        // std::cout << "Remeshing neighbor: " << neighbor_chunk.x << " " << neighbor_chunk.y << " " << neighbor_chunk.z << " " << std::endl;
        index = chunk_position_to_index(neighbor_chunk);
        assert(index != -1);
        if (index != -1) {
            (*m_new_chunks)[index].set_state(Chunk::State::REMESH);
        }
    }

    if (static_cast<int>(block_position.z) == 0) {
        glm::vec3 neighbor_chunk = chunk_position - glm::vec3(0, 0, CHUNK_LENGTH);
        // std::cout << "Remeshing neighbor: " << neighbor_chunk.x << " " << neighbor_chunk.y << " " << neighbor_chunk.z <<
        //         " " << std::endl;
        index = chunk_position_to_index(neighbor_chunk);
        if (index != -1) {
            (*m_new_chunks)[index].set_state(Chunk::State::REMESH);
        }
    }

    if (static_cast<int>(block_position.z) == CHUNK_LENGTH - 1) {
        glm::vec3 neighbor_chunk = chunk_position + glm::vec3(0, 0, CHUNK_LENGTH);
        //std::cout << "Remeshing neighbor: " << neighbor_chunk.x << " " << neighbor_chunk.y << " " << neighbor_chunk.z <<
        //        " " << std::endl;
        index = chunk_position_to_index(neighbor_chunk);
        if (index != -1) {
            (*m_new_chunks)[index].set_state(Chunk::State::REMESH);
        }
    }
}

void World::set_chunk_state(Chunk::State state, const glm::vec3 &position) {
    int index = chunk_position_to_index(position);
    if (index == -1) {
        return;
    }

    (*m_new_chunks)[index].set_state(state);
}

void World::update(const glm::vec3 &player_position) {
    glm::vec3 player_chunk_pos = world_position_to_chunk_position(player_position);

    if (player_chunk_pos != m_player_chunk_position) {
        m_player_chunk_position = player_chunk_pos;
        TIME_FUNCTION(update_chunks(), "UPDATE CHUNKS");
    }

    process_unloaded_blocks();
}

std::array<Chunk, TOTAL_CHUNKS> &World::get_chunks() {
    return *m_new_chunks;
}

void World::add_unloaded_block(const UnloadedBlock &unloaded_block) {
    m_unloaded_blocks.push_back(unloaded_block);
}

bool World::is_chunk_loaded(const glm::vec3 &chunk_position) const {
    for (auto &chunk: *m_new_chunks) {
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
    // m_world_generator.generate_chunk(chunk);
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

    for (int i = 0; i < TOTAL_CHUNKS; i++) {
        (*m_new_chunks)[i].set_state(Chunk::State::UNINITIALIZED);
    }

    std::vector<Chunk *> available_chunks;

    for (int i = 0; i < TOTAL_CHUNKS; i++) {
        Chunk &old_chunk = (*m_old_chunks)[i];

        if (old_chunk.get_state() == Chunk::State::UNINITIALIZED) {
            available_chunks.push_back(&old_chunk);
            continue;
        }

        if (!is_chunk_out_of_range(old_chunk.position(), m_player_chunk_position)) {
            int new_index = chunk_position_to_index(old_chunk.position());
            if (new_index != -1) {
                (*m_new_chunks)[new_index] = old_chunk;
            }
        } else {
            renderer::ChunkMesh *mesh = old_chunk.get_mesh();
            if (mesh) {
                delete_chunk_mesh(mesh);
                old_chunk.set_mesh(nullptr);
            }

            available_chunks.push_back(&old_chunk);
        }
    }

    int available_index = 0;
    for (int x = -SIMULATION_RADIUS; x <= SIMULATION_RADIUS; x++) {
        for (int y = -SIMULATION_RADIUS; y <= SIMULATION_RADIUS; y++) {
            for (int z = -SIMULATION_RADIUS; z <= SIMULATION_RADIUS; z++) {
                auto chunk_position = glm::vec3(
                    m_player_chunk_position.x + (x * CHUNK_WIDTH),
                    m_player_chunk_position.y + (y * CHUNK_HEIGHT),
                    m_player_chunk_position.z + (z * CHUNK_LENGTH)
                );

                int index = chunk_position_to_index(chunk_position);
                if (index == -1) continue;

                if ((*m_new_chunks)[index].get_state() == Chunk::State::UNINITIALIZED &&
                    available_index < available_chunks.size()) {
                    Chunk *available_chunk = available_chunks[available_index++];

                    (*m_new_chunks)[index] = *available_chunk;

                    (*m_new_chunks)[index].set_state(Chunk::State::REGENERATE);
                }

                (*m_new_chunks)[index].set_position(chunk_position);
            }
        }
    }
}

void World::process_unloaded_blocks() {
    auto it = std::ranges::remove_if(m_unloaded_blocks, [this](UnloadedBlock &unloaded_block) {
        glm::vec3 chunk_position = world_position_to_chunk_position(unloaded_block.global_position);
        int index = chunk_position_to_index(chunk_position);
        if (index == -1) {
            return false;
        }
        Chunk &chunk = (*m_new_chunks)[index];
        if (chunk.get_state() == Chunk::State::READY || chunk.get_state() == Chunk::State::REMESH) {
            glm::vec3 relative_position = unloaded_block.global_position - chunk_position;
            chunk.set_block(relative_position.x, relative_position.y, relative_position.z, unloaded_block.block);
            chunk.set_state(Chunk::State::REMESH);
            return true;
        }
        return false;
    }).begin();

    m_unloaded_blocks.erase(it, m_unloaded_blocks.end());
}
