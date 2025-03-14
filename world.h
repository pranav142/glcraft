//
// Created by pknadimp on 3/13/25.
//

#ifndef WORLD_H
#define WORLD_H

#include "chunk.h"
#include "mesh_generator.h"

class World {
public:
    World() = default;

    void initialize();

    void update(const glm::vec3 &player_position);

    [[nodiscard]] const std::vector<Chunk> &get_chunks() const;

private:
    [[nodiscard]] bool is_chunk_loaded(const glm::vec3 &chunk_position) const;

    [[nodiscard]] bool is_chunk_out_of_range(const glm::vec3 &chunk_position,
                                             const glm::vec3 &player_chunk_position) const;

    void load_chunk(const glm::vec3 &chunk_position);

private:
    void create_stone_chunk(int length, int width, int height, Chunk &chunk) const;

private:
    std::vector<Chunk> m_chunks;

    glm::vec3 m_position = glm::vec3(0, 0, 0);
};


#endif //WORLD_H
