#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "block.h"
#include "block_registry.h"
#include "shader.h"
#include "renderer.h"
#include "chunk.h"
#include "camera.h"

Camera camera = Camera();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int g_width = 800;
int g_height = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    g_height = height;
    g_width = width;
    glViewport(0, 0, width, height);
}

bool firstMouse;
double lastX = 0;
double lastY = 0;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotate(yoffset, xoffset);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 30.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(CameraDirection::FORWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(CameraDirection::BACKWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(CameraDirection::LEFT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(CameraDirection::RIGHT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.move(CameraDirection::UP, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.move(CameraDirection::DOWN, cameraSpeed);
}

renderer::ChunkMesh create_stone_chunk(int length, int width, int height, Chunk &chunk) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < height; k++) {
                if (k == height - 1) {
                    auto block = create_block(BlockTypeID::GRASS);
                    chunk.set_block(i, k, j, block);
                } else {
                    auto block = create_block(BlockTypeID::DIRT);
                    chunk.set_block(i, k, j, block);
                }
            }
        }
    }

    renderer::ChunkMesh chunk_mesh = renderer::create_chunk_mesh(chunk);
    return chunk_mesh;
}

int main() {
    BlockRegistry();
    camera.enable_flying();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    renderer::Renderer renderer(g_width, g_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // renderer.enable_wireframe();
    renderer.enable_culling();

    std::vector<renderer::ChunkMesh> chunk_meshes;

    for (int i = -8; i <= 8; i++) {
        for (int j = -8; j <= 8; j++) {
            Chunk chunk(16 * i, -17, 16 * j);
            create_stone_chunk(16, 16, 16, chunk);
            chunk_meshes.push_back(renderer::create_chunk_mesh(chunk));
        }
    }

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        std::cout << 1 / deltaTime << std::endl;
        lastFrame = currentFrame;
        // TODO: only need to resize on changes
        renderer.resize(g_width, g_height);

        processInput(window);

        renderer.begin_frame();

        glm::mat4 view = camera.view_matrix();

        for (auto &chunk_mesh: chunk_meshes) {
            renderer.render_chunk(chunk_mesh, view);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
