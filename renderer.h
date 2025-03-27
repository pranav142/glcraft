//
// Created by pknadimp on 3/11/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "mesh_generator.h"
#include "shader.h"

#include <glm/glm.hpp>

#include "crosshair.h"
#include "texture_manager.h"

namespace renderer {
    class Renderer {
    public:
        Renderer() = default;

        void initialize(int width, int height);

        void resize(int width, int height);

        void begin_frame();

        void enable_wireframe();

        void enable_culling();

        void render_chunk(const ChunkMesh &chunk_mesh, const glm::mat4 &view_mat, bool is_transparent) const;

        void render_ui(int width, int height);

        glm::mat4 projection_matrix() const;

    private:
        void update_matrices(int width, int height);

    private:
        glm::mat4 m_projection_matrix = glm::mat4(1.0f);
        glm::mat4 m_ortho_matrix = glm::mat4(1.0f);

        Shader m_block_shader = Shader();
        Shader m_crosshair_shader = Shader();
        TextureManager m_texture_manager = TextureManager();

        CrossHair m_crosshair = CrossHair();
    };
} // renderer


#endif //RENDERER_H
