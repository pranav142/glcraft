//
// Created by pknadimp on 3/11/25.
//

#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "block.h"
#include "chunk.h"
#include "texture_manager.h"
#include <glad/glad.h>

namespace renderer {
    struct ChunkMesh {
        unsigned int VBO = 0, VAO = 0, EBO = 0;
        glm::vec3 position = glm::vec3(0, 0, 0);
        int num_indices = 0;
    };

    constexpr int RENDER_RADIUS = 6;

    static Block empty_block = Block(BlockTypeID::EMPTY);

    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        FRONT,
        BACK,
    };

    ChunkMesh* create_chunk_mesh(const Chunk &chunk);

    void delete_chunk_mesh(ChunkMesh *mesh);

    static void add_face(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer,
                         Direction direction,
                         Block block, glm::vec3 block_position);

    static std::vector<float> get_face_vertices(Direction direction, const AtlasTextureCoordinates &coords);

    static void fill_chunk_vertex_and_index_buffer(std::vector<float> &vertex_buffer,
                                                   std::vector<uint32_t> &index_buffer, const Chunk &chunk);

    static AtlasTextureCoordinates get_block_texture_coordinates(Block &block, Direction direction);

    static float get_face_brightness(Direction direction);
};


#endif //MESH_GENERATOR_H
