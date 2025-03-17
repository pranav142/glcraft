//
// Created by pknadimp on 3/14/25.
//

#include "world_gen.h"

void WorldGenerator::generate_chunk(Chunk &chunk) const {
    if (chunk.position().y < 0) {
        return;
    }

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            // TODO: @Speed use heightmaps to cache this value
            float noise = m_noise.GetNoise(chunk.position().x + x, chunk.position().z + z);

            int height = (noise + 1) * 120;
            constexpr int water_level = 120;

            int index = 0;
            for (int y = chunk.position().y; y < chunk.position().y + CHUNK_HEIGHT; y++) {
                if (y < height) {
                    Block block = create_block(BlockTypeID::STONE);
                    chunk.set_block(x, index, z, block);
                } else if (y < water_level) {
                    Block block = create_block(BlockTypeID::WATER);
                    chunk.set_block(x, index, z, block);
                }

                index++;
            }
        }
    }
}
