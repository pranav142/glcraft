//
// Created by pknadimp on 3/11/25.
//

#include "mesh_generator.h"

#include <iostream>
#include <ostream>
#include <bits/fs_fwd.h>
#include <glad/glad.h>
#include "texture_manager.h"

std::vector<float> renderer::create_block_vertex_buffer(const AtlasTextureCoordinates &coords) {
    std::vector<float> vertices = {
        // Back face
        -0.5f, -0.5f, -0.5f, coords.min_u, coords.min_v, // Bottom Left Back
        0.5f, -0.5f, -0.5f, coords.max_u, coords.min_v, // Bottom Right Back
        0.5f, 0.5f, -0.5f, coords.max_u, coords.max_v, // Top Right Back
        -0.5f, 0.5f, -0.5f, coords.min_u, coords.max_v, // Top Left Back

        // Front face
        -0.5f, -0.5f, 0.5f, coords.min_u, coords.min_v, // Bottom Left Front
        0.5f, -0.5f, 0.5f, coords.max_u, coords.min_v, // Bottom Right Front
        0.5f, 0.5f, 0.5f, coords.max_u, coords.max_v, // Top Right Front
        -0.5f, 0.5f, 0.5f, coords.min_u, coords.max_v, // Top Left Front

        // Left face
        -0.5f, -0.5f, -0.5f, coords.min_u, coords.min_v, // Bottom Back Left
        -0.5f, -0.5f, 0.5f, coords.max_u, coords.min_v, // Bottom Front Left
        -0.5f, 0.5f, 0.5f, coords.max_u, coords.max_v, // Top Front Left
        -0.5f, 0.5f, -0.5f, coords.min_u, coords.max_v, // Top Back Left

        // Right face
        0.5f, -0.5f, -0.5f, coords.min_u, coords.min_v, // Bottom Back Right
        0.5f, -0.5f, 0.5f, coords.max_u, coords.min_v, // Bottom Front Right
        0.5f, 0.5f, 0.5f, coords.max_u, coords.max_v, // Top Front Right
        0.5f, 0.5f, -0.5f, coords.min_u, coords.max_v, // Top Back Right

        // Bottom face
        -0.5f, -0.5f, -0.5f, coords.min_u, coords.min_v, // Back Left Bottom
        0.5f, -0.5f, -0.5f, coords.max_u, coords.min_v, // Back Right Bottom
        0.5f, -0.5f, 0.5f, coords.max_u, coords.max_v, // Front Right Bottom
        -0.5f, -0.5f, 0.5f, coords.min_u, coords.max_v, // Front Left Bottom

        // Top face
        -0.5f, 0.5f, -0.5f, coords.min_u, coords.min_v, // Back Left Top
        0.5f, 0.5f, -0.5f, coords.max_u, coords.min_v, // Back Right Top
        0.5f, 0.5f, 0.5f, coords.max_u, coords.max_v, // Front Right Top
        -0.5f, 0.5f, 0.5f, coords.min_u, coords.max_v // Front Left Top
    };
    return vertices;
}

renderer::BlockMesh renderer::create_block_mesh(const Block &block) {
    unsigned int VAO, VBO, EBO;

    static uint32_t indices[36] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    AtlasTextureCoordinates coords = TextureManager::get_texture_coordinate(block.type);
    std::vector<float> buffer = create_block_vertex_buffer(coords);

    glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    BlockMesh block_mesh{};
    block_mesh.VAO = VAO;
    block_mesh.VBO = VBO;
    block_mesh.EBO = EBO;
    block_mesh.num_indices = sizeof(indices) / sizeof(uint32_t);
    return block_mesh;
}

