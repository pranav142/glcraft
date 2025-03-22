//
// Created by pknadimp on 3/14/25.
//

#ifndef WORLD_GEN_H
#define WORLD_GEN_H

#include "chunk.h"
#include "FastNoiseLite.h"
#include "world.h"
#include <random>

class WorldGenerator {
public:
    WorldGenerator(int seed, World& world) : m_seed(seed), m_world(world) {
        m_noise.SetSeed(seed);
        m_noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    }

    void generate_chunk(Chunk &chunk);

private:
    double noise(double nx, double nz) const;

    bool should_place_tree(double nx, double nz) const;

    bool should_add_grass_texture(double nx, double nz) const;

    void create_tree(Chunk &chunk, int x, int y, int z);

    void add_block(Chunk& chunk, Block block, int x, int y, int z);

private:
    int m_seed = 0;
    FastNoiseLite m_noise = FastNoiseLite();
    std::vector<float> m_height_map = std::vector<float>(128 * 128);

//    std::mt19937 m_generator = std::mt19937(m_seed);
//
//    std::uniform_int_distribution<std::mt19937::result_type> m_tree_distribution =
//            std::uniform_int_distribution<std::mt19937::result_type>(1, 1000);
//
    World& m_world;
};

#endif //WORLD_GEN_H
