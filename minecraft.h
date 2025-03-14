//
// Created by pknadimp on 3/13/25.
//

#ifndef MINECRAFT_H
#define MINECRAFT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "camera.h"
#include "world.h"


class Minecraft {
public:
    Minecraft(int width = 800, int height = 600) : m_width(width), m_height(height) {
        initialize();
    };

    bool initialize();

    void handle_mouse_move(double xpos, double ypos);

    void resize(int width, int height);

    void run();

    void render();

private:
    void process_input();

private:
    int m_width, m_height;
    GLFWwindow *m_window;

    renderer::Renderer m_renderer = renderer::Renderer();
    World m_world = World();
    Camera m_camera = Camera();

    float m_delta_time = 0.0f;
    float m_last_frame = 0.0f;

    bool m_first_mouse = true;
    double m_last_X = 0;
    double m_last_Y = 0;
};


#endif //MINECRAFT_H