renderer::ChunkMesh renderer::create_chunk_mesh(const Chunk &chunk) {
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    std::vector<float> vertex_buffer;
    std::vector<uint32_t> index_buffer;

    // TODO: get rid of magic 16
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                auto block = chunk.get_block(i, j, k);
                if (block.type == renderer::TextureType::DIRT) {
                    continue;
                }

                glm::vec3 block_position = glm::vec3(i, j, k) + chunk.position();
                if (j + 1 <= 15) {
                    auto top_block = chunk.get_block(i, j + 1, k);
                    if (top_block.type == renderer::TextureType::DIRT) {
                        add_face(vertex_buffer, index_buffer, Direction::UP, block, block_position);
                    }
                } else {
                    add_face(vertex_buffer, index_buffer, Direction::UP, block, block_position);
                }

                if (j - 1 >= 0) {
                    auto top_block = chunk.get_block(i, j - 1, k);
                    if (top_block.type == renderer::TextureType::DIRT) {
                        add_face(vertex_buffer, index_buffer, Direction::DOWN, block, block_position);
                    }
                } else {
                    add_face(vertex_buffer, index_buffer, Direction::DOWN, block, block_position);
                }


                if (i + 1 <= 15) {
                    auto top_block = chunk.get_block(i + 1, j, k);
                    if (top_block.type == renderer::TextureType::DIRT) {
                        add_face(vertex_buffer, index_buffer, Direction::RIGHT, block, block_position);
                    }
                } else {
                    add_face(vertex_buffer, index_buffer, Direction::RIGHT, block, block_position);
                }

                if (i - 1 >= 0) {
                    auto top_block = chunk.get_block(i - 1, j, k);
                    if (top_block.type == renderer::TextureType::DIRT) {
                        add_face(vertex_buffer, index_buffer, Direction::LEFT, block, block_position);
                    }
                } else {
                    add_face(vertex_buffer, index_buffer, Direction::LEFT, block, block_position);
                }


                if (k + 1 <= 15) {
                    auto top_block = chunk.get_block(i, j, k + 1);
                    if (top_block.type == renderer::TextureType::DIRT) {
                        add_face(vertex_buffer, index_buffer, Direction::FRONT, block, block_position);
                    }
                } else {
                    add_face(vertex_buffer, index_buffer, Direction::FRONT, block, block_position);
                }

                if (k - 1 >= 0) {
                    auto top_block = chunk.get_block(i, j, k - 1);
                    if (top_block.type == renderer::TextureType::DIRT) {
                        add_face(vertex_buffer, index_buffer, Direction::BACK, block, block_position);
                    }
                } else {
                    add_face(vertex_buffer, index_buffer, Direction::BACK, block, block_position);
                }
            }
        }
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(uint32_t), index_buffer.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), vertex_buffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ChunkMesh chunk_mesh{};
    chunk_mesh.EBO = EBO;
    chunk_mesh.VBO = VBO;
    chunk_mesh.VAO = VAO;
    chunk_mesh.num_indices = index_buffer.size();
    chunk_mesh.position = chunk.position();
    return chunk_mesh;
}

void renderer::add_face(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer, Direction direction,
                        Block block, glm::vec3 block_position) {
    AtlasTextureCoordinates tex_coords = TextureManager::get_texture_coordinate(block.type);

    int vertex_count = vertex_buffer.size() / 5;

    std::vector<float> vertices = get_face_vertices(direction, tex_coords);
    for (int i = 0; i < 4; i++) {
        vertex_buffer.push_back(block_position.x + vertices[3 * i]);
        vertex_buffer.push_back(block_position.y + vertices[3 * i + 1]);
        vertex_buffer.push_back(block_position.z + vertices[3 * i + 2]);

        vertex_buffer.push_back(i == 0 || i == 3 ? tex_coords.min_u : tex_coords.max_u);
        vertex_buffer.push_back(i == 0 || i == 1 ? tex_coords.min_v : tex_coords.max_v);
    }

    index_buffer.push_back(vertex_count);
    index_buffer.push_back(vertex_count + 1);
    index_buffer.push_back(vertex_count + 2);
    index_buffer.push_back(vertex_count + 2);
    index_buffer.push_back(vertex_count + 3);
    index_buffer.push_back(vertex_count);
}

std::vector<float> renderer::get_face_vertices(Direction direction, const AtlasTextureCoordinates &coords) {
    switch (direction) {
        case Direction::UP:
            return std::vector<float>{
                0.5f, 0.5f, 0.5f,
                0.5f, 0.5f, -0.5f, // Back Right Top
                -0.5f, 0.5f, -0.5f, //
                -0.5f, 0.5f, 0.5f,
            };
        case Direction::DOWN:
            return std::vector<float>{
                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, 0.5f,
                -0.5f, -0.5f, 0.5f,
            };
        case Direction::LEFT:
            return std::vector<float>{
                // Left face
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, 0.5f,
                -0.5f, 0.5f, 0.5f,
                -0.5f, 0.5f, -0.5f,
            };
        case Direction::BACK:
            return std::vector<float>{
                0.5f, 0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, 0.5f, -0.5f,
            };
        case Direction::RIGHT:
            return std::vector<float>{
                0.5f, 0.5f, 0.5f,
                0.5f, -0.5f, 0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, 0.5f, -0.5f,
            };
        case Direction::FRONT:
            return std::vector<float>{
                -0.5f, -0.5f, 0.5f,
                0.5f, -0.5f, 0.5f,
                0.5f, 0.5f, 0.5f,
                -0.5f, 0.5f, 0.5f,
            };
        default:
            return std::vector<float>{};
    }
}
