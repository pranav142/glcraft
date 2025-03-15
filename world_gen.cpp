//
// Created by pknadimp on 3/14/25.
//

#include "world_gen.h"

void WorldGenerator::generate_chunk(Chunk &chunk) const {
    // map height from 0 to 60
    if (chunk.position().y < 0) {
        return;
    }

    for (int z = 0; z < CHUNK_LENGTH; z++) {
        for (int x = 0; x < CHUNK_WIDTH; x++) {
            float noise = m_noise.GetNoise(chunk.position().x + x, chunk.position().z + z);
            int height = std::min((noise + 1) * 120, chunk.position().y + CHUNK_HEIGHT);
            int index = 0;
            for (int y = chunk.position().y; y < height; y++) {
                Block block = create_block(BlockTypeID::STONE);
                chunk.set_block(x, index, z, block);
                index++;
            }
        }
    }
}
