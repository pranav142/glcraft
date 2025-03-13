//
// Created by pknadimp on 3/11/25.
//

#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "block.h"
#include "chunk.h"
#include "texture_manager.h"

namespace renderer {
    struct ChunkMesh {
        unsigned int VBO, VAO, EBO;
        glm::vec3 position;
        int num_indices;
    };

    static Block empty_block = Block(TextureType::DIRT);

    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        FRONT,
        BACK,
    };

    ChunkMesh create_chunk_mesh(const Chunk& chunk);

    static void add_face(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer, Direction direction,
                        Block block, glm::vec3 block_position);

    static std::vector<float> get_face_vertices(Direction direction, const AtlasTextureCoordinates &coords);

    static void fill_chunk_vertex_and_index_buffer(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer, const Chunk& chunk);
};


#endif //MESH_GENERATOR_H
