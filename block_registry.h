//
// Created by pknadimp on 3/12/25.
//

#ifndef BLOCK_REGISTRY_H
#define BLOCK_REGISTRY_H

#include "block.h"

class BlockRegistry {
public:
    BlockRegistry() {
        initialize();
    };

    // TODO: @Clarity Rename this: it is not returning a block
    static const BlockType &get_block(BlockTypeID id);


private:
    static void initialize();

    static void create_grass_block();

    static void create_dirt_block();

    static void create_stone_block();

    static void create_water_block();
private:
    static std::array<BlockType, static_cast<int>(BlockTypeID::COUNT)> m_blocks;
};



#endif //BLOCK_REGISTRY_H
