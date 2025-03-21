//
// Created by pknadimp on 3/13/25.
//

#include "minecraft.h"
#include <iostream>
#include "block_registry.h"
#include "renderer.h"
#include "mesh_generator.h"

#include "timer.h"

bool Minecraft::initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, "glcraft", nullptr, nullptr);
    if (m_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
        if (auto instance = static_cast<Minecraft *>(glfwGetWindowUserPointer(window))) {
            instance->resize(width, height);
        }
    });

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xpos, double ypos) {
        if (auto instance = static_cast<Minecraft *>(glfwGetWindowUserPointer(window))) {
            instance->handle_mouse_move(xpos, ypos);
        }
    });

    BlockRegistry();

    m_renderer.initialize(m_width, m_height);
    m_renderer.enable_culling();
    // m_renderer.enable_wireframe();

    m_camera.enable_flying();

    m_world.initialize(m_camera.position());

    return true;
}

void Minecraft::handle_mouse_move(double xpos, double ypos) {
    if (m_first_mouse) {
        m_last_X = xpos;
        m_last_Y = ypos;
        m_first_mouse = false;
    }

    float xoffset = xpos - m_last_X;
    float yoffset = m_last_Y - ypos;
    m_last_X = xpos;
    m_last_Y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_camera.rotate(yoffset, xoffset);
}

void Minecraft::resize(int width, int height) {
    m_width = width;
    m_height = height;

    m_renderer.resize(width, height);
}

void Minecraft::run() {
    while (!glfwWindowShouldClose(m_window)) {
        float m_current_frame = glfwGetTime();
        m_delta_time = m_current_frame - m_last_frame;
        std::cout << 1 / m_delta_time << std::endl;
        m_last_frame = m_current_frame;

        process_input();

        update();
        render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Minecraft::render() {
    m_renderer.begin_frame();
    glm::mat4 view = m_camera.view_matrix();

    auto &chunks = m_world.get_chunks();
    int meshes_created = 0;
    constexpr int MAX_MESHES_PER_FRAME = 2;

    int generated = 0;
    constexpr int MAX_GENERATED_PER_FRAME = 20;

    for (auto &chunk: chunks) {
        if (chunk.get_state() == Chunk::State::REGENERATE && generated < MAX_GENERATED_PER_FRAME) {
            if (chunk.get_block_count() > 0) {
                chunk.clear_blocks();
            }
            m_world_generator.generate_chunk(chunk);
            chunk.set_state(Chunk::State::REMESH);
            generated++;

            glm::vec3 neighbor_positions[6] = {
                chunk.position() + glm::vec3(CHUNK_WIDTH, 0, 0),
                chunk.position() - glm::vec3(CHUNK_WIDTH, 0, 0),
                chunk.position() + glm::vec3(0, CHUNK_HEIGHT, 0),
                chunk.position() - glm::vec3(0, CHUNK_HEIGHT, 0),
                chunk.position() + glm::vec3(0, 0, CHUNK_LENGTH),
                chunk.position() - glm::vec3(0, 0, CHUNK_LENGTH)
            };

            for (const auto &neighbor_pos: neighbor_positions) {
                auto chunk_opt = m_world.get_chunk(neighbor_pos);
                if (!chunk_opt.has_value()) {
                    continue;
                }
                auto neighbor = chunk_opt.value();
                if (neighbor.get().get_state() == Chunk::State::READY) {
                    m_world.set_chunk_state(Chunk::State::REMESH, neighbor_pos);
                }
            }
        }

        if (chunk.get_state() == Chunk::State::REMESH && meshes_created < MAX_MESHES_PER_FRAME) {
            renderer::ChunkMesh *old_mesh = chunk.get_mesh();
            if (old_mesh) {
                delete_chunk_mesh(old_mesh);
            }
            chunk.set_mesh(nullptr);

            if (chunk.get_block_count() > 0) {
                renderer::ChunkMesh *chunk_mesh = renderer::create_chunk_mesh(chunk, m_world);
                chunk.set_mesh(chunk_mesh);
                meshes_created++;
            }
            chunk.set_state(Chunk::State::READY);
        }

        if (chunk.get_state() == Chunk::State::READY && chunk.get_mesh()) {
            renderer::ChunkMesh *chunk_mesh = chunk.get_mesh();
            m_renderer.render_chunk(*chunk_mesh, view, false);
        }
    }

    for (auto &chunk: chunks) {
        if (chunk.get_state() == Chunk::State::READY && chunk.get_mesh()) {
            if (chunk.get_mesh()->transparent_mesh.num_indices > 0) {
                renderer::ChunkMesh *chunk_mesh = chunk.get_mesh();
                m_renderer.render_chunk(*chunk_mesh, view, true);
            }
        }
    }
}

void Minecraft::update() {
    m_world.update(m_camera.position());
}

void Minecraft::process_input() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }

    float camera_speed = 20.0f * m_delta_time;

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.move(CameraDirection::FORWARD, camera_speed);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.move(CameraDirection::BACKWARD, camera_speed);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.move(CameraDirection::LEFT, camera_speed);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.move(CameraDirection::RIGHT, camera_speed);
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_camera.move(CameraDirection::UP, camera_speed);
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_camera.move(CameraDirection::DOWN, camera_speed);
}
