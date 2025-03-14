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

    [[nodiscard]] const std::vector<Chunk> &get_chunks(const glm::vec3& position);

private:
    void create_example_chunks();
    void create_stone_chunk(int length, int width, int height, Chunk &chunk) const;

private:
    std::vector<Chunk> m_chunks;
};


#endif //WORLD_H
