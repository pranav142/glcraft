//
// Created by pknadimp on 3/11/25.
//

#ifndef BLOCK_H
#define BLOCK_H

#include <glm/glm.hpp>

#include "texture_manager.h"

enum class BlockTypeID : uint8_t {
    GRASS = 0,
    STONE = 1,
    DIRT = 2,
    WATER = 3,
    EMPTY = 4,
    AIR = 5,
    COUNT
};

struct Block {
    BlockTypeID type = BlockTypeID::AIR;
};

// Stores Textures Of All Sides
struct BlockTextures {
    renderer::AtlasTextureCoordinates top, bottom, left, right, front, back;
};

class BlockType {
public:
    BlockTextures textures;
    bool is_transparent = false;
};

Block create_block(BlockTypeID);

#endif //BLOCK_H
