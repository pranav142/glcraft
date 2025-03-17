//
// Created by pknadimp on 3/11/25.
//

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <glm/glm.hpp>

namespace renderer {
    enum class TextureType : uint32_t {
        GRASS_TOP,
        GRASS_SIDE,
        DIRT,
        STONE,
        WATER,
    };

    struct AtlasPosition {
        int x, y;
    };

    struct AtlasTextureCoordinates {
        float min_u, max_u, min_v, max_v;
    };

    static AtlasPosition block_positions[] = {
        {0, 0},
        {1, 0},
        {2, 0},
        {3, 0},
        {15, 15},
    };

    class TextureManager {
    public:
        TextureManager() = default;

        void initialize();

        void bind_texture() const;

        [[nodiscard]] static AtlasTextureCoordinates get_texture_coordinate(TextureType type);

    private:
        void create_atlas_texture();

    private:
        static constexpr float ATLAS_WIDTH = 16.0f;
        static constexpr float ATLAS_HEIGHT = 16.0f;

        unsigned int m_atlas_texture = 0;
    };
}

#endif //TEXTURE_MANAGER_H
