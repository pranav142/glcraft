//
// Created by pknadimp on 3/11/25.
//

#include "renderer.h"
#include "glad/glad.h"
#include <glm/ext.hpp>

void renderer::Renderer::initialize(int width, int height) {
    glViewport(0, 0, width, height);
    update_matrices(width, height);
    m_block_shader.initialize("shaders/shader.vert", "shaders/shader.frag");
    m_crosshair_shader.initialize("shaders/crosshair.vert", "shaders/crosshair.frag");
    m_texture_manager.initialize();
    m_crosshair.initialize();
}

void renderer::Renderer::resize(int width, int height) {
    glViewport(0, 0, width, height);
    update_matrices(width, height);
}

void renderer::Renderer::begin_frame() {
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void renderer::Renderer::enable_wireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void renderer::Renderer::enable_culling() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void renderer::Renderer::render_chunk(const renderer::ChunkMesh &chunk_mesh, const glm::mat4 &view_matrix,
                                      bool is_transparent) const {
    m_block_shader.use();

    m_texture_manager.bind_texture();
    m_block_shader.set_int("texture1", 0);

    m_block_shader.set_matrix("view", view_matrix);
    m_block_shader.set_matrix("projection", m_projection_matrix);

    // auto model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(chunk_mesh.position));
    auto model_matrix = glm::mat4(1.0f);
    m_block_shader.set_matrix("model", model_matrix);

    if (is_transparent && chunk_mesh.transparent_mesh.num_indices > 0) {
        // glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(chunk_mesh.transparent_mesh.VAO);
        glDrawElements(GL_TRIANGLES, chunk_mesh.transparent_mesh.num_indices, GL_UNSIGNED_INT, 0);

        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        // glDepthMask(GL_TRUE);
    } else {
        glBindVertexArray(chunk_mesh.opaque_mesh.VAO);
        glDrawElements(GL_TRIANGLES, chunk_mesh.opaque_mesh.num_indices, GL_UNSIGNED_INT, 0);

        glDisable(GL_CULL_FACE);
        glBindVertexArray(chunk_mesh.texture_mesh.VAO);
        glDrawElements(GL_TRIANGLES, chunk_mesh.texture_mesh.num_indices, GL_UNSIGNED_INT, 0);
        glEnable(GL_CULL_FACE);
    }
}

// setting these matrices isnt working
void renderer::Renderer::render_ui(int width, int height) {
    const Mesh &crosshair_mesh = m_crosshair.get_mesh();

    m_crosshair_shader.use();

    glm::mat4 view = glm::mat4(1.0f);
    m_crosshair_shader.set_matrix("view", view);
    m_crosshair_shader.set_matrix("projection", view);

    glm::mat4 model = glm::mat4(1.0f);

    // size in pixels
    float size = 20.0f;
    model = glm::scale(model, glm::vec3(size / width, size / height, 1.0f));

    m_crosshair_shader.set_matrix("model", model);

    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(crosshair_mesh.VAO);
    glDrawElements(GL_TRIANGLES, crosshair_mesh.num_indices, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}


glm::mat4 renderer::Renderer::projection_matrix() const {
    return m_projection_matrix;
}

void renderer::Renderer::update_matrices(int width, int height) {
    m_projection_matrix = glm::perspective(glm::radians(75.0f),
                                           static_cast<float>(width) / static_cast<float>(height), 0.1f,
                                           300.0f);
    m_ortho_matrix = glm::ortho(0.0f, static_cast<float>(width),
                                static_cast<float>(height), 0.0f,
                                -1.0f, 1.0f);
}
