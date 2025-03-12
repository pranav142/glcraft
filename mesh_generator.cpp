//
// Created by pknadimp on 3/11/25.
//

#include "mesh_generator.h"

#include <iostream>
#include <ostream>
#include <glad/glad.h>

std::vector<float> renderer::create_block_vertex_buffer(const AtlasTextureCoordinates &coords) {
    std::vector<float> vertices = {
        // Back face
        -0.5f, -0.5f, -0.5f,  coords.min_u, coords.min_v, // Bottom Left Back
         0.5f, -0.5f, -0.5f,  coords.max_u, coords.min_v, // Bottom Right Back
         0.5f,  0.5f, -0.5f,  coords.max_u, coords.max_v, // Top Right Back
        -0.5f,  0.5f, -0.5f,  coords.min_u, coords.max_v, // Top Left Back

        // Front face
        -0.5f, -0.5f,  0.5f,  coords.min_u, coords.min_v, // Bottom Left Front
         0.5f, -0.5f,  0.5f,  coords.max_u, coords.min_v, // Bottom Right Front
         0.5f,  0.5f,  0.5f,  coords.max_u, coords.max_v, // Top Right Front
        -0.5f,  0.5f,  0.5f,  coords.min_u, coords.max_v, // Top Left Front

        // Left face
        -0.5f, -0.5f, -0.5f,  coords.min_u, coords.min_v, // Bottom Back Left
        -0.5f, -0.5f,  0.5f,  coords.max_u, coords.min_v, // Bottom Front Left
        -0.5f,  0.5f,  0.5f,  coords.max_u, coords.max_v, // Top Front Left
        -0.5f,  0.5f, -0.5f,  coords.min_u, coords.max_v, // Top Back Left

        // Right face
         0.5f, -0.5f, -0.5f,  coords.min_u, coords.min_v, // Bottom Back Right
         0.5f, -0.5f,  0.5f,  coords.max_u, coords.min_v, // Bottom Front Right
         0.5f,  0.5f,  0.5f,  coords.max_u, coords.max_v, // Top Front Right
         0.5f,  0.5f, -0.5f,  coords.min_u, coords.max_v, // Top Back Right

        // Bottom face
        -0.5f, -0.5f, -0.5f,  coords.min_u, coords.min_v, // Back Left Bottom
         0.5f, -0.5f, -0.5f,  coords.max_u, coords.min_v, // Back Right Bottom
         0.5f, -0.5f,  0.5f,  coords.max_u, coords.max_v, // Front Right Bottom
        -0.5f, -0.5f,  0.5f,  coords.min_u, coords.max_v, // Front Left Bottom

        // Top face
        -0.5f,  0.5f, -0.5f,  coords.min_u, coords.min_v, // Back Left Top
         0.5f,  0.5f, -0.5f,  coords.max_u, coords.min_v, // Back Right Top
         0.5f,  0.5f,  0.5f,  coords.max_u, coords.max_v, // Front Right Top
        -0.5f,  0.5f,  0.5f,  coords.min_u, coords.max_v  // Front Left Top
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
    block_mesh.position = block.position;
    return block_mesh;
}
