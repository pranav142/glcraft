//
// Created by pknadimp on 3/14/25.
//

#include "world_gen.h"

#include <iostream>

void WorldGenerator::generate_chunk(Chunk &chunk) const {
    if (chunk.position().y < 0) {
        return;
    }

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            // TODO: @Speed use heightmaps to cache this value
            float nx = chunk.position().x + x;
            float nz = chunk.position().z + z;

            float elevation = 0.6 * noise(1 * nx, 1 * nz)
                              + 0.4 * noise(2 * nx, 2 * nz)
                              + 0.2 * noise(4 * nx, 4 * nz);

            elevation /= (0.6 + 0.4 + 0.2);

            elevation = std::pow(elevation * 1.2, 1.5);

            int height = elevation * 150 + 1;

            int water_level = 50;

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
