cmake_minimum_required(VERSION 3.29)
project(minecraft)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

add_executable(minecraft
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c
        utils.cpp
        utils.h
        shader.cpp
        shader.h
        block.cpp
        block.h
        renderer.cpp
        renderer.h
        mesh_generator.cpp
        mesh_generator.h
        texture_manager.cpp
        texture_manager.h
        chunk.cpp
        chunk.h
        block_registry.cpp
        block_registry.h
        camera.cpp
        camera.h
        minecraft.cpp
        minecraft.h
        world.cpp
        world.h
        world_gen.cpp
        world_gen.h
        libs/FastNoise/FastNoiseLite.h
)

target_link_libraries(minecraft PUBLIC glfw)

target_include_directories(minecraft PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/FastNoise
)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
        COMMENT "Copying resources for minecraft..."
        DEPENDS minecraft
)