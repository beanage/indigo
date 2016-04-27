#pragma once

#include <string>

namespace indigo {
namespace filesystem {
bool is_directory(std::string const& path);
bool is_file(std::string const& path);
std::string join(std::string const& first, std::string const& second);
std::string extension(std::string const& path);
}
}
