//
// Created by pknadimp on 3/13/25.
//

#include "minecraft.h"

#include <iostream>
#include "block_registry.h"
#include "renderer.h"
#include "mesh_generator.h"

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
    m_renderer.enable_wireframe();

    // m_camera.enable_flying();

    m_world.initialize();

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

    auto chunks = m_world.get_chunks();
    for (auto &chunk: chunks) {
        renderer::ChunkMesh* chunk_mesh = chunk.get_mesh();
        if (chunk_mesh) {
            m_renderer.render_chunk(*chunk_mesh, view);
        }
    }
}

void Minecraft::update() {
    m_world.update(m_camera.position());
}

void Minecraft::process_input() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    float camera_speed = 5.0f * m_delta_time;

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
