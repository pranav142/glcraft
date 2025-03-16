//
// Created by pknadimp on 3/13/25.
//

#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include "mesh_generator.h"
#include "world_gen.h"

static constexpr int SIMULATION_RADIUS = 6;

class World {
public:
    World() = default;

    void initialize(const glm::vec3& player_position);

    void update(const glm::vec3 &player_position);

    [[nodiscard]] std::vector<Chunk> &get_chunks();

private:
    [[nodiscard]] bool is_chunk_loaded(const glm::vec3 &chunk_position) const;

    [[nodiscard]] bool is_chunk_out_of_range(const glm::vec3 &chunk_position,
                                             const glm::vec3 &player_chunk_position) const;

    void load_chunk(const glm::vec3 &chunk_position);

    void update_chunks();
private:
    std::vector<Chunk> m_chunks;
    WorldGenerator m_world_generator = WorldGenerator(1000);

    bool is_first = true;
    glm::ivec3 m_player_chunk_position = glm::ivec3(0, 0, 0);
};


#endif //WORLD_H
