#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "block.h"
#include "shader.h"
#include "renderer.h"
#include "chunk.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
double yaw = -90.0f;
double pitch = 0;

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

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 30.0f * deltaTime;

    auto front_dir = glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * front_dir;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * front_dir;

    auto right_dir = glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= right_dir * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += right_dir * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * glm::vec3(0, 1.0, 0);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * glm::vec3(0, 1.0, 0);
}

renderer::ChunkMesh create_stone_chunk(int length, int width, int height, Chunk &chunk) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < height; k++) {
                auto block = create_block(renderer::TextureType::STONE);
                chunk.set_block(i, k, j, block);
            }
        }
    }

    renderer::ChunkMesh chunk_mesh = renderer::create_chunk_mesh(chunk);
    return chunk_mesh;
}

int main() {
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

    for (int i = -6; i <=  6; i++) {
        for (int j = -6; j < 6; j++) {
            Chunk chunk(16 * i, -17, 16 * j);
            create_stone_chunk(16, 16, 16, chunk);
            chunk_meshes.push_back(renderer::create_chunk_mesh(chunk));
        }
    }

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        std::cout << 1/deltaTime << std::endl;
        lastFrame = currentFrame;
        // TODO: only need to resize on changes
        renderer.resize(g_width, g_height);

        processInput(window);

        renderer.begin_frame();

        glm::mat4 view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

        for (auto &chunk_mesh: chunk_meshes) {
            renderer.render_chunk(chunk_mesh, view);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }


    glfwTerminate();
    return 0;
}
