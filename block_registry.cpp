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
    create_sand_block();
    create_stone_block();
    create_water_block();
    create_gravel_block();
    create_snow_block();
    create_wood_block();
    create_leaf_block();
    create_grass_texture();
    create_rose_texture();
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

void BlockRegistry::create_sand_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::SAND)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::SAND);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = false;
}

void BlockRegistry::create_gravel_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::GRAVEL)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::GRAVEL);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = false;
}

void BlockRegistry::create_snow_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::SNOW)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::SNOW);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = false;
}

void BlockRegistry::create_wood_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::WOOD)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::WOOD_SIDE);
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;


    textures.bottom = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::WOOD_TOP);
    textures.top = textures.bottom;

    block.textures = textures;
    block.is_transparent = false;
}

void BlockRegistry::create_leaf_block() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::LEAVES)];

    BlockTextures textures{};
    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::LEAF);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = true;
}

void BlockRegistry::create_grass_texture() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::GRASS_TEXTURE)];
    BlockTextures textures{};

    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::GRASS_TEXTURE);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = true;
    block.is_sprite = true;
}

void BlockRegistry::create_rose_texture() {
    BlockType &block = m_blocks[static_cast<int>(BlockTypeID::ROSE_TEXTURE)];
    BlockTextures textures{};

    textures.left = renderer::TextureManager::get_texture_coordinate(renderer::TextureType::ROSE_TEXTURE);
    textures.bottom = textures.left;
    textures.top = textures.left;
    textures.right = textures.left;
    textures.front = textures.left;
    textures.back = textures.right;

    block.textures = textures;
    block.is_transparent = true;
    block.is_sprite = true;
}
