//
// Created by pknadimp on 3/26/25.
//

#include "crosshair.h"

void renderer::CrossHair::initialize() {
    create_mesh();
}

const renderer::Mesh& renderer::CrossHair::get_mesh() const {
    return m_mesh;
}

void renderer::CrossHair::create_mesh() {
    std::vector<float> vertex_buffer = {
        -0.5, -0.5,  // bottom-left
         0.5, -0.5,  // bottom-right
         0.5,  0.5,  // top-right
        -0.5,  0.5   // top-left
    };

    std::vector<uint32_t> index_buffer = {
        0, 1, 2,  // first triangle
        2, 3, 0   // second triangle
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer.size() * sizeof(uint32_t), index_buffer.data(),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer.size() * sizeof(float), vertex_buffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);


    m_mesh.EBO = EBO;
    m_mesh.VBO = VBO;
    m_mesh.VAO = VAO;
    m_mesh.num_indices = index_buffer.size();
}

void renderer::CrossHair::delete_mesh() const {
    renderer::delete_mesh(m_mesh);
}
