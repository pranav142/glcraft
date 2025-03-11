//
// Created by pknadimp on 3/10/25.
//

#ifndef UTILS_H
#define UTILS_H

#include <optional>
#include <string>

namespace utils {
    std::optional<std::string> read_file(const std::string& file_path);
} // utils

#endif //UTILS_H
