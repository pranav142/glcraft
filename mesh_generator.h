//
// Created by pknadimp on 3/11/25.
//

#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "block.h"

#include "texture_manager.h"

namespace renderer {
    struct BlockMesh {
        unsigned int VBO, VAO, EBO;
        glm::vec3 position;
        int num_indices;
    };

    std::vector<float> create_block_vertex_buffer(const AtlasTextureCoordinates &coords);

    BlockMesh create_block_mesh(const Block &block);

    BlockMesh delete_block_mesh(BlockMesh &block);
};


#endif //MESH_GENERATOR_H
