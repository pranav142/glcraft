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
    struct Mesh {
        unsigned int VBO = 0, VAO = 0, EBO = 0;
        int num_indices = 0;
    };

    struct ChunkMesh {
        Mesh opaque_mesh;
        Mesh transparent_mesh;
        Mesh texture_mesh;
        glm::vec3 position = glm::vec3(0, 0, 0);
    };


    struct FaceTemplate {
        float vertices[20];
    };

    static const FaceTemplate FACE_TEMPLATES[6] = {
        {
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f
        },
        {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f
        },
        {
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 0.0f
        },
        {
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 0.0f
        },
        {
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f
        },
        {
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f
        }
    };

    static const FaceTemplate TEXTURE_TEMPLATES[2] = {
        {
            -0.4f, -0.5f, 0.4f, 0.0f, 1.0f,
            0.4f, -0.5f, -0.4f, 1.0f, 1.0f,
            0.4f, 0.5f, -0.4f, 1.0f, 0.0f,
            -0.4f, 0.5f, 0.4f, 0.0f, 0.0f,
        },
        {
            0.4f, -0.5f, 0.4f, 0.0f, 1.0f,
            -0.4f, -0.5f, -0.4f, 1.0f, 1.0f,
            -0.4f, 0.5f, -0.4f, 1.0f, 0.0f,
            0.4f, 0.5f, 0.4f, 0.0f, 0.0f,
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

    ChunkMesh *create_chunk_mesh(const Chunk &chunk, const World &world);

    void delete_chunk_mesh(ChunkMesh *mesh);

    static void add_face(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer,
                         Direction direction,
                         Block block, glm::vec3 block_position);

    static void add_texture(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer,
                            Block block, glm::vec3 block_position);

    static void fill_chunk_vertex_and_index_buffer(std::vector<float> &opaque_vertex_buffer,
                                                   std::vector<uint32_t> &opaque_index_buffer,
                                                   std::vector<float> &transparent_vertex_buffer,
                                                   std::vector<uint32_t> &transparent_index_buffer,
                                                   std::vector<float> &texture_vertex_buffer,
                                                   std::vector<uint32_t> &texture_index_buffer,
                                                   const Chunk &chunk,
                                                   const World &world);

    static AtlasTextureCoordinates get_block_texture_coordinates(Block &block, Direction direction);

    static float get_face_brightness(Direction direction);

    static Block get_block(int x, int y, int z, const Chunk &chunk, const World &world);

    void initialize_mesh(Mesh &mesh, const std::vector<float> &vertex_buffer,
                         const std::vector<uint32_t> &index_buffer);

    static bool should_add_face(const Block &adjacent_block, const Block &block);
};


#endif //MESH_GENERATOR_H
