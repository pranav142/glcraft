//
// Created by pknadimp on 3/14/25.
//

#include "world_gen.h"

#include <iostream>

void WorldGenerator::generate_chunk(Chunk &chunk) {
    if (chunk.position().y < 0) {
        return;
    }

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            // TODO: @Speed use heightmaps to cache this value
            float nx = chunk.position().x + x;
            float nz = chunk.position().z + z;

            float mountains = 0.95 * noise(0.75 * nx, 0.75 * nz) + 0.05 * noise(6 * nx, 6 * nz) + 0.3;
            // 0 to 1.2
            mountains = std::pow(mountains, 4) * 80;

            float valleys = noise(0.2 * nx, 0.2 * nz) * 100;

            float elevation = mountains + valleys;

            int height = elevation + 1;

            int water_level = 60;

            float material_noise = noise(nx * 6.0, nz * 6.0) * 2 - 1;
            float patch_noise = noise(nx * 10.0, nz * 10.0) * 2;

            int index = 0;
            for (int y = chunk.position().y; y < chunk.position().y + CHUNK_HEIGHT; y++) {
                if (y <= height) {
                    if (y > 120 + 15 * material_noise) {
                        Block block = create_block(BlockTypeID::SNOW);
                        chunk.set_block(x, index, z, block);
                    } else if (y > 80 + 15 * material_noise) {
                        Block block = create_block(BlockTypeID::STONE);
                        chunk.set_block(x, index, z, block);
                    } else if (y > water_level + 5 + 15 * material_noise) {
                        if (y == height) {
                            Block block = create_block(BlockTypeID::GRASS);
                            chunk.set_block(x, index, z, block);
                            if (should_place_tree(nx, nz) && y > water_level) {
                                create_tree(chunk, x, index, z);
                            } else if (should_add_grass_texture(nx, nz) && y > water_level) {
                                // TODO: @Clarity rename from foliage
                                Block foliage = create_block(BlockTypeID::GRASS_TEXTURE);
                                add_block(chunk, foliage, x, index + 1, z);
                            } else if (should_add_flower_texture(nx, nz) && y >= water_level) {
                                Block rose_flower = create_block(BlockTypeID::ROSE_TEXTURE);
                                add_block(chunk, rose_flower, x, index + 1, z);
                            }
                        } else {
                            Block block = create_block(BlockTypeID::DIRT);
                            chunk.set_block(x, index, z, block);
                        }
                    } else {
                        if (patch_noise < 0.5) {
                            Block block = create_block(BlockTypeID::GRAVEL);
                            chunk.set_block(x, index, z, block);
                        } else {
                            Block block = create_block(BlockTypeID::SAND);
                            chunk.set_block(x, index, z, block);
                        }
                    }
                } else if (y < water_level) {
                    Block block = create_block(BlockTypeID::WATER);
                    chunk.set_block(x, index, z, block);
                }

                index++;
            }
        }
    }
}

double WorldGenerator::noise(double nx, double nz) const {
    // Scales noise from -1 to 1 to 0 to 1
    return m_noise.GetNoise(nx, nz) / 2.0 + 0.5;
}

// TODO: Tree Generation Should Be More Customizable Based on Biome
bool WorldGenerator::should_place_tree(double nx, double nz) const {
    double primary_noise = std::pow(noise(50 * nx, 50 * nz), 3);
    constexpr double TREE_THRESHOLD = 0.65; // 1/1000 chance

    return primary_noise > TREE_THRESHOLD;
}

bool WorldGenerator::should_add_grass_texture(double nx, double nz) const {
    double grass_patch_noise = noise(5 * nx, 5 * nz);
    constexpr double GRASS_PATCH_THRESHOLD = 0.6;
    return grass_patch_noise > GRASS_PATCH_THRESHOLD;
}

bool WorldGenerator::should_add_flower_texture(double nx, double nz) const {
    double grass_patch_noise = noise(10 * nx, 10 * nz);
    constexpr double FLOWER_PATCH_THRESHOLD = 0.8;
    return grass_patch_noise > FLOWER_PATCH_THRESHOLD;

}

void WorldGenerator::create_tree(Chunk &chunk, int x, int y, int z) {
    Block block = create_block(BlockTypeID::WOOD);
    constexpr int TREE_HEIGHT = 4;
    for (int i = 1; i <= TREE_HEIGHT; i++) {
        add_block(chunk, block, x, y + i, z);
    }

    block = create_block(BlockTypeID::LEAVES);
    for (int dy = -1; dy <= 0; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            for (int dz = -2; dz <= 2; dz++) {
                if (dx == 0 && dz == 0) {
                    continue;
                }
                add_block(chunk, block, x + dx, y + TREE_HEIGHT + dy, z + dz);
            }
        }
    }

    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            add_block(chunk, block, x + dx, y + TREE_HEIGHT + 1, z + dz);
        }
    }


    add_block(chunk, block, x, y + TREE_HEIGHT + 2, z);
    add_block(chunk, block, x, y + TREE_HEIGHT + 2, z + 1);
    add_block(chunk, block, x, y + TREE_HEIGHT + 2, z - 1);
    add_block(chunk, block, x + 1, y + TREE_HEIGHT + 2, z);
    add_block(chunk, block, x - 1, y + TREE_HEIGHT + 2, z);
}

// takes relative x, y, z to chunk if out of bounds stores in to be loaded
void WorldGenerator::add_block(Chunk &chunk, Block block, int x, int y, int z) {
    if (chunk.coordinate_in_bounds(x, y, z)) {
        chunk.set_block(x, y, z, block);
    }

    UnloadedBlock unloaded_block{
        .block = block,
        .original_chunk_position = chunk.position(),
        .global_position = chunk.position() + glm::vec3(x, y, z),
    };

    m_world.add_unloaded_block(unloaded_block);
}
