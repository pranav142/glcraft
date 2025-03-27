//
// Created by pknadimp on 3/10/25.
//

#ifndef SHADER_H
#define SHADER_H

#include "utils.h"
#include <string>
#include <glm/glm.hpp>

namespace renderer {
    class Shader {
    public:
        Shader() = default;

        void initialize(const std::string& vertex_path, const std::string& fragment_path);

        void use() const;

        [[nodiscard]] unsigned int ID() const;

        void set_bool(const std::string &name, bool value) const;

        void set_int(const std::string &name, int value) const;

        void set_float(const std::string &name, float value) const;

        void set_matrix(const std::string &name, const glm::mat4& matrix) const;

        void set_vec2(const std::string &name, const glm::vec2& value) const;
    private:
        [[nodiscard]] std::optional<unsigned int> compile_vertex_shader(const std::string &vertex_path);

        [[nodiscard]] std::optional<unsigned int> compile_fragment_shader(const std::string &fragment_path) const;

        [[nodiscard]] std::optional<unsigned int> create_shader_program(unsigned int vertex_shader,
                                                                        unsigned int fragment_shader) const;

    private:
        unsigned int m_id = 0;
    };
}


#endif //SHADER_H
