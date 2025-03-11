//
// Created by pknadimp on 3/10/25.
//

#include "utils.h"
#include <fstream>
#include <sstream>

std::optional<std::string> utils::read_file(const std::string &file_path) {
    std::stringstream contents;
    std::ifstream file(file_path);

    if (!file.is_open()) {
        return std::nullopt;
    }

    contents << file.rdbuf();

    file.close();
    return contents.str();
}
