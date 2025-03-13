//
// Created by pknadimp on 3/11/25.
//

#include "mesh_generator.h"

#include <iostream>
#include <bits/ranges_algobase.h>
#include <glad/glad.h>
#include "texture_manager.h"


renderer::ChunkMesh renderer::create_chunk_mesh(const Chunk &chunk) {
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    std::vector<float> vertex_buffer;
    std::vector<uint32_t> index_buffer;

    vertex_buffer.reserve(3600);
    index_buffer.reserve(1500);

    fill_chunk_vertex_and_index_buffer(vertex_buffer, index_buffer, chunk);

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

    std::vector<float> vertices = get_face_vertices(direction, tex_coords);
    if (vertices.empty()) {
        return;
    }

    int vertex_count = vertex_buffer.size() / 5;

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
                0.5f, 0.5f, -0.5f,
                -0.5f, 0.5f, -0.5f,
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

void renderer::fill_chunk_vertex_and_index_buffer(std::vector<float> &vertex_buffer,
                                                  std::vector<uint32_t> &index_buffer, const Chunk &chunk) {
    // Checks Adjacent Blocks and only
    // adds vertices that are not covered
    // by another block
    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_LENGTH; k++) {
                auto block = chunk.get_block(i, j, k);
                if (block.type == renderer::TextureType::DIRT) {
                    continue;
                }

                glm::vec3 block_position = glm::vec3(i, j, k) + chunk.position();

                auto top_block = chunk.get_block(i, j + 1, k);
                if (top_block.type == TextureType::DIRT) {
                    add_face(vertex_buffer, index_buffer, Direction::UP, block, block_position);
                }

                auto bottom_block = chunk.get_block(i, j - 1, k);
                if (bottom_block.type == TextureType::DIRT) {
                    add_face(vertex_buffer, index_buffer, Direction::DOWN, block, block_position);
                }

                auto right_block = chunk.get_block(i + 1, j, k);
                if (right_block.type == TextureType::DIRT) {
                    add_face(vertex_buffer, index_buffer, Direction::RIGHT, block, block_position);
                }

                auto left_block = chunk.get_block(i - 1, j, k);
                if (left_block.type == TextureType::DIRT) {
                    add_face(vertex_buffer, index_buffer, Direction::LEFT, block, block_position);
                }

                auto front_block = chunk.get_block(i, j, k + 1);
                if (front_block.type == TextureType::DIRT) {
                    add_face(vertex_buffer, index_buffer, Direction::FRONT, block, block_position);
                }

                auto back_block = chunk.get_block(i, j, k - 1);
                if (back_block.type == TextureType::DIRT) {
                    add_face(vertex_buffer, index_buffer, Direction::BACK, block, block_position);
                }
            }
        }
    }
}
