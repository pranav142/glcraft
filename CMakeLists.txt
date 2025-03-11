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
)

target_link_libraries(minecraft PUBLIC glfw)

target_include_directories(minecraft PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
)

add_custom_command(TARGET minecraft PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
        COMMENT "Copying shaders before running minecraft..."
)

add_custom_command(TARGET minecraft PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
        COMMENT "Copying assets before running minecraft..."
)