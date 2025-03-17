//
// Created by pknadimp on 3/11/25.
//

#include "mesh_generator.h"

#include <iostream>
#include <glad/glad.h>

#include "block_registry.h"
#include "texture_manager.h"
#include "timer.h"

// TODO: Passing In World is bad practice
// TODO: Adjust to take in neighboring blocks
renderer::ChunkMesh *renderer::create_chunk_mesh(const Chunk &chunk, const World &world) {
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    std::vector<float> vertex_buffer;
    std::vector<uint32_t> index_buffer;

    vertex_buffer.reserve(40000);
    index_buffer.reserve(10000);

    TIME_FUNCTION(fill_chunk_vertex_and_index_buffer(vertex_buffer, index_buffer, chunk, world), "Filling vertices");

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(uint32_t), index_buffer.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), vertex_buffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    auto chunk_mesh = new ChunkMesh();
    chunk_mesh->EBO = EBO;
    chunk_mesh->VBO = VBO;
    chunk_mesh->VAO = VAO;
    chunk_mesh->num_indices = index_buffer.size();
    chunk_mesh->position = chunk.position();
    return chunk_mesh;
}

void renderer::delete_chunk_mesh(ChunkMesh *mesh) {
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    glDeleteBuffers(1, &mesh->EBO);

    delete mesh;
}

void renderer::add_face(std::vector<float> &vertex_buffer, std::vector<uint32_t> &index_buffer, Direction direction,
                        Block block, glm::vec3 block_position) {
    AtlasTextureCoordinates tex_coords = get_block_texture_coordinates(block, direction);
    float brightness = get_face_brightness(direction);

    int vertex_count = vertex_buffer.size() / 6;

    const FaceTemplate &template_face = FACE_TEMPLATES[static_cast<int>(direction)];

    for (int i = 0; i < 4; i++) {
        // Position
        vertex_buffer.push_back(block_position.x + template_face.vertices[i * 5]);
        vertex_buffer.push_back(block_position.y + template_face.vertices[i * 5 + 1]);
        vertex_buffer.push_back(block_position.z + template_face.vertices[i * 5 + 2]);

        float u_factor = template_face.vertices[i * 5 + 3];
        float v_factor = template_face.vertices[i * 5 + 4];

        // Texture
        vertex_buffer.push_back(tex_coords.min_u + u_factor * (tex_coords.max_u - tex_coords.min_u));
        vertex_buffer.push_back(tex_coords.min_v + v_factor * (tex_coords.max_v - tex_coords.min_v));

        // Brightnes
        vertex_buffer.push_back(brightness);
    }

    index_buffer.push_back(vertex_count);
    index_buffer.push_back(vertex_count + 1);
    index_buffer.push_back(vertex_count + 2);
    index_buffer.push_back(vertex_count + 2);
    index_buffer.push_back(vertex_count + 3);
    index_buffer.push_back(vertex_count);
}

void renderer::fill_chunk_vertex_and_index_buffer(std::vector<float> &vertex_buffer,
                                                  std::vector<uint32_t> &index_buffer, const Chunk &chunk,
                                                  const World &world) {
    // Checks Adjacent Blocks and only
    // adds vertices that are not covered
    // by another block
    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_LENGTH; k++) {
                auto block = chunk.get_block(i, j, k);
                if (block.type == BlockTypeID::EMPTY) {
                    continue;
                }

                glm::vec3 block_position = glm::vec3(i, j, k) + chunk.position();
                auto top_block = renderer::get_block(i, j + 1, k, chunk, world);
                if (top_block.type == BlockTypeID::EMPTY) {
                    add_face(vertex_buffer, index_buffer, Direction::UP, block, block_position);
                }

                auto bottom_block = renderer::get_block(i, j - 1, k, chunk, world);
                if (bottom_block.type == BlockTypeID::EMPTY) {
                    add_face(vertex_buffer, index_buffer, Direction::DOWN, block, block_position);
                }

                auto right_block = renderer::get_block(i + 1, j, k, chunk, world);
                if (right_block.type == BlockTypeID::EMPTY) {
                    add_face(vertex_buffer, index_buffer, Direction::RIGHT, block, block_position);
                }

                auto left_block = renderer::get_block(i - 1, j, k, chunk, world);
                if (left_block.type == BlockTypeID::EMPTY) {
                    add_face(vertex_buffer, index_buffer, Direction::LEFT, block, block_position);
                }

                auto front_block = renderer::get_block(i, j, k + 1, chunk, world);
                if (front_block.type == BlockTypeID::EMPTY) {
                    add_face(vertex_buffer, index_buffer, Direction::FRONT, block, block_position);
                }

                auto back_block = renderer::get_block(i, j, k - 1, chunk, world);
                if (back_block.type == BlockTypeID::EMPTY) {
                    add_face(vertex_buffer, index_buffer, Direction::BACK, block, block_position);
                }
            }
        }
    }
}

renderer::AtlasTextureCoordinates renderer::get_block_texture_coordinates(Block &block, Direction direction) {
    const BlockType &block_type = BlockRegistry::get_block(block.type);

    switch (direction) {
        case Direction::UP:
            return block_type.textures.top;
        case Direction::DOWN:
            return block_type.textures.bottom;
        case Direction::BACK:
            return block_type.textures.back;
        case Direction::LEFT:
            return block_type.textures.left;
        case Direction::RIGHT:
            return block_type.textures.right;
        case Direction::FRONT:
            return block_type.textures.front;
    }
    return {0, 0, 0, 0};
}

float renderer::get_face_brightness(Direction direction) {
    switch (direction) {
        case Direction::UP:
            return 0.9f; // Bright but not direct sunlight
        case Direction::DOWN:
            return 0.3f; // Very dark as it faces away from all light
        case Direction::RIGHT:
            return 1.0f; // Brightest face, directly facing the east sun
        case Direction::LEFT:
            return 0.3f; // Opposite from sun, only gets ambient light
        case Direction::FRONT:
            return 0.65f; // Gets partial sunlight
        case Direction::BACK:
            return 0.45f; // Less direct light than front
    }
    return 1.0f;
}

Block renderer::get_block(int x, int y, int z, const Chunk &chunk, const World &world) {
    if (chunk.coordinate_in_bounds(x, y, z)) {
        return chunk.get_block(x, y, z);
    }

    glm::vec3 world_position = chunk.position() + glm::vec3(x, y, z);
    glm::vec3 chunk_position = world_position_to_chunk_position(world_position);

    auto adjacent_chunk_opt= world.get_chunk(chunk_position);
    if (!adjacent_chunk_opt) {
        return empty_block;
    }

    const Chunk& adjacent_chunk = adjacent_chunk_opt->get();
    glm::vec3 relative_position = world_position - adjacent_chunk.position();

    return adjacent_chunk.get_block(
        relative_position.x,
        relative_position.y,
        relative_position.z
    );
}
