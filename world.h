//
// Created by pknadimp on 3/13/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <optional>

#include "chunk.h"
#include "world_gen.h"

// Hash function for glm::vec3
namespace std {
    template<>
    struct hash<glm::vec3> {
        size_t operator()(const glm::vec3 &v) const {
            return ((static_cast<int>(v.x) * 73856093) ^
                    (static_cast<int>(v.y) * 19349663) ^
                    (static_cast<int>(v.z) * 83492791));
        }
    };
}

static constexpr int SIMULATION_RADIUS = 6;

class World {
public:
    World() {
        m_new_chunks.reserve((2 * SIMULATION_RADIUS + 1) * (2 * SIMULATION_RADIUS + 1) * (2 * SIMULATION_RADIUS + 1));
        m_old_chunks.reserve((2 * SIMULATION_RADIUS + 1) * (2 * SIMULATION_RADIUS + 1) * (2 * SIMULATION_RADIUS + 1));
    };

    void initialize(const glm::vec3 &player_position);

    std::optional<std::reference_wrapper<const Chunk>> get_chunk(const glm::vec3 &position) const;

    void update(const glm::vec3 &player_position);

    [[nodiscard]] std::vector<Chunk> &get_chunks();

private:
    [[nodiscard]] bool is_chunk_loaded(const glm::vec3 &chunk_position) const;

    [[nodiscard]] bool is_chunk_out_of_range(const glm::vec3 &chunk_position,
                                             const glm::vec3 &player_chunk_position) const;

    void load_chunk(::Chunk &chunk, const glm::vec3 & chunk_position);

    int chunk_position_to_index(const glm::vec3& chunk_position) const;

    void update_chunks();

private:
    std::vector<Chunk> m_new_chunks;
    std::vector<Chunk> m_old_chunks;

    WorldGenerator m_world_generator = WorldGenerator(1000);

    glm::vec3 m_player_chunk_position = glm::vec3(0, 0, 0);
};


#endif //WORLD_H
