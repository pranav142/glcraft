//
// Created by pknadimp on 3/14/25.
//

#ifndef WORLD_GEN_H
#define WORLD_GEN_H

#include "chunk.h"
#include "FastNoiseLite.h"

class WorldGenerator {
public:
    WorldGenerator(int seed) : m_seed(seed) {
        m_noise.SetSeed(seed);
        m_noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    }

    void generate_chunk(Chunk& chunk) const;

private:
    double noise(double nx, double nz) const;

    double ridge_noise(double value, double ridge_factor) const;
private:
    int m_seed = 0;
    FastNoiseLite m_noise = FastNoiseLite();
    std::vector<float> m_height_map = std::vector<float>(128 * 128);
};

#endif //WORLD_GEN_H
