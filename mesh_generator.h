//
// Created by pknadimp on 3/11/25.
//

#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "block.h"
#include "chunk.h"
#include "texture_manager.h"
#include "world.h"

#include <glad/glad.h>

namespace renderer {
    struct ChunkMesh {
        unsigned int VBO = 0, VAO = 0, EBO = 0;
        glm::vec3 position = glm::vec3(0, 0, 0);
        int num_indices = 0;
    };

    struct FaceTemplate {
        float vertices[20];
    };

    static const FaceTemplate FACE_TEMPLATES[6] = {
        // UP
        {
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f
        },
        // DOWN
        {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f
        },
        // LEFT
        {
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 0.0f
        },
        // BACK
        {
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 0.0f
        },
        // RIGHT
        {
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f
        },
        // FRONT
        {
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f
        }
    };
    constexpr int RENDER_RADIUS = 6;

    static Block empty_block = Block(BlockTypeID::EMPTY);

    enum class Direction : uint8_t {
        UP,
        DOWN,
        LEFT,
        BACK,
        RIGHT,
        FRONT,
    };

    ChunkMesh *create_chunk_mesh(const Chunk &chunk, const World& world);

    void delete_chunk_mesh(ChunkMesh *mesh);

    static void add_face(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer,
                         Direction direction,
                         Block block, glm::vec3 block_position);

    static void fill_chunk_vertex_and_index_buffer(std::vector<float> &vertex_buffer,
                                                   std::vector<uint32_t> &index_buffer, const Chunk &chunk,
                                                  const World& world);

    static AtlasTextureCoordinates get_block_texture_coordinates(Block &block, Direction direction);

    static float get_face_brightness(Direction direction);

    static Block get_block(int x, int y, int z, const Chunk &chunk, const World& world);
};


#endif //MESH_GENERATOR_H
