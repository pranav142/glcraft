//
// Created by pknadimp on 3/12/25.
//

#include "block_registry.h"

std::array<BlockType, static_cast<int>(BlockTypeID::COUNT)> BlockRegistry::m_blocks;

const BlockType &BlockRegistry::get_block(BlockTypeID id) {
    return m_blocks[static_cast<int>(id)];
}

void BlockRegistry::initialize() {
    create_grass_block();
    create_dirt_block();
    create_stone_block();
    create_water_block();
}

void BlockRegistry::create_grass_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::GRASS)];

    BlockTextures textures{};
    textures.top = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::GRASS_TOP);
    textures.bottom = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::DIRT);
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::GRASS_SIDE);
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = false;
}

void BlockRegistry::create_dirt_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::DIRT)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::DIRT);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = false;
}

void BlockRegistry::create_stone_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::STONE)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::STONE);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = false;
}

void BlockRegistry::create_water_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::WATER)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::WATER);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = true;
}
