//
// Created by pknadimp on 3/11/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "mesh_generator.h"
#include "shader.h"

#include <glm/glm.hpp>

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

        void render_chunk(const ChunkMesh &chunk_mesh, const glm::mat4 &view_mat) const;

    private:
        void update_projection_matrix(int width, int height);

    private:
        glm::mat4 m_projection_matrix = glm::mat4(1.0f);
        Shader m_block_shader = Shader();
        TextureManager m_texture_manager = TextureManager();
    };
} // renderer


#endif //RENDERER_H
