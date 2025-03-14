//
// Created by pknadimp on 3/11/25.
//
#define STB_IMAGE_IMPLEMENTATION
#include "texture_manager.h"
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

void renderer::TextureManager::initialize() {
    create_atlas_texture();
}

void renderer::TextureManager::bind_texture() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_atlas_texture);
}

renderer::AtlasTextureCoordinates renderer::TextureManager::get_texture_coordinate(TextureType type) {
    AtlasPosition position = block_positions[static_cast<uint32_t>(type)];

    AtlasTextureCoordinates coords{};

    coords.min_u = static_cast<float>(position.x) / 16.0f;
    coords.max_u = static_cast<float>(position.x + 1) / 16.0f;

    coords.min_v = 1.0f - static_cast<float>(position.y) / 16.0f;
    coords.max_v = 1.0f - static_cast<float>(position.y + 1) / 16.0f;

    return coords;
}

void renderer::TextureManager::create_atlas_texture() {
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("assets/blocks.png", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load image" << std::endl;
        return;
    }

    glGenTextures(1, &m_atlas_texture);
    glBindTexture(GL_TEXTURE_2D, m_atlas_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}
