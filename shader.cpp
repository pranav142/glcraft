//
// Created by pknadimp on 3/10/25.
//

#include "shader.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

renderer::Shader::Shader(const std::string &vertex_path, const std::string &fragment_path) {
    std::optional<unsigned int> vertex_shader = compile_vertex_shader(vertex_path);
    if (!vertex_shader.has_value()) {
        return;
    }

    std::optional<unsigned int> fragment_shader = compile_fragment_shader(fragment_path);
    if (!fragment_shader.has_value()) {
        return;
    }

    std::optional<unsigned int> shader_program = create_shader_program(vertex_shader.value(), fragment_shader.value());
    if (!shader_program.has_value()) {
        return;
    }

    m_id = shader_program.value();

    glDeleteShader(vertex_shader.value());
    glDeleteShader(fragment_shader.value());
}

void renderer::Shader::use() const {
    glUseProgram(m_id);
}

unsigned int renderer::Shader::ID() const {
    return m_id;
}

void renderer::Shader::set_bool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int) value);
}

void renderer::Shader::set_int(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void renderer::Shader::set_float(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}


std::optional<unsigned int> renderer::Shader::compile_vertex_shader(const std::string &vertex_path) {
    int success;
    char info_log[512];

    auto vertex_src = utils::read_file(vertex_path);
    if (!vertex_src.has_value()) {
        std::cout << "Failed to read vertex source" << std::endl;
        return std::nullopt;
    }
    const char *vertex_shader_code = vertex_src.value().c_str();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
        return std::nullopt;
    };

    return vertex_shader;
}

std::optional<unsigned int> renderer::Shader::compile_fragment_shader(const std::string &fragment_path) const {
    int success;
    char info_log[512];

    auto fragment_src = utils::read_file(fragment_path);
    if (!fragment_src.has_value()) {
        std::cout << "Failed to read fragment shader source" << std::endl;
        return std::nullopt;
    }
    const char *fragment_shader_code = fragment_src.value().c_str();

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        return std::nullopt;
    }

    return fragment_shader;
}

std::optional<unsigned int> renderer::Shader::create_shader_program(unsigned int vertex_shader,
                                                                    unsigned int fragment_shader) const {
    int success;
    char info_log[512];

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        return std::nullopt;
    }

    return shader_program;
}
