//
// Created by pknadimp on 3/11/25.
//

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

#include "texture_manager.h"

enum class BlockTypeID : uint16_t {
    GRASS = 0,
    STONE = 1,
    DIRT = 2,
    EMPTY = 3,
    COUNT
};

struct Block {
    BlockTypeID type = BlockTypeID::EMPTY;
};

// Stores Textures Of All Sides
struct BlockTextures {
    renderer::AtlasTextureCoordinates top, bottom, left, right, front, back;
};

class BlockType {
public:
    BlockTextures textures;
};

Block create_block(BlockTypeID);

#endif //BLOCK_H
