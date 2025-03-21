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

            // + 0.4 * noise(1 * nx, 1 * nz)
            // + 0.2 * noise(1 * nx, 1 * nz);
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
                            if (should_place_tree()) {
                                create_tree(chunk, x, index, z);
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

bool WorldGenerator::should_place_tree() {
    return m_tree_distribution(m_generator) > 995;
}

 void WorldGenerator::create_tree(Chunk &chunk, int x, int y, int z) {
    Block block = create_block(BlockTypeID::WOOD);
    constexpr int TREE_HEIGHT = 4;
    for (int i = 1; i <= TREE_HEIGHT; i++) {
        chunk.set_block(x, y + i, z, block);
        chunk.set_block(x, y + i, z, block);
        chunk.set_block(x, y + i, z, block);
        chunk.set_block(x, y + i, z, block);
    }

    block = create_block(BlockTypeID::LEAVES);
    for (int dy = -1; dy <= 0; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            for (int dz = -2; dz <= 2; dz++) {
                if (dx == 0 && dz == 0) {
                    continue;
                }
                chunk.set_block(x + dx, y + TREE_HEIGHT + dy, z + dz, block);
            }
        }
    }

    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            chunk.set_block(x + dx, y + TREE_HEIGHT + 1, z + dz, block);
        }
    }


    chunk.set_block(x, y + TREE_HEIGHT + 2, z, block);
    chunk.set_block(x, y + TREE_HEIGHT + 2, z + 1, block);
    chunk.set_block(x, y + TREE_HEIGHT + 2, z - 1, block);
    chunk.set_block(x + 1, y + TREE_HEIGHT + 2, z, block);
    chunk.set_block(x - 1, y + TREE_HEIGHT + 2, z, block);
}
