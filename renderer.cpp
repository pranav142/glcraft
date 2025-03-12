//
// Created by pknadimp on 3/11/25.
//

#include "renderer.h"
#include "glad/glad.h"
#include <glm/ext.hpp>

renderer::Renderer::Renderer(int width, int height) {
    glViewport(0, 0, width, height);
    update_projection_matrix(width, height);
}

void renderer::Renderer::resize(int width, int height) {
    glViewport(0, 0, width, height);
    update_projection_matrix(width, height);
}

void renderer::Renderer::begin_frame() {
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::Renderer::enable_wireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void renderer::Renderer::enable_culling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void renderer::Renderer::render_block(const BlockMesh &block_mesh, const glm::mat4 &view_matrix) const {
    m_block_shader.use();

    m_texture_manager.bind_texture();
    m_block_shader.set_int("texture1", 0);

    m_block_shader.set_matrix("view", view_matrix);
    m_block_shader.set_matrix("projection", m_projection_matrix);

    auto model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, block_mesh.position);
    m_block_shader.set_matrix("model", model_matrix);

    glBindVertexArray(block_mesh.VAO);
    glDrawElements(GL_TRIANGLES, block_mesh.num_indices, GL_UNSIGNED_INT, 0);
}

void renderer::Renderer::render_chunk(const renderer::ChunkMesh &chunk_mesh, const glm::mat4 &view_matrix) const {
    m_block_shader.use();

    m_texture_manager.bind_texture();
    m_block_shader.set_int("texture1", 0);

    m_block_shader.set_matrix("view", view_matrix);
    m_block_shader.set_matrix("projection", m_projection_matrix);

    auto model_matrix = glm::mat4(1.0f);
    m_block_shader.set_matrix("model", model_matrix);

    glBindVertexArray(chunk_mesh.VAO);
    glDrawElements(GL_TRIANGLES, chunk_mesh.num_indices, GL_UNSIGNED_INT, 0);
}

void renderer::Renderer::update_projection_matrix(int width, int height) {
    m_projection_matrix = glm::perspective(glm::radians(45.0f),
                                           static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                           100.0f);
}
