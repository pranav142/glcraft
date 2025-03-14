//
// Created by pknadimp on 3/13/25.
//

#include "world.h"

#include "mesh_generator.h"
#include "mesh_generator.h"
#include "unordered_set"

void World::initialize() {
    // create_example_chunks();
}

// THIS IS FUCKING NASTY FIND A BETTER WAY
namespace std {
    template<>
    struct hash<glm::vec3> {
        size_t operator()(const glm::vec3& v) const {
            // Combine the hash of each component
            size_t hash = 0;
            hash ^= std::hash<float>()(v.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<float>()(v.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= std::hash<float>()(v.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };
}

const std::vector<Chunk> &World::get_chunks(const glm::vec3 &position) {
    glm::ivec3 player_chunk_pos = glm::ivec3(
        floor(position.x / 16.0f) * 16,
        floor(position.y / 16.0f) * 16,
        floor(position.z / 16.0f) * 16
    );

    // Create a set of positions that should be loaded
    std::unordered_set<glm::vec3> desired_chunk_positions;

    // Add all positions within range to our desired set
    for (int x = -3; x <= 3; x++) {
        for (int z = -3; z <= 3; z++) {
            glm::vec3 chunk_pos(
                player_chunk_pos.x + (x * 16),
                0, // Always place floor at y=0
                player_chunk_pos.z + (z * 16)
            );

            desired_chunk_positions.insert(chunk_pos);
        }
    }

    // First, unload chunks that are out of range
    for (auto it = m_chunks.begin(); it != m_chunks.end();) {
        bool keep_chunk = desired_chunk_positions.find(it->position()) != desired_chunk_positions.end();

        if (!keep_chunk) {
            // Clean up mesh resources before removing chunk
            renderer::ChunkMesh *mesh = it->get_mesh();
            if (mesh) {
                renderer::delete_chunk_mesh(mesh);
            }

            it = m_chunks.erase(it);
        } else {
            // Only increment if we didn't erase
            ++it;
        }
    }

    // Now load new chunks that don't exist yet
    for (const auto &chunk_pos: desired_chunk_positions) {
        // Check if chunk already exists
        bool chunk_exists = false;
        for (auto &chunk: m_chunks) {
            if (chunk.position() == chunk_pos) {
                chunk_exists = true;
                break;
            }
        }

        // Create new chunk if it doesn't exist
        if (!chunk_exists) {
            Chunk new_chunk(chunk_pos.x, chunk_pos.y, chunk_pos.z);
            create_stone_chunk(CHUNK_WIDTH, CHUNK_WIDTH, 16, new_chunk);

            if (new_chunk.get_block_count() > 0) {
                renderer::ChunkMesh *mesh = renderer::create_chunk_mesh(new_chunk);
                new_chunk.set_mesh(mesh);
                m_chunks.push_back(new_chunk);
            }
        }
    }

    return m_chunks;
}

void World::create_example_chunks() {
    //  for (int i = -8; i <= 8; i++) {
    //      for (int j = -8; j <= 8; j++) {
    //          Chunk chunk(16 * i, 0, 16 * j);
    //          create_stone_chunk(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_LENGTH, chunk);
    //          if (!chunk.get_block_count() == 0) {
    //              m_chunks.push_back(renderer::create_chunk_mesh(chunk));
    //          }
    //      }
    //  }
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
