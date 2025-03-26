//
// Created by pknadimp on 3/13/25.
//

#ifndef WORLD_H
#define WORLD_H

#include <optional>

#include "chunk.h"

struct UnloadedBlock {
    Block block;
    glm::vec3 original_chunk_position;
    glm::vec3 global_position;
};

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

static constexpr int SIMULATION_RADIUS = 8;

constexpr int TOTAL_CHUNKS = (2 * SIMULATION_RADIUS + 1) * (2 * SIMULATION_RADIUS + 1) * (
                                 2 * SIMULATION_RADIUS + 1);


class World {
public:
    World() {
    };

    void initialize(const glm::vec3 &player_position);

    std::optional<std::reference_wrapper<const Chunk> > get_chunk(const glm::vec3 &position) const;

    Block get_block(const glm::vec3 &position);

    void set_block(const glm::vec3 &position, const Block &block);

    void set_chunk_state(Chunk::State state, const glm::vec3 &position);

    void update(const glm::vec3 &player_position);

    [[nodiscard]] std::array<Chunk, TOTAL_CHUNKS> &get_chunks();

    void add_unloaded_block(const UnloadedBlock &unloaded_block);

private:
    [[nodiscard]] bool is_chunk_loaded(const glm::vec3 &chunk_position) const;

    [[nodiscard]] bool is_chunk_out_of_range(const glm::vec3 &chunk_position,
                                             const glm::vec3 &player_chunk_position) const;

    void load_chunk(::Chunk &chunk, const glm::vec3 &chunk_position);

    int chunk_position_to_index(const glm::vec3 &chunk_position) const;

    void update_chunks();

    void process_unloaded_blocks();

private:
    std::array<Chunk, TOTAL_CHUNKS> *m_new_chunks = new std::array<Chunk, TOTAL_CHUNKS>;


    std::array<Chunk, TOTAL_CHUNKS> *m_old_chunks = new std::array<Chunk, TOTAL_CHUNKS>;

    glm::vec3 m_player_chunk_position = glm::vec3(0, 0, 0);

    std::vector<UnloadedBlock> m_unloaded_blocks;
};


#endif //WORLD_H
